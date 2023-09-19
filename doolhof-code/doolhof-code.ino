/*
helper macros
*/

//macro om in_array function makkelijker te maken.
#define ARRAY_SIZEOF(x) sizeof(x)/sizeof(x[0])


/*
* pins voor peripherals.
*/

const int LINE_SENSOR_PINS[5] = {0};


/*
* waardes voor de lijn sensoren gegroepeerd op actie.
*/

const uint8_t VALUES_FORWARD[3] = {
	0b11011,
	0b01011,
	0b11010
};
const uint8_t VALUES_LEFT[5] = {
	0b00001,
	0b00010,
	0b00011,
	0b10010,
	0b10011,
};

const uint8_t VALUES_RIGHT[6] = {
	0b10001,
	0b01000,
	0b10000,
	0b11000,
	0b11001,
	0b01001
};


/*
* overige variabelen.
*/
uint64_t time_since_start = 0;

/*
* utility functions
*/

bool in_array(uint8_t arr[], uint8_t value, size_t n){
	for(size_t i = 0; i < n; i++){
		if(arr[i] == value)
			return true;
	}

	return false;
}


void setup() {
}

void loop() {
}
