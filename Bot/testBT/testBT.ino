// Calculate based on max Input size expected for one command
#define INPUT_SIZE 30

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

// Pins used for the Bluetooth serial
SoftwareSerial Bluetooth(12, 11); // RX, TX

// Get next command from Serial (add 1 for final 0)
char Input[INPUT_SIZE + 1];
// Current index in the input for reading input per char
int CurrentIndex = 0;
// Indicates that all bytes are received for the current command
bool DataReceived = false;

// The setup method run once (afdter each reset)
void setup() {  
  // Set up the Bluetooth serial connection
  Bluetooth.begin(9600);

  // Set up the Serial connection can be used for debugging
  // Serial.begin(9600);
  // Serial.println("Waiting for command...");

  // Print the commands to Bleutooth serial to be used on a Phone
  // (for example https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal&hl=en_US)
  Bluetooth.println("Send 's' to stop");
  Bluetooth.println("Send 'f: <distance in cm>' to move Forward");
  Bluetooth.println("Send 'b: <distance in cm>' to move Backward");
  Bluetooth.println("Send 'w: <angle in degrees>' to rotate clockWise");
  Bluetooth.println("Send 'c: <angle in degrees>' to move Counter-clockwise");
}

// The loop method (run continously in the Arduino loop)
void loop() {
  // Each loop, run the stepper motors to keep the bot moving
  rightStepper.run();
  leftStepper.run();
  
  // When there is a byte to read, read it
  if (Bluetooth.available()) {
    char character = Bluetooth.read();
    
    if(character == '\r'){
      DataReceived = true;
      PrintToSerials("Command received");
      Input[CurrentIndex] = 0;
      CurrentIndex = 0;
    } else {
      Input[CurrentIndex] = character;
      CurrentIndex = CurrentIndex + 1;
    }
  }

  // When all data is receaived parse the command and execute it
  if(DataReceived){
    char directive = GetDirective(Input);
    int number = GetNumber(Input);

    // When the number cannot be read (except for the "stop" command),
    // do not trigger the motors
    if(directive != 's' && number <= 0) {
      PrintToSerials("Could not parse the number");
    } else {    
      switch(directive) {
        case 's':
          Stop();
          break;
        case 'f':
          MoveForward(number);
          break;
        case 'b':
          MoveBackward(number);
          break;
        case 'w':
          RotateClockwise(number);
          break;
        case 'c':
          RotateCounterClockwise(number);
          break;
        default:
          PrintToSerials("unknown command");
      }
    }

    // Set the flag to false to receive the next command
    DataReceived = false;
  }
}

char GetDirective(char* input){
  return Input[0];
}

int GetNumber(char* input){
  // Split the Input in two values
  char* separator = strchr(Input, ':');
  if (separator != 0) {
      *separator = 0;
      ++separator;
      // Parse the number
      return atoi(separator);
  }

  return -1;
}

void Stop(){
  PrintToSerials("MOTORS Off!");
  
  rightStepper.stop();
  leftStepper.stop();

  moveStartTime = millis(); // reset move start time
}

void MoveForward(int distance){
  PrintToSerials("Forward: " + String(distance));
  
  rightStepper.setRpm(48);
  leftStepper.setRpm(48);
  rightStepper.newMoveCW(4096 * distance);
  leftStepper.newMoveCCW(4096 * distance);

  moveStartTime = millis(); // reset move start time
}

void MoveBackward(int distance){
  PrintToSerials("Backward: " + String(distance));
  
  rightStepper.setRpm(48);
  leftStepper.setRpm(48);
  rightStepper.newMoveCCW(4096 * distance);
  leftStepper.newMoveCW(4096 * distance);

  moveStartTime = millis(); // reset move start time
}

void RotateClockwise(int angle){
  PrintToSerials("Clockwise: " + String(angle));

  rightStepper.setRpm(24);
  leftStepper.setRpm(24);
  rightStepper.newMoveCW(4096*angle/180);
  leftStepper.newMoveCW(4096*angle/180);   

  moveStartTime = millis(); // reset move start time
}

void RotateCounterClockwise(int angle){
  PrintToSerials("Counter-clockwise: " + String(angle));

  rightStepper.setRpm(24);
  leftStepper.setRpm(24);
  rightStepper.newMoveCCW(4096*angle/180);
  leftStepper.newMoveCCW(4096*angle/180);   

  moveStartTime = millis(); // reset move start time
}

void PrintToSerials(String message){
    // Serial.println(message);    
    Bluetooth.println(message);
}
