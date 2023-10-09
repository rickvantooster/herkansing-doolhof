#include <Arduino.h>
#include <NewPing.h>

const uint32_t LINE_SENSOR_PINS[5] = {A2, A3, A4, A5, 0};
const uint32_t PING_ECHO = A1;
const uint32_t PING_TRIGGER = A0;

NewPing sonar(PING_TRIGGER, PING_ECHO, 30);

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

uint32_t ping_distance(){
	uint32_t result = sonar.ping_cm(30); 
	Serial.print("[Ping sensor] distance = ");
	Serial.print(result); 
	Serial.println("cm");

	return result;
}

uint8_t get_middle_value(){
	return (uint8_t)digitalRead(LINE_SENSOR_PINS[2]);

}
uint8_t get_right_value(){
	return (uint8_t)digitalRead(LINE_SENSOR_PINS[4]);

}
uint8_t get_left_value(){
	return (uint8_t)digitalRead(LINE_SENSOR_PINS[0]);

}
