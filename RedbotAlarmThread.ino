#include <Time.h>
#include <TimeAlarms.h>
#include <RedBot.h>

#include <Thread.h>
#include <ThreadController.h>

// ThreadController that will controll all threads
ThreadController control = ThreadController();

//Thread initialized
Thread buzz = Thread();
Thread movement = Thread();

RedBotMotors motors;

RedBotBumper lBumper = RedBotBumper(3);  // initialzes bumper object on pin 3
RedBotBumper rBumper = RedBotBumper(11); // initialzes bumper object on pin 11

int buttonPin = 12; // variable to store the button Pin 

int lBumperState;  // state variable to store the bumper value
int rBumperState;  // state variable to store the bumper value

// Create a couple of constants for our pins.
const int buzzerPin = 9;

//CALLBACKS
//*******************************
// callback for myThread
void buzzCallback(){
  while(digitalRead(buttonPin) == HIGH){
    tone(buzzerPin, 1000);   // Play a 1kHz tone on the pin number held in
    //  the variable "buzzerPin". 
    delay(1000);   // Wait for 125ms. 
    noTone(buzzerPin);   // Stop playing the tone.
    delay(500);
    tone(buzzerPin, 2000);  // Play a 2kHz tone on the buzzer pin
    delay(1000);
    noTone(buzzerPin);
    delay(500);   // delay for 1000 ms (1 second)
  }
}

// callback for hisThread
void movementCallback(){
  while(true){
    motors.drive(255);
    lBumperState = lBumper.read();  // default INPUT state is HIGH, it is LOW when bumped
    rBumperState = rBumper.read();  // default INPUT state is HIGH, it is LOW when bumped
  
    if (lBumperState == LOW) // left side is bumped/
    { 
      motors.brake();
      reverse();    // backs up
      turnRight();  // turns
    }
  
    else if (rBumperState == LOW) // right side is bumped/
    { 
      motors.brake();
      reverse();   // backs up
      turnLeft();  // turns
    }
  }
}

//MOVEMENT
//**********************************
//reverse function
void reverse()
{
  motors.drive(-255);
  delay(500);
  motors.brake();
  delay(100);  // short delay to let robot fully stop
}

// turnRight() function -- turns RedBot to the Right
void turnRight()
{
  motors.leftMotor(-150);  // spin CCW
  motors.rightMotor(-150); // spin CCW
  delay(500);
  motors.brake();
  delay(100);  // short delay to let robot fully stop
}

// turnRight() function -- turns RedBot to the Left
void turnLeft()
{
  motors.leftMotor(+150);  // spin CW
  motors.rightMotor(+150); // spin CW
  delay(500);
  motors.brake();
  delay(100);  // short delay to let robot fully stop
}

//MAIN Methods
void setup(){
	Serial.begin(9600);

	// Configure myThread
	buzz.onRun(buzzCallback);
	buzz.setInterval(250);

	// Configure myThread
	movement.onRun(movementCallback);
	movement.setInterval(250);

	// Adds both threads to the controller
	control.add(&buzz);
	control.add(&movement); // & to pass the pointer to it
  
}

void loop(){
	// run ThreadController
	// this will check every thread inside ThreadController,
	// if it should run. If yes, he will run it;
	control.run();
}


