#ifndef SEGMENT_DISPLAY_H
#define SEGMENT_DISPLAY_H

void segment_display_init();
void display_set_digits(uint8_t digit1, uint8_t digit2);
void display_control();

uint8_t display_get_letter_idx(char c);

void display_set_letters(char char1, char char2);

void display_show(bool* segments);
void display_clear();

uint8_t digit_count(uint32_t input);

void display_show_drive_time();
void display_line_num(uint8_t value);
#endif
