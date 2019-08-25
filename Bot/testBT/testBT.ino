#include <CheapStepper.h>
#include <SoftwareSerial.h>

// Number of steps per rotation:
const int stepsPerRevolution = 64;

//Wiring:
// Pin 2 to IN1 on the ULN2003 driver
// Pin 3 to IN2 on the ULN2003 driver
// Pin 4 to IN3 on the ULN2003 driver
// Pin 5 to IN4 on the ULN2003 driver
CheapStepper rightStepper (2, 3, 4, 5);
CheapStepper leftStepper (6, 7, 8, 9);

unsigned long moveStartTime = 0; 

SoftwareSerial Bluetooth(12, 11); // RX, TX
int Data; // the data received

void setup() {  
  Bluetooth.begin(9600);
  Serial.begin(9600);
  Serial.println("Waiting for command...");
  Bluetooth.println("Send 'up' to turn on the MOTORS. Send 'stop' to turn Off");
}

void loop() {
  rightStepper.run();
  leftStepper.run();

  if (Bluetooth.available()) { //wait for data received
    Data = Bluetooth.read();
    if (Data == 'u') {
      Serial.println("Forward!");
      Bluetooth.println("Forward!");

      rightStepper.setRpm(24);
      leftStepper.setRpm(24);
      rightStepper.newMoveCW(4096);
      leftStepper.newMoveCCW(4096);
      moveStartTime = millis(); // reset move start time
    }
    else if (Data == 'd') {
      Serial.println("Backward!");
      Bluetooth.println("Backward!");
      
      rightStepper.setRpm(24);
      leftStepper.setRpm(24);
      rightStepper.newMoveCCW(4096);
      leftStepper.newMoveCW(4096);
      moveStartTime = millis(); // reset move start time
    }
    else if (Data == 'l') {
      Serial.println("Left!");
      Bluetooth.println("Right!");
      
      rightStepper.setRpm(24);
      leftStepper.setRpm(24);
      rightStepper.newMoveCCW(4096);
      leftStepper.newMoveCCW(4096);
      moveStartTime = millis(); // reset move start time
    }
    else if (Data == 'r') {
      Serial.println("Left!");
      Bluetooth.println("Right!");
      
      rightStepper.setRpm(24);
      leftStepper.setRpm(24);
      rightStepper.newMoveCW(4096);
      leftStepper.newMoveCW(4096);
      moveStartTime = millis(); // reset move start time
    }
    else if (Data == 's') {
      Serial.println("MOTORS Off!");
      Bluetooth.println("MOTORS Off ! ");
      
      rightStepper.stop();
      leftStepper.stop();
      moveStartTime = millis(); // reset move start time
    }
    else {
      ;
    }
  }
}
