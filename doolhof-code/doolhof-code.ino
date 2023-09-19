#define ARRAY_SIZEOF(x) sizeof(x)/sizeof(x[0])

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

uint64_t time_since_start = 0;


void setup() {
}

void loop() {
}
