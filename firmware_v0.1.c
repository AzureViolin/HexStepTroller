// by Azureviolin, modified from Arduino Stepper.cpp V.04

#include <ioavr.h>

#define LOW 0
#define HIGH 1

#define bit_get(p,m) ((p) & (m)) 
#define bit_set(p,m) ((p) |= (m)) 
#define bit_clear(p,m) ((p) &= ~(m)) 
#define bit_flip(p,m) ((p) ^= (m)) 
#define bit_write(c<Plug>PeepOpen,m) (c ? bit_set(p,m) : bit_clear(p,m)) 
#define BIT(x) (0x01 << (x)) 
#define LONGBIT(x) ((unsigned long)0x00000001 << (x))


// struct of Stepper Motor
struct Stepper{
	int direction;
	int speed; // RPM
	unsigned long step_delay; //ms
	int number_of_steps; 
	int pint_count; // 2 pin or 4 pin configuration
	int step_number; //which step the motor is on now

	// motor pin numbers:

	char motor_port_1;
	int motor_pin_1;
	char motor_port_2;
	int motor_pin_2;
	char motor_port_3;
	int motor_pin_3;
	char motor_port_4;
	int motor_pin_4;

	long last_step_time //ms, time stamp when last step was taken
} Base,Shoulder,Elbow,Right_Wrist,Hand;

Stepper Left_Wrist = {
	int direction=0;
	int speed=0; // RPM
	unsigned long step_delay=10; //ms
	int number_of_steps=200; 
	int pin_count=4; // 2 pin or 4 pin configuration
	int step_number=0; //Initial Position. which step the motor is on now

	// motor pin numbers:

	char motor_port_1=A;
	int motor_pin_1=0;
	char motor_port_2=A;
	int motor_pin_2=1;
	char motor_port_3=A;
	int motor_pin_3=2;
	char motor_port_4=A;
	int motor_pin_4=3;

	long last_step_time=0;
}



// Move Stepper. This function calls millis() and stepMotor()
void step(Stepper stepper, int steps_to_move){
	int steps_left = 0;
	if (steps_to_move > 0) {
		steps_left = steps_to_move; // how many steps to take
		stepper.direction = 1;// determin direction
	} 
    else {
		steps_left = -steps_to_move;
		stepper.direction = 0;
	}
	
	// decrease the number of steps, moving on step each time:
	while(steps_left >0) {
	// move only if the appropriate delay has passed:
	if (millis() - stepper.last_step_time >= stepper.step_delay){
		// get the timeStamp of when you stepped:
		stepper.last_step_time = millis();
		// increase or decrease the step number, depending on direction
		if (stepper.direction == 1) {
			stepper.step_number++;
			if (stepper.step_number == stepper.number_of_steps) {
          		stepper.step_number = 0;
        	}
		}
      	else { 
        	if (stepper.step_number == 0) {
          	stepper.step_number = stepper.number_of_steps;
        	}
        stepper.step_number--;
      	}
      // decrease the steps left:
      steps_left--;
      // step the motor to step number 0, 1, 2, or 3:
      stepMotor(stepper.step_number % 4);
    }
  	}
}


	/// Set Speed, in RPM
void setSpeed(Stepper stepper, long whatSpeed){
	stepper.step_delay = 60L * 1000L / stepper.number_of_steps / whatSpeed;
}

// Moves the motor by 1 step. calls clearPin and setPin.
int stepMotor(Stepper stepper, int thisStep) {
  if (stepper.pin_count == 2) {
    switch (thisStep) {
      case 0: // 01 
      clearPin(stepper.motor_port_1,stepper.motor_pin_1);
      setPin(stepper.motor_port_2,stepper.motor_pin_2);
      break;
      case 1: // 11 
      setPin(stepper.motor_port_1,stepper.motor_pin_1);
      setPin(stepper.motor_port_2,stepper.motor_pin_2);
      break;
      case 2: // 10 
      setPin(stepper.motor_port_1,stepper.motor_pin_1);
      clearPin(stepper.motor_port_2,stepper.motor_pin_2);
      break;
      case 3: // 00 
      clearPin(stepper.motor_port_1,stepper.motor_pin_1);
      clearPin(stepper.motor_port_2,stepper.motor_pin_2);
      break;
    } 
  }
  if (stepper.pin_count == 4) {
    switch (thisStep) {
      case 0:    // 1010
      setPin(stepper.motor_port_1,stepper.motor_pin_1);
      clearPin(stepper.motor_port_2,stepper.motor_pin_2);
      setPin(stepper.motor_port_3,stepper.motor_pin_3);
      clearPin(stepper.motor_port_4,stepper.motor_pin_4);
      break;
      case 1:    // 0110
      clearPin(stepper.motor_port_1,stepper.motor_pin_1);
      setPin(stepper.motor_port_2,stepper.motor_pin_2);
      setPin(stepper.motor_port_3,stepper.motor_pin_3);
      clearPin(stepper.motor_port_4,stepper.motor_pin_4);
      break;
      case 2:    //0101
      clearPin(stepper.motor_port_1,stepper.motor_pin_1);
      setPin(stepper.motor_port_2,stepper.motor_pin_2);
      clearPin(stepper.motor_port_3,stepper.motor_pin_3);
      setPin(stepper.motor_port_4,stepper.motor_pin_4);
      break;
      case 3:    //1001
      setPin(stepper.motor_port_1,stepper.motor_pin_1);
      clearPin(stepper.motor_port_2,stepper.motor_pin_2);
      clearPin(stepper.motor_port_3,stepper.motor_pin_3);
      setPin(stepper.motor_port_4,stepper.motor_pin_4);
      break;
    } 
  }
}


// set selected pin to 1/HIGH 
void setPin(char portNum, int pinNum){
	switch (portNum){
		//PORTA 
		case 'A':{
			switch (pinNum){
				case 0: bit_set(PORTA_OUTSET,BIT(0)); break; //Set PA0 = HIGH
				case 1: bit_set(PORTA_OUTSET,BIT(1)); break;
				case 2: bit_set(PORTA_OUTSET,BIT(2)); break;
				case 3: bit_set(PORTA_OUTSET,BIT(3)); break;
			}
		break;
		}
	}
}





// set selected pin to 0/LOW 
void clearPin(char portNum, int pinNum){
	switch (portNum){
		//PORTA 
		case 'A':{
			switch (pinNum){
				case 0: bit_set(PORTA_OUTCLR,BIT(0)); break; //Set PA0 = HIGH
				case 1: bit_set(PORTA_OUTCLR,BIT(1)); break;
				case 2: bit_set(PORTA_OUTCLR,BIT(2)); break;
				case 3: bit_set(PORTA_OUTCLR,BIT(3)); break;
			}
		break;
		}
	}
}
// getTime
long getTime();

// setPinMode
void setPinMode(char portNum, int pinNum, int direction)
	OUTPUT =1 
	INPUT =0
	
// move all the stepper one by one
void action();
	

// Compute the required speed for each stepper 
void setSpeedAll(int stepper_amount){
	current_stepper=from 1 to stepper_amount
	setSpeed(current_stepper);
}

// Read instructions from usb/serial port, and stores time and destination angle in global variables 
void readInstruction(int port);
{USB communication with computer, store stuff in global variable
}

// Move the whole robotic arm to a destination within a predefined time 
void moveArm();
{read global variable: time, step1angle, step2angle, ... step6angle
	setSpeedAll();
	action();
}

//test main

int main(void){

PORTA_DIR = 0XFF;//Set PortA to output

setSpeed(Left_Wrist,30);
step(Left_Wrist,50);
while(1){
	if(Left_Wrist.step_number=50)
		step(Left_Wrist,-50);
	}
return 0;
}

/*
int main(void){
	while(1){
		readInstruction(port);
		moveArm();
	}
}
*/

/*
int reference_main(void) 
{ 
	PORTD_DIR = 0xFF; 
	while (1) { 
		PORTD_OUT = 0xAA; 
		_delay_ms(100); 
		PORTD_OUT = 0x55; 
		_delay_ms(100); 
	}									  
}
*/
