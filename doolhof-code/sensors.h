#ifndef SENSORS_H
#define SENSORS_H

void linsensor_init();
uint8_t get_line_sensor();
uint8_t get_middle_value();
uint8_t get_right_value();
uint8_t get_left_value();

uint32_t ping_distance();
#endif
