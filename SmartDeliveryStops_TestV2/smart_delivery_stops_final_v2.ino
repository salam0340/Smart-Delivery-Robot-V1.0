#include <Keypad.h>

#define IR_SENSOR_RIGHT 13
#define IR_SENSOR_LEFT 11
#define MOTOR_SPEED 180

//Right motor
int enableRightMotor = 6;
int rightMotorPin1 = 2;
int rightMotorPin2 = 3;
//Left motor
int enableLeftMotor = 5;
int leftMotorPin1 = 4;
int leftMotorPin2 = 9;

// Keypad configuration (4×4)
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
// আপনার পিনগুলো নিচের মতো ম্যাপিং ঠিক আছে
byte rowPins[ROWS] = {A3, A2, A1, A0};
byte colPins[COLS] = {12, 10, 8, 7};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

enum State { WAIT_FOR_KEY, GO_TO_TABLE, RETURN_CIRCULAR };
State state = WAIT_FOR_KEY;

int targetTable = 0;
int nodeCount = 0;
const int TOTAL_STOPS = 6;    // 5 tables + 1 kitchen
int returnStops = 0;
bool countedNode = false;     // node-debounce flag

void setup() {
  Serial.begin(9600);
  // PWM frequency tweak
  TCCR0B = TCCR0B & B11111000 | B00000010;
  
  pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT); pinMode(rightMotorPin2, OUTPUT);
  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);  pinMode(leftMotorPin2, OUTPUT);
  pinMode(IR_SENSOR_RIGHT, INPUT);
  pinMode(IR_SENSOR_LEFT, INPUT);

  rotateMotor(0,0);
  Serial.println("Ready: Press 1-5 for table");
}

void loop() {
  char key = keypad.getKey();
  
  if (state == WAIT_FOR_KEY) {
    if (key >= '1' && key <= '5') {
      targetTable = key - '0';
      nodeCount = 0;
      state = GO_TO_TABLE;
      Serial.print("Table "); Serial.print(targetTable); Serial.println(" selected.");
      delay(3000);
    }
  }
  else if (state == GO_TO_TABLE) {
    lineFollowOnce();
    handleNodeCounting();      // check & count nodes
    if (nodeCount >= targetTable) {
      rotateMotor(0,0);
      Serial.println("At target table! Waiting 3s...");
      delay(5000);
      // prepare circular return
      returnStops = TOTAL_STOPS - targetTable;
      nodeCount = 0;
      countedNode = false;
      state = RETURN_CIRCULAR;
      Serial.print("Returning via circular path, will pass ");
      Serial.print(returnStops);
      Serial.println(" nodes.");
      delay(3000);
    }
  }
  else if (state == RETURN_CIRCULAR) {
    lineFollowOnce();
    handleNodeCounting();
    if (nodeCount >= returnStops) {
      rotateMotor(0,0);
      Serial.println("Back at kitchen! 😊");
      state = WAIT_FOR_KEY;
    }
  }
}

// Line-follow one step
void lineFollowOnce() {
  int r = digitalRead(IR_SENSOR_RIGHT), l = digitalRead(IR_SENSOR_LEFT);
  if (r==LOW && l==LOW)           rotateMotor(MOTOR_SPEED, MOTOR_SPEED);
  else if (r==HIGH && l==LOW)     rotateMotor(-MOTOR_SPEED, MOTOR_SPEED);
  else if (r==LOW && l==HIGH)     rotateMotor(MOTOR_SPEED, -MOTOR_SPEED);
  else                             rotateMotor(0,0);
}

// Handle node detection with debounce
void handleNodeCounting() {
  bool atNode = (digitalRead(IR_SENSOR_RIGHT)==HIGH && digitalRead(IR_SENSOR_LEFT)==HIGH);
  if (atNode && !countedNode) {
    nodeCount++;
    countedNode = true;
    Serial.print("Node #"); Serial.println(nodeCount);
    // একটু সরিয়ে নিয়ে আসা, যাতে continuous HIGH না পড়ে
    rotateMotor(MOTOR_SPEED, MOTOR_SPEED);
    delay(200);
  }
  else if (!atNode) {
    // সেন্সর সাদা হলে আবার কাউন্ট করার জন্য flag ফালিয়ে দাও
    countedNode = false;
  }
}

// detect a “node” (both sensors see black)
bool isNode() {
  return (digitalRead(IR_SENSOR_RIGHT)==HIGH && digitalRead(IR_SENSOR_LEFT)==HIGH);
}

// motor control
void rotateMotor(int rSpeed, int lSpeed) {
  // right motor
  if (rSpeed<0)      { digitalWrite(rightMotorPin1,LOW);  digitalWrite(rightMotorPin2,HIGH); }
  else if (rSpeed>0) { digitalWrite(rightMotorPin1,HIGH); digitalWrite(rightMotorPin2,LOW); }
  else               { digitalWrite(rightMotorPin1,LOW);  digitalWrite(rightMotorPin2,LOW); }
  // left motor
  if (lSpeed<0)      { digitalWrite(leftMotorPin1,LOW);   digitalWrite(leftMotorPin2,HIGH); }
  else if (lSpeed>0) { digitalWrite(leftMotorPin1,HIGH);  digitalWrite(leftMotorPin2,LOW); }
  else               { digitalWrite(leftMotorPin1,LOW);   digitalWrite(leftMotorPin2,LOW); }
  analogWrite(enableRightMotor, abs(rSpeed));
  analogWrite(enableLeftMotor,  abs(lSpeed));
}
