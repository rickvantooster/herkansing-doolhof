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
bool* display_1;
bool* display_2;
int current_display, display_timer;
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
const bool SEGMENT_LETTERS[6][7] = {
	//a,b,c,d,e,f,g
	{1,0,0,0,1,1,1}, //F
	{0,0,0,0,1,1,0}, //I
	{1,0,1,1,0,1,1}, //S
	{0,0,0,1,1,1,1}, //t
	{1,1,0,1,0,1,0}, //m
  {0,0,0,0,0,0,0}  // off
};


void display_show(bool* segments){
	//nog implementeren.
	digitalWrite(SEGMENT_U1, LOW);
	//display_clear();
	if(segments != NULL){
		for(size_t i = 0; i < 7; i++){
			digitalWrite(SEGMENT_PINS[i], segments[i]);
		}
	}
}

void display_control(){
  if(millis() >= display_timer + 5){
    if(current_display == 1){
      digitalWrite(SEGMENT_U1, LOW);
      digitalWrite(SEGMENT_U2, HIGH);
      display_show(display_2);
      current_display = 2;
      display_timer = millis();
    }else{
      if(current_display == 2){
        digitalWrite(SEGMENT_U1, HIGH);
        digitalWrite(SEGMENT_U2, LOW);
        display_show(display_1);
        current_display = 1;
        display_timer = millis();
      }
    }
  }
}



void display_clear(){
	display_set_letters('O', 'O');

}

void segment_display_init(){
	pinMode(SEGMENT_U1, OUTPUT);
	pinMode(SEGMENT_U2, OUTPUT);
  display_timer = millis();
  current_display = 1;
	for(size_t i = 0; i < SIZEOF_ARRAY(SEGMENT_PINS); i++){
		pinMode(SEGMENT_PINS[i], OUTPUT);

	}
}

void display_set_digits(uint8_t digit1, uint8_t digit2){
	display_1 = SEGMENT_NUMERIC[digit1];
	display_2 = SEGMENT_NUMERIC[digit2];
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
    case 'O':
      result = 5;
      break;
	};
	return result;

}

void display_set_letters(char char1, char char2){
	display_1 = SEGMENT_LETTERS[display_get_letter_idx(char1)];
	display_2 = SEGMENT_LETTERS[display_get_letter_idx(char2)];
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
		display_1 = SEGMENT_NUMERIC[temp_time % 10];
		display_2 = SEGMENT_LETTERS[display_get_letter_idx('m')];
	}else{
		display_2 = SEGMENT_NUMERIC[temp_time % 10];
		temp_time /= 10;
		display_1 = SEGMENT_NUMERIC[temp_time % 10];
	}
}

void display_line_num(uint8_t value){
	if(digit_count(value) == 1){
		display_set_digits(value, 0);
	}else{
		uint8_t temp_value = value;
		display_2 = SEGMENT_NUMERIC[temp_value % 10];
		temp_value /= 10;
		display_1 = SEGMENT_NUMERIC[temp_value % 10];
	}
}
