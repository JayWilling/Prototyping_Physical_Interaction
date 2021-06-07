
// Contact 10 -> Motor 6 -- 90 OPEN , 0 CLOSED
// Contact 11 -> Motor 4 -- 0 OPEN , 90 CLOSED
// Contact 12 -> Motor 5 -- 0 OPEN , 90 CLOSED
// Contact 13 -> Motor 7 -- 90 OPEN , 0 CLOSED


#include <Servo.h>
#include <stdlib.h>

// Create a new servo object:
Servo myservo;
Servo myservo1;
Servo myservo2;
Servo myservo3;

// Define the servo pin:
#define servoPin 6
#define servoPin1 4
#define servoPin2 5
#define servoPin3 7


// Arrays to store the pins, servos, and open positions. Maintains their order
Servo servoArray[4] = {myservo, myservo1, myservo2, myservo3};
int contactArray[4] = {10, 11, 12, 13}; //each endpoint
int openPositions[4] = {90, 0, 0, 90};
int mazeState = 0;
bool opened = false;
bool exitReached = false;
int motorPosition = 0;

// Create a variable to store the servo position:
int angle = 0;
void setup() {
//   Attach the Servo variable to a pin:
  Serial.begin(9600);
  myservo.attach(servoPin);
  myservo1.attach(servoPin1);
  myservo2.attach(servoPin2);
  myservo3.attach(servoPin3);
  for (int j =0; j< 4; j++) { //loops through each end point including delay 
    servoArray[j].write((openPositions[j] - 90)* -1);
  }
  delay(1500);

//displaying each end points status either being an idle of 1 or an input of 0
  pinMode(contactArray[0], INPUT_PULLUP); 
  pinMode(contactArray[1], INPUT_PULLUP);
  pinMode(contactArray[2], INPUT_PULLUP);
  pinMode(contactArray[3], INPUT_PULLUP);
}
void loop() {
  // Open all gates
//  Serial.print(digitalRead(contactArray[0]));
//  Serial.print(", ");
//  Serial.print(digitalRead(contactArray[1]));
//  Serial.print(", ");
//  Serial.print(digitalRead(contactArray[2]));
//  Serial.print(", ");
//  Serial.print(digitalRead(contactArray[3]));
//  Serial.print(", ");
//  Serial.println();
  if (Serial.available() > 0) {
    String piCommand = Serial.readStringUntil('\n');
    if (piCommand == "Playing") {
      Serial.print("Playing");
      Serial.println();
      if (!opened && !exitReached) {
        myservo.write(90);//90 open
        delay(1000);
        myservo1.write(0);//0 open
        delay(1000);
        myservo2.write(0);// 0 open
        delay(1000);
        myservo3.write(90);// 90 open
        delay(1000);
        opened = true;
      }
      
      // Read the position of the current end gate
      motorPosition = openPositions[mazeState];
      
      // Flicker the current end gate until the maze is completed
      if (digitalRead(contactArray[mazeState]) && !exitReached) { //flickers the motor for port 6
        delay(1);
        // Set the next motor position
        if (!digitalRead(contactArray[mazeState])){
          exitReached = true; //exits while loop to rotate door if end point is reached
        }
        servoArray[mazeState].write((motorPosition - 90)* -1);
        delay(1250);
        if (!digitalRead(contactArray[mazeState])) {
          exitReached = true;//exits while loop to rotate door if end point is reached
        }
        servoArray[mazeState].write(motorPosition);
        delay(1250);
        if (!digitalRead(contactArray[mazeState])) {
          exitReached = true;//exits while loop to rotate door if end point is reached
        }
      } else {
        exitReached = true;
      }
      if (exitReached && opened) {
        servoArray[mazeState].write((motorPosition - 90)* -1);
        opened = false;
        for (int j =0; j< 4; j++) { //loops through each end point including delay 
          servoArray[j].write((openPositions[j] - 90)* -1);
        }
        delay(1500);
        Serial.print("Win");
        Serial.println();
        mazeState++;
      } else if (exitReached && !opened) {
        Serial.print("Win");
        Serial.println();
      }
    } else if (piCommand == "Celebrate") {
      for (int i = 0; i < 5; i++) {
        servoArray[0].write(openPositions[0]);
        servoArray[2].write(openPositions[2]);
        delay(1);
        servoArray[0].write((openPositions[0] - 90) * -1);
        servoArray[2].write((openPositions[2] - 90) * -1);
        delay(1);
        servoArray[1].write(openPositions[1]);
        servoArray[3].write(openPositions[3]);
        delay(1);
        servoArray[1].write((openPositions[1] - 90) * -1);
        servoArray[3].write((openPositions[3] - 90) * -1);
        delay(1);
      }
    } else {
      exitReached = false;
      Serial.flush();
    }
  }
}
