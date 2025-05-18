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
byte rowPins[ROWS] = {A3, A2, A1, A0};  // Arduino analog pins
byte colPins[COLS] = {12, 10, 8, 7};     // Digital pins
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// State vars
enum State { WAIT_FOR_KEY, GO_TO_TABLE, RETURN_HOME };
State state = WAIT_FOR_KEY;
int targetTable = 0;
int nodeCount = 0;

void setup() {
  Serial.begin(9600);

  // PWM frequency tweak for pins 5 & 6
  TCCR0B = TCCR0B & B11111000 | B00000010;

  pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);

  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  pinMode(IR_SENSOR_RIGHT, INPUT);
  pinMode(IR_SENSOR_LEFT, INPUT);

  rotateMotor(0,0);
  Serial.println("Ready: Press 1-4 to choose table");
}

void loop() {
  char key = keypad.getKey();
  if (state == WAIT_FOR_KEY) {
    if (key >= '1' && key <= '4') {
      targetTable = key - '0';
      nodeCount = 0;
      state = GO_TO_TABLE;
      Serial.print("Table ");
      Serial.print(targetTable);
      Serial.println(" selected. চলছি!");
      delay(500);
    }
  }
  else if (state == GO_TO_TABLE) {
    lineFollowOnce();
    // Detect node: both sensors on black
    if (digitalRead(IR_SENSOR_RIGHT)==HIGH && digitalRead(IR_SENSOR_LEFT)==HIGH) {
      nodeCount++;
      Serial.print("Node #"); Serial.println(nodeCount);
      // একটু ঘুরে যেতে দিচ্ছি যাতে সন থেকে সরে যায়
      rotateMotor(MOTOR_SPEED, MOTOR_SPEED);
      delay(200);
    }
    if (nodeCount >= targetTable) {
      // পৌঁছে গেল
      rotateMotor(0,0);
      Serial.println("পৌঁছেছি! ৩ সেকেন্ড অপেক্ষা করছি।");
      delay(3000);
      nodeCount = 0;
      state = RETURN_HOME;
      Serial.println("হোমে ফিরছি...");
      delay(500);
    }
  }
  else if (state == RETURN_HOME) {
    // ঘুরে ঘুরে ঘরে ফিরবে যতক্ষণ encounter দুইবার ব্ল্যাক-বক্স
    lineFollowOnce();
    if (digitalRead(IR_SENSOR_RIGHT)==HIGH && digitalRead(IR_SENSOR_LEFT)==HIGH) {
      nodeCount++;
      Serial.print("Return node #"); Serial.println(nodeCount);
      delay(200);
    }
    if (nodeCount >= 2) {  // দুইবার ব্ল্যাক-বক্স → হোম
      rotateMotor(0,0);
      Serial.println("হোমে ফিরে এলো! আবার কী চাপবে? 😃");
      state = WAIT_FOR_KEY;
    }
  }
}

// একবারের জন্য লাইন ফলোয়ার স্টেপ
void lineFollowOnce() {
  int r = digitalRead(IR_SENSOR_RIGHT);
  int l = digitalRead(IR_SENSOR_LEFT);
  if (r==LOW && l==LOW)           rotateMotor(MOTOR_SPEED, MOTOR_SPEED);
  else if (r==HIGH && l==LOW)     rotateMotor(-MOTOR_SPEED, MOTOR_SPEED);
  else if (r==LOW && l==HIGH)     rotateMotor(MOTOR_SPEED, -MOTOR_SPEED);
  else                             rotateMotor(0,0);
}

// মোটর ঘোরানোর ফাংশন আগের মতোই
void rotateMotor(int rightMotorSpeed, int leftMotorSpeed) {
  if (rightMotorSpeed < 0) {
    digitalWrite(rightMotorPin1,LOW); digitalWrite(rightMotorPin2,HIGH);
  } else if (rightMotorSpeed > 0) {
    digitalWrite(rightMotorPin1,HIGH); digitalWrite(rightMotorPin2,LOW);
  } else {
    digitalWrite(rightMotorPin1,LOW); digitalWrite(rightMotorPin2,LOW);
  }
  if (leftMotorSpeed < 0) {
    digitalWrite(leftMotorPin1,LOW); digitalWrite(leftMotorPin2,HIGH);
  } else if (leftMotorSpeed > 0) {
    digitalWrite(leftMotorPin1,HIGH); digitalWrite(leftMotorPin2,LOW);
  } else {
    digitalWrite(leftMotorPin1,LOW); digitalWrite(leftMotorPin2,LOW);
  }
  analogWrite(enableRightMotor, abs(rightMotorSpeed));
  analogWrite(enableLeftMotor, abs(leftMotorSpeed));
}
