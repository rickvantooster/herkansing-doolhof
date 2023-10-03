#ifndef SENSORS_H
#define SENSORS_H

void linsensor_init();
uint8_t get_line_sensor();
uint8_t get_middle_value();

uint32_t ping_distance();
#endif
