#include <Arduino.h> 
#include "segment_display.h"
#include "state.h"
#include "utils.h"

const uint32_t SEGMENT_U2 = 9;
const uint32_t SEGMENT_U1 = 10;
const uint32_t SEGMENT_PINS[] = {1, 2, 4, 5, 6, 7, 8};
extern uint8_t state;
extern uint32_t start_time_driving;
extern uint32_t finish_time;

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

// 7-segment waardes voor de te gebruiken letters.
const bool SEGMENT_LETTERS[5][7] = {
	//a,b,c,d,e,f,g
	{1,0,0,0,1,1,1}, //F
	{0,0,0,0,1,1,0}, //I
	{1,0,1,1,0,1,1}, //S
	{0,0,0,1,1,1,1}, //t
	{1,1,0,1,0,1,0} // m

};


void display_show(const bool* segments1, const bool* segments2){
	//nog implementeren.
	digitalWrite(SEGMENT_U1, LOW);
	//display_clear();
	if(segments1 != NULL){
		for(size_t i = 0; i < 7; i++){
			digitalWrite(SEGMENT_PINS[i], segments1[i]);

		}

	}
	if(segments2 != NULL){
		for(size_t i = 0; i < 7; i++){
			digitalWrite(SEGMENT_PINS[i], segments1[i]);

		}

	}
}



void display_clear(){
	display_show(NULL, NULL);

}

void segment_display_init(){
	pinMode(SEGMENT_U1, OUTPUT);
	pinMode(SEGMENT_U2, OUTPUT);
	for(size_t i = 0; i < SIZEOF_ARRAY(SEGMENT_PINS); i++){
		pinMode(SEGMENT_PINS[i], OUTPUT);

	}
}

void display_set_digits(uint8_t digit1, uint8_t digit2){
	const bool* display1_values = SEGMENT_NUMERIC[digit1];
	const bool* display2_values = SEGMENT_LETTERS[digit2];
	//display_show(display1_values, display2_values);
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
	//display_show(display1_values, display2_values);
}




uint8_t digit_count(uint32_t input){
	uint32_t count = 0;
	uint32_t n = input;
	while(n != 0){
		n /= 10;
		count++;

	}

	return count;

}
void display_show_drive_time(){
	if(state == DRIVING){
		uint32_t delta_t = (millis() - start_time_driving) / 1000;
		finish_time = delta_t;

	}
	uint32_t temp_time = finish_time;
	uint8_t digits = digit_count(finish_time);
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
