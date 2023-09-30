#define array_sizeof(x) sizeof(x)/sizeof(x[0])
//const uint8_t pins[] = {A0, A1, A2, A3, A4};
const uint8_t pins[] = {A2};

void setup() {
	for(size_t i = 0; i < array_sizeof(pins); i++){
		pinMode(pins[i], INPUT);
	}
	Serial.begin(9600);
}

void loop() {
	for(size_t i = 0; i < array_sizeof(pins); i++){
		Serial.print(analogRead(pins[i]));
		Serial.print(" ");
	}

	Serial.println();
	for(size_t i = 0; i < array_sizeof(pins); i++){
		Serial.print(digitalRead(pins[i]));
		Serial.print(" ");
	}
	Serial.println();
	delay(2500);
}
