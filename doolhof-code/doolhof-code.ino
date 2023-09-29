/*
helper macros
*/

//macro om in_array function makkelijker te maken.
#define ARRAY_SIZEOF(x) sizeof(x)/sizeof(x[0])


/*
* pins voor peripherals.
*/

const uint32_t LINE_SENSOR_PINS[5] = {0};


/*
* waardes voor de lijn sensoren gegroepeerd op actie.
*/

const uint8_t VALUES_FORWARD[3] = {
	0b01011, // 11
	0b11010, // 26
	0b11011, //16+8+2+1 = 27
};
const uint8_t VALUES_LEFT[5] = {
	0b00001, // 1
	0b00010, // 2
	0b00011, // 3
	0b10010, // 18
	0b10011, // 19
};

const uint8_t VALUES_RIGHT[6] = {
	0b01000, //8
	0b01001, //9
	0b10000, //16
	0b10001, //17
	0b11000, //24
	0b11001, //25
};

const uint8_t VALUE_POSSIBLE_FINISH = 0b00000;


/*
* overige variabelen.
*/
uint64_t time_since_start = 0;
int speed = 30; //exact value tbd later

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

/*
* sensor gerelateerde functions.
*/

uint8_t get_line_sensor(){
	uint8_t result = 0;
	uint8_t idx = 4;
	for(size_t i = 0; i < 5; i++){
		result |= ((uint8_t)digitalRead(LINE_SENSOR_PINS[i]) << idx);
		idx--;

	}
	return result;

}

/*
* motor related functions
*/

void forward(){
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  analogWrite(3, speed);
  analogWrite(11, speed);
}

void backward(){
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  analogWrite(3, speed);
  analogWrite(11, speed);
}

void right(){
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  analogWrite(3, 0);
  analogWrite(11, speed);
}

void left(){
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  analogWrite(3, speed);
  analogWrite(11, 0);
}

void uturn(){
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
  analogWrite(3, speed);
  analogWrite(11, speed);
  // een methode om te timen dat hij precies 180 graden draait?
}

void checkFinish(){
  
}

void setup() {
	Serial.begin(9600);
}

void loop() {
	uint8_t line = get_line_sensor();
	if(line == VALUE_POSSIBLE_FINISH){
		checkFinish()
	}else if(in_array(VALUES_RIGHT, line, ARRAY_SIZEOF(VALUES_RIGHT))){
		right();

	}else if(in_array(VALUES_LEFT, line, ARRAY_SIZEOF(VALUES_LEFT))){
		left();

	}else if(in_array(VALUES_FORWARD, line, ARRAY_SIZEOF(VALUES_FORWARD))){
		forward();
		return; // Uitzoeken of dit nodig is.
	}else{
		uturn();
	}
	forward();

	
}
