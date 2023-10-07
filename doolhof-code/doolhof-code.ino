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
const int SPEED = 45; //exact value tbd later
const int SPEED_OPOSITE = 30; //exact value tbd later

enum cached_direction{
	FORWARD,
	RIGHT,
	LEFT,

};

/*
* pins voor motors.
*/



const uint32_t MOTOR_LEFT = 12;
const uint32_t MOTOR_RIGHT = 13;

const uint32_t SPEED_LEFT = 3;
const uint32_t SPEED_RIGHT = 11;


/*
* waardes voor de lijn sensoren gegroepeerd op actie.
*/

const uint8_t VALUES_FORWARD[] = {
	0b01011, // 11
	0b11010, // 26
	0b11011, //16+8+2+1 = 27
};

const uint8_t VALUES_LEFT[] = {
	0b00001, // 1
	0b00010, // 2
	0b00011, // 3
	0b10010, // 18
	0b10011, // 19
};

const uint8_t VALUES_RIGHT[] = {
	0b01000, //8
	0b01001, //9
	0b10000, //16
	0b10001, //17
	0b11000, //24
	0b11001, //25
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
  digitalWrite(MOTOR_LEFT, ROTATE_CLOCKWISE);
  digitalWrite(MOTOR_RIGHT, ROTATE_COUNTER_CLOCKWISE);
  analogWrite(SPEED_LEFT, SPEED);
  analogWrite(SPEED_RIGHT, SPEED);
}

void backward(){
  digitalWrite(MOTOR_LEFT, ROTATE_COUNTER_CLOCKWISE);
  digitalWrite(MOTOR_RIGHT, ROTATE_CLOCKWISE);
  analogWrite(SPEED_LEFT, SPEED);
  analogWrite(SPEED_RIGHT, SPEED);
}


void right(){
  digitalWrite(MOTOR_LEFT, ROTATE_COUNTER_CLOCKWISE);
  digitalWrite(MOTOR_RIGHT, ROTATE_COUNTER_CLOCKWISE);
  analogWrite(SPEED_LEFT, SPEED);
  analogWrite(SPEED_RIGHT, SPEED);
}

void left(){
  digitalWrite(MOTOR_LEFT, ROTATE_CLOCKWISE);
  digitalWrite(MOTOR_RIGHT, ROTATE_CLOCKWISE);
  analogWrite(SPEED_LEFT, SPEED);
  analogWrite(SPEED_RIGHT, 0);
}
void stop(){
  analogWrite(SPEED_LEFT, 0);
  analogWrite(SPEED_RIGHT, 0);

}

void turn_right(){
	// een methode om te timen dat hij precies 180 graden draait?
	right();
	while(get_middle_value() != 0){
		right();

	}
	stop();

}

void turn_left(){
	// een methode om te timen dat hij precies 180 graden draait?
	left();
	while(get_middle_value() != 0){
		left();

	}
	stop();

}
void uturn(){
	// een methode om te timen dat hij precies 180 graden draait?
	while(get_middle_value() != 0){
		digitalWrite(MOTOR_LEFT, ROTATE_CLOCKWISE);
		digitalWrite(MOTOR_RIGHT, ROTATE_COUNTER_CLOCKWISE);
		analogWrite(SPEED_LEFT, SPEED);
		analogWrite(SPEED_RIGHT, SPEED);

	}

	stop();
}
  
void check_finish(){
	forward();
	analogWrite(SPEED_LEFT, 0);
	analogWrite(SPEED_RIGHT, 0);
	if(get_line_sensor() != VALUE_POSSIBLE_FINISH){
		backward();
		return;
	}
	state = 127;
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
	Serial.print("[countdown] value = ");
	Serial.println(countdown_val);
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
	//display_show_drive_time();
	if(distance > 0 && distance >= 8){
		uturn();
	}else if(in_array(VALUES_RIGHT, line, SIZEOF_ARRAY(VALUES_RIGHT))){
		//display_set_digits(0, 0);
		right();

	}else if(in_array(VALUES_FORWARD, line, SIZEOF_ARRAY(VALUES_FORWARD))){
		//display_set_digits(1, 0);
		forward();

	}else if(line == VALUE_POSSIBLE_FINISH){
		check_finish();
	}else if(in_array(VALUES_RIGHT, line, SIZEOF_ARRAY(VALUES_RIGHT))){
		//display_set_digits(0, 0);
		right();

	}else if(in_array(VALUES_LEFT, line, SIZEOF_ARRAY(VALUES_LEFT))){
		//display_set_digits(2, 0);
		left();
	}else{
		//display_set_digits(3, 0);
		uturn();
	}
	//forward();

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
	if(millis() - countdown_time >= 1000){
		display_set_letters('F', 'I');
	}
}


void setup() {
	//Serial.begin(9600);
	linsensor_init();
	//segment_display_init();
	motors_init();
	state = PRE_DRIVING;
}

void loop() {
	/*
	uint32_t distance = ping_distance();
	uint8_t line = get_line_sensor();
	if(line == VALUE_UTURN){
		stop();
	}else{
		forward();

	}
	*/

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
			Serial.println("[state] non valid state");
			delay(10000);
			break;
	}
	
}
