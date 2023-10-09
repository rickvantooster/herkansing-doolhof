const bool ROTATE_CLOCKWISE = false;
const bool ROTATE_COUNTER_CLOCKWISE = true;

const int SPEED = 40;

const uint32_t MOTOR_DIRECTION_LEFT = 12;
const uint32_t MOTOR_DIRECTION_RIGHT = 13;

const uint32_t SPEED_LEFT = 3;
const uint32_t SPEED_RIGHT = 11;

const uint32_t SEGMENT_U2 = 9;
const uint32_t SEGMENT_U1 = 10;
const uint32_t SEGMENT_PINS[] = {1, 2, 4, 5, 6, 7, 8};

#define SIZEOF_ARRAY(x) sizeof(x)/sizeof(x[0])

const bool motor_only = false;

// 7-segment waardes voor alle cijfers.
const bool SEGMENT_NUMERIC[10][7] = {
	//a,b,c,d,e,f,g
	{1,1,1,1,1,1,0}, // 0
	{0,1,1,0,0,0,0}, // 1
	{1,1,0,1,1,0,1}, // 2
	{1,1,1,1,0,0,1}, // 3
	{0,1,1,0,0,1,1}, // 4
	{1,0,1,1,0,1,1}, // 5
	{1,0,1,1,1,1,1}, // 6
	{1,1,1,0,0,0,0}, // 7
	{1,1,1,1,1,1,1}, // 8
	{1,1,1,1,0,1,1}  // 9
};

const bool SEGMENT_LETTERS[10][7] = {
	//a,b,c,d,e,f,g
	{1,0,0,0,1,1,1}, // F
	{0,0,0,0,1,0,1}, // R
	{0,0,1,1,1,1,1}, // B
	{0,0,0,1,1,1,0}, // L
};
void motors_init(){
	TCCR2B = TCCR2B & B11111000 | B00000111;
	pinMode(MOTOR_DIRECTION_LEFT, OUTPUT);
	pinMode(MOTOR_DIRECTION_RIGHT, OUTPUT);

}

void segment_display_init(){
	pinMode(SEGMENT_U1, OUTPUT);
	pinMode(SEGMENT_U2, OUTPUT);
	for(size_t i = 0; i < SIZEOF_ARRAY(SEGMENT_PINS); i++){
		pinMode(SEGMENT_PINS[i], OUTPUT);

	}
}

void display_show(const bool* segments1, const bool* segments2){
	//nog implementeren.
	digitalWrite(SEGMENT_U1, LOW);
	//display_clear();
	if(segments1 != NULL){
		for(size_t i = 0; i < 7; i++){
			digitalWrite(SEGMENT_PINS[i], segments1[i]);

		}

	}
}

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

bool in_array(uint8_t* arr, uint8_t value, size_t n){
	for(size_t i = 0; i < n; i++){
		if(arr[i] == value){
			return true;

		}

	}
	return false;

}

const uint8_t VALUE_POSSIBLE_FINISH = 0b00000;


const uint32_t LINE_SENSOR_PINS[5] = {A2, A3, A4, A5, 0};
const uint32_t PING_ECHO = A1;
const uint32_t PING_TRIGGER = A0;

void linsensor_init(){
	for(size_t i = 0; i < 5; i++){
		pinMode(LINE_SENSOR_PINS[i], INPUT);
	}

}

uint8_t get_line_sensor(){
	uint8_t result = 0;
	uint8_t idx = 4;
	for(size_t i = 0; i < 5; i++){
		result |= ((uint8_t)digitalRead(LINE_SENSOR_PINS[i]) << idx);
		idx--;

	}
	//Serial.print("[Line sensor] value = ");
	//Serial.println(result);
	return result;

}

uint8_t get_center_line(){
	return digitalRead(LINE_SENSOR_PINS[2]);
}

void forward(){
  digitalWrite(MOTOR_DIRECTION_LEFT, ROTATE_CLOCKWISE);
  digitalWrite(MOTOR_DIRECTION_RIGHT, ROTATE_COUNTER_CLOCKWISE);
  analogWrite(SPEED_LEFT, SPEED);
  analogWrite(SPEED_RIGHT, SPEED);
}

void backward(){
  digitalWrite(MOTOR_DIRECTION_LEFT, ROTATE_COUNTER_CLOCKWISE);
  digitalWrite(MOTOR_DIRECTION_RIGHT, ROTATE_CLOCKWISE);
  analogWrite(SPEED_LEFT, SPEED);
  analogWrite(SPEED_RIGHT, SPEED);
}

void right(){
  digitalWrite(MOTOR_DIRECTION_LEFT, ROTATE_COUNTER_CLOCKWISE);
  digitalWrite(MOTOR_DIRECTION_RIGHT, ROTATE_COUNTER_CLOCKWISE);
  analogWrite(SPEED_LEFT, SPEED);
  analogWrite(SPEED_RIGHT, 0);
}

void left(){
  digitalWrite(MOTOR_DIRECTION_LEFT, ROTATE_CLOCKWISE);
  digitalWrite(MOTOR_DIRECTION_RIGHT, ROTATE_CLOCKWISE);
  analogWrite(SPEED_LEFT, 0);
  analogWrite(SPEED_RIGHT, SPEED);
}

void stop(){
  analogWrite(SPEED_LEFT, 0);
  analogWrite(SPEED_RIGHT, 0);

}

void turn_right(){
	uint8_t line = get_center_line();
	while(line != 0){
		right();
		line = get_center_line();

	}


}

enum phases{
	FORWARD,
	RIGHT,
	LEFT,
	BACKWARD,
	DONE

};

uint8_t phase = FORWARD;
uint32_t turn_start = 0;
const uint32_t TURN_DURATION = 2000;

void setup() {
	motors_init();
	//segment_display_init();
	linsensor_init();

}

void loop() {

	//right();

	uint8_t line = get_line_sensor();
	if(line == VALUE_POSSIBLE_FINISH){
		forward();

	}else if(in_array(VALUES_RIGHT, line, SIZEOF_ARRAY(VALUES_RIGHT))){
		right();
		//right();

	}else if(in_array(VALUES_FORWARD, line, SIZEOF_ARRAY(VALUES_FORWARD))){
		forward();

	}else if(in_array(VALUES_LEFT, line, SIZEOF_ARRAY(VALUES_LEFT))){
		left();

	}else{
		stop();

	}


	/*
	if(phase != DONE){
		display_show(SEGMENT_LETTERS[phase], NULL);
	}else{
		display_show(SEGMENT_LETTERS[0], NULL);
	}
	if(phase == FORWARD){
		forward();
		Serial.println("forward");
		phase = RIGHT;
		delay(1500);
		turn_start = millis();
	}else if(phase == RIGHT){
		Serial.println("RIGHT");
		right();
		if((millis() - turn_start) >= TURN_DURATION){
			phase = DONE;

		}
	}else if(phase == BACKWARD){
		Serial.println("BACK");
		backward();
		phase = LEFT;
		delay(1500);
		turn_start = millis();
	}else if(phase == LEFT){
		Serial.println("LEFT");
		left();
		if((millis() - turn_start) >= TURN_DURATION){
			phase = DONE;

		}

	}else if(phase == DONE){
		stop();

	}
	*/


}
