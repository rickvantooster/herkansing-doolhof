#include <NewPing.h>
#include "segment_display.h"
#include "state.h"
#include "utils.h"
#include "sensors.h"

/*
* constants om bepaalde acties vertduidelijken.
*/

const bool ROTATE_CLOCKWISE = false;
const bool ROTATE_COUNTER_CLOCKWISE = true;
const int SPEED = 75; //exact value tbd later
const int TURN_SPEED = 70; //exact value tbd later
const int CHECK_FINISH_SPEED = 20;


/*
* pins voor motors.
*/

const uint32_t MOTOR_LEFT = 13;
const uint32_t MOTOR_RIGHT = 12;

const uint32_t SPEED_LEFT = 11;
const uint32_t SPEED_RIGHT = 3;


/*
* waardes voor de lijn sensoren gegroepeerd op actie.
*/


const uint8_t VALUES_FORWARD[] = {
	0b11011,
	0b00011,

};
const uint8_t VALUES_RIGHT[] = {
	0b11000,
	0b11001,
	0b11100,
	0b11101,
	0b11110 //TODO: testen zonder deze waarde
};
const uint8_t VALUES_LEFT[] = {
	0b10011, 
	0b10111, 
	0b00111,
	0b01111, //TODO: testen zonder deze waarde
};

const uint8_t VALUE_POSSIBLE_FINISH = 0b00000;
const uint8_t VALUE_UTURN = 0b11111;


/*
* overige variabelen.
*/

uint64_t countdown_time = 0;
uint8_t last_found_line = 0;
uint8_t state = STARTUP_COUNTDOWN;
uint8_t countdown_step = 10;
uint32_t start_time_driving = 0;
bool display_on = true;
uint8_t finish_blink_remaining = 6;
uint32_t finish_time = 0;




/*
* motor related functions
*/

void motors_init(){
	TCCR2B = TCCR2B & B11111000 | B00000111; // for PWM frequency of 30.64 Hz

	pinMode(MOTOR_LEFT, OUTPUT);
	pinMode(MOTOR_RIGHT, OUTPUT);
}

void forward(){
  digitalWrite(MOTOR_LEFT, ROTATE_COUNTER_CLOCKWISE);
  analogWrite(SPEED_LEFT, SPEED);
  digitalWrite(MOTOR_RIGHT, ROTATE_CLOCKWISE);
  analogWrite(SPEED_RIGHT, SPEED);
	display_set_digits(3, 0);
}

void backward(){
  digitalWrite(MOTOR_LEFT, ROTATE_CLOCKWISE);
  digitalWrite(MOTOR_RIGHT, ROTATE_COUNTER_CLOCKWISE);
  analogWrite(SPEED_LEFT, SPEED);
  analogWrite(SPEED_RIGHT, SPEED);
}


void stop(){
  analogWrite(SPEED_LEFT, 0);
  analogWrite(SPEED_RIGHT, 0);

}

void right(){
	digitalWrite(MOTOR_LEFT, ROTATE_COUNTER_CLOCKWISE);
	digitalWrite(MOTOR_RIGHT, ROTATE_COUNTER_CLOCKWISE);
	analogWrite(SPEED_LEFT, TURN_SPEED);
	analogWrite(SPEED_RIGHT, 0);


}

void left(){
	digitalWrite(MOTOR_LEFT, ROTATE_CLOCKWISE);
	digitalWrite(MOTOR_RIGHT, ROTATE_CLOCKWISE);
	analogWrite(SPEED_LEFT, 0);
	analogWrite(SPEED_RIGHT, TURN_SPEED);
	display_set_digits(1, 0);
	while(get_line_sensor() != 0b11011);

}
void uturn(){
	digitalWrite(MOTOR_LEFT, ROTATE_COUNTER_CLOCKWISE);
	digitalWrite(MOTOR_RIGHT, ROTATE_COUNTER_CLOCKWISE);
	analogWrite(SPEED_LEFT, TURN_SPEED * 1.5);
	analogWrite(SPEED_RIGHT, TURN_SPEED * 1.5);
}

  
bool check_finish(){
	stop();
	forward();
	delay(150); 
	stop();
	delay(500);
	if(get_line_sensor() != VALUE_POSSIBLE_FINISH){
		backward();
		delay(280); 
		stop();
		right();
		delay(1000);
		return false;
	}
	state = FINISH_BLINK;
	stop();
	return true;
	//einde doolhof
}

/*
* State gerelateerde functions
*/

void state_startup_count(){
	static uint8_t countdown_val = 10;
	if(countdown_val == 10){
		display_set_digits(1, 0);
		countdown_time = millis();
		countdown_val--;
	}
	if(millis() - countdown_time < 1000){
		return;

	}
	if(countdown_val == 0){
		//display_set_digits(0, 0);
		display_set_letters('S', 't');
		state = PRE_DRIVING;
	}else{
		display_set_digits(countdown_val, 0);
	}
	countdown_val--;
	countdown_time = millis();
}

void state_pre_driving(){
	if(millis() - countdown_time >= 1000){
		state = DRIVING;
		start_time_driving = millis();
	}

}

void state_driving(){
	uint32_t distance = ping_distance();
	uint8_t line = get_line_sensor();
	display_line_num(line);
	if((distance > 0 && distance <= 16) || line == VALUE_UTURN){
		uturn();
	}else if(line == VALUE_POSSIBLE_FINISH){
		if(check_finish()){
			state = FINISH_BLINK;
			stop();
			return;

		}
	}else if(in_array(VALUES_RIGHT, line, SIZEOF_ARRAY(VALUES_RIGHT))){
		right();

	}else if(in_array(VALUES_FORWARD, line, SIZEOF_ARRAY(VALUES_FORWARD))){
		forward();

	}else  if(in_array(VALUES_LEFT, line, SIZEOF_ARRAY(VALUES_LEFT))){
		left();
	}
}

void state_finish_blinking(){
	if(millis() - countdown_time >= 1000){
		if(finish_blink_remaining % 2 == 0){
			//TODO: display clearen.
			if(finish_blink_remaining == 0){
				state = FINISH;
			}
		}else{
			display_show_drive_time();
		}
		finish_blink_remaining--;
		countdown_time = millis();

	}
}
void state_finish(){
	stop();
	if(millis() - countdown_time >= 1000){
		display_set_letters('F', 'I');
	}
}


void setup() {
	linsensor_init();
	//segment_display_init();
	motors_init();
	state = PRE_DRIVING;
}


void loop() {

	switch(state){
		case STARTUP_COUNTDOWN:
			state_startup_count();
			break;
		case PRE_DRIVING:
			state_pre_driving();
			break;
		case DRIVING:
			state_driving();
			break;
		case FINISH_BLINK:
			state_finish_blinking();
			break;
		case FINISH:
			state_finish();
			break;
		default: //tijdelijk om display te testen.
			break;
	}
	
}
