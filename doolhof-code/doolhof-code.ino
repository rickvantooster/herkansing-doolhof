#include <NewPing.h>
/*
helper macros
*/

#define SIZEOF_ARRAY(x) sizeof(x)/sizeof(x[0])

/*
* constants om bepaalde acties vertduidelijken.
*/

const bool ROTATE_CLOCKWISE = false;
const bool ROTATE_COUNTER_CLOCKWISE = true;

/*
* status gerelateerde waardes.
*/
enum robot_state{
	STARTUP_COUNTDOWN,
	PRE_DRIVING,
	DRIVING,
	FINISH_BLINK,
	FINISH

};

/*
* pins voor peripherals.
*/

const uint32_t LINE_SENSOR_PINS[5] = {A2, A3, A4, A5, 0};
const uint32_t PING_ECHO = A1;
const uint32_t PING_TRIGGER = A0;

const uint32_t SEGMENT_U2 = 9;
const uint32_t SEGMENT_U1 = 10;

const uint32_t SEGMENT_PINS[] = {1, 2, 4, 5, 6, 7, 8};

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

// 7-segment waardes voor alle cijfers.
const bool SEGMENT_NUMERIC[10][8] = {
	{1,1,1,1,1,1,0},
	{0,1,1,0,0,0,0},
	{1,1,0,1,1,0,1},
	{1,1,1,1,0,0,1},
	{0,1,1,0,0,1,1},
	{1,0,1,1,0,1,1},
	{1,0,1,1,1,1,1},
	{1,1,1,0,0,0,0},
	{1,1,1,1,1,1,1},
	{1,1,1,1,0,0,1}
};

// 7-segment waardes voor de te gebruiken letters.
const bool SEGMENT_LETTERS[5][8] = {
	//a,b,c,d,e,f,g
	{1,0,0,0,1,1,1}, //F
	{0,0,0,0,1,1,0}, //I
	{1,0,1,1,0,1,1}, //S
	{0,0,0,1,1,1,1}, //t
	{1,1,0,1,0,1,0} // m

};

/*
* overige variabelen.
*/

uint64_t countdown_time = 0;
int speed = 40; //exact value tbd later
NewPing sonar(PING_TRIGGER, PING_ECHO, 30);
uint8_t last_found_line = 0;
uint8_t state = STARTUP_COUNTDOWN;
uint8_t countdown_step = 10;
uint32_t start_time_driving = 0;
bool display_on = true;
uint8_t finish_blink_remaining = 6;
uint32_t time_to_finish = 0;

/*
* utility functions
*/

bool in_array(uint8_t* arr, uint8_t value, size_t n){
	size_t pivot = n / 2;
	size_t i = 0;
	size_t end = n;

	if(arr[pivot] == value){
		return true;
	}

	if(value < arr[pivot]){
		end = pivot;
	}else{
		i = pivot;

	}


	for(i = 0; i < end; i++){
		if(arr[i] == value)
			return true;
	}

	return false;
}

uint32_t digit_count(uint32_t input){
	uint32_t count = 0;
	uint32_t n = input;
	while(n != 0){
		n /= 10;
		count++;

	}

	return count;

}

/*
* Pin setup functions.
*/

void linsensor_init(){
	for(size_t i = 0; i < 5; i++){
		pinMode(LINE_SENSOR_PINS[i], INPUT);
	}

}

void segment_display_init(){
	pinMode(SEGMENT_U1, OUTPUT);
	pinMode(SEGMENT_U2, OUTPUT);
	for(size_t i = 0; i < SIZEOF_ARRAY(SEGMENT_PINS); i++){
		pinMode(SEGMENT_PINS[i], OUTPUT);

	}
}

void motors_init(){
	TCCR2B = TCCR2B & B11111000 | B00000111; // for PWM frequency of 30.64 Hz
	pinMode(MOTOR_LEFT, OUTPUT);
	pinMode(MOTOR_RIGHT, OUTPUT);
	//pinMode(3, OUTPUT);
	//pinMode(11, OUTPUT);
}


/*
* sensor gerelateerde functions.
*/


uint8_t get_line_sensor(){
	uint8_t result = 0;
	uint8_t idx = 4;
	Serial.print("[Line sensor] value = ");
	for(size_t i = 0; i < 5; i++){
		result |= ((uint8_t)digitalRead(LINE_SENSOR_PINS[i]) << idx);
		idx--;
		Serial.print(digitalRead(LINE_SENSOR_PINS[i]));

	}
	Serial.println();
	//Serial.print("[Line sensor] value = ");
	//Serial.println(result);
	return result;

}

uint32_t ping_distance(){
	uint32_t result = sonar.ping_cm(30); 
	Serial.print("[Ping sensor] distance = ");
	Serial.print(result); 
	Serial.println("cm");

	return result;
}

/*
* 7-segment gerelateerde functions
*/


void display_set_digits(uint8_t digit1, uint8_t digit2){
	const bool* display1_values = SEGMENT_NUMERIC[digit1];
	const bool* display2_values = SEGMENT_LETTERS[digit2];
	display_show(display1_values, display2_values);
}

uint8_t display_get_letter_idx(char c){
	uint8_t result = 0;
	switch(c) {
		case 'F':
			result = 0;
			break;
		case 'I':
			result = 1;
			break;

		case 'S':
			result = 2;
			break;
		case 't':
			result = 3;
			break;
		case 'm':
			result = 4;
			break;
	};
	return result;

}

void display_set_letters(char char1, char char2){
	const bool* display1_values = SEGMENT_LETTERS[display_get_letter_idx(char1)];
	const bool* display2_values = SEGMENT_LETTERS[display_get_letter_idx(char2)];
	display_show(display1_values, display2_values);
}

void display_show(const bool* segments1, const bool* segments2){
	//nog implementeren.
}

void display_show_drive_time(){
	if(state == DRIVING){
		uint32_t delta_t = (millis() - start_time_driving) / 1000;
		time_to_finish = delta_t;

	}
	uint32_t temp_time = time_to_finish;
	uint8_t digits = digit_count(time_to_finish);
	//Wanneer er meer dan 99 seconden zijn tonen we het aantal minuten.
	if(digits > 2){
		temp_time /= 60;
		const bool* display1_values = SEGMENT_NUMERIC[temp_time % 10];
		const bool* display2_values = SEGMENT_LETTERS[display_get_letter_idx('m')];
		display_show(display1_values, display2_values);
	}else{
		const bool* display2_values = SEGMENT_NUMERIC[temp_time % 10];
		temp_time /= 10;
		const bool* display1_values = SEGMENT_NUMERIC[temp_time % 10];
		display_show(display1_values, display2_values);
	}


}


/*
* motor related functions
*/

void forward(){
  digitalWrite(MOTOR_LEFT, ROTATE_CLOCKWISE);
  digitalWrite(MOTOR_RIGHT, ROTATE_COUNTER_CLOCKWISE);
  analogWrite(SPEED_LEFT, speed);
  analogWrite(SPEED_RIGHT, speed);
}

void backward(){
  digitalWrite(MOTOR_LEFT, ROTATE_COUNTER_CLOCKWISE);
  digitalWrite(MOTOR_RIGHT, ROTATE_CLOCKWISE);
  analogWrite(SPEED_LEFT, speed);
  analogWrite(SPEED_RIGHT, speed);
}

void right(){
  digitalWrite(MOTOR_LEFT, ROTATE_CLOCKWISE);
  digitalWrite(MOTOR_RIGHT, ROTATE_COUNTER_CLOCKWISE);
  analogWrite(SPEED_LEFT, 0);
  analogWrite(SPEED_RIGHT, speed);
}

void left(){
  digitalWrite(MOTOR_LEFT, ROTATE_COUNTER_CLOCKWISE);
  digitalWrite(MOTOR_RIGHT, ROTATE_CLOCKWISE);
  analogWrite(SPEED_LEFT, speed);
  analogWrite(SPEED_RIGHT, 0);
}

void uturn(){
  digitalWrite(MOTOR_LEFT, HIGH);
  digitalWrite(MOTOR_RIGHT, LOW);
  analogWrite(SPEED_LEFT, speed);
  analogWrite(SPEED_RIGHT, speed);
  // een methode om te timen dat hij precies 180 graden draait?
  //Rick: mogelijk te doen door te kijken totdat we de laatst gedetecteerde lijn hebben gevonden?
}
  
void check_finish(){
	forward();
	if(get_line_sensor() != VALUE_POSSIBLE_FINISH){
		backward();
		return;
	}
	state = FINISH_BLINK;
	//einde doolhof
}

/*
* State gerelateerde functions
*/

void state_startup_count(){
	if(millis() - countdown_time >= 1000){
		if(countdown_step == 10){
			display_set_digits(1, 0);
		}else if(countdown_step == 0){
			display_set_letters('S', 't');
			state = PRE_DRIVING;
		}else{
			display_set_digits(0, countdown_step);
		}
		countdown_step--;
		countdown_time = millis();
	}
}

void state_driving(){
	uint32_t distance = ping_distance();
	uint8_t line = get_line_sensor();
	display_show_drive_time();
	if(distance > 0 && 8 <= distance){
		uturn();

	}else if(line == VALUE_POSSIBLE_FINISH){
		check_finish();
	}else if(in_array(VALUES_RIGHT, line, SIZEOF_ARRAY(VALUES_RIGHT))){
		right();

	}else if(in_array(VALUES_FORWARD, line, SIZEOF_ARRAY(VALUES_FORWARD))){
		forward();
		return; // Uitzoeken of dit nodig is.

	}else if(in_array(VALUES_LEFT, line, SIZEOF_ARRAY(VALUES_LEFT))){
		left();
	}else{
		uturn();
	}
	forward();

}

void state_finish(){
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


void setup() {
	Serial.begin(9600);
	linsensor_init();
	segment_display_init();
	motors_init();
	state = STARTUP_COUNTDOWN;
}

void loop() {
	if (state == STARTUP_COUNTDOWN) {
		state_startup_count();
	}else if(state == PRE_DRIVING && millis() - countdown_time >= 1000){
		state = DRIVING;
		start_time_driving = millis();
	}else if(state == DRIVING){
		state_driving();
	}else if(state == FINISH){
		state_finish();
	}else if(state == FINISH){
		if(millis() - countdown_time >= 1000){
			display_set_letters('F', 'I');
		}
	}

	
}
