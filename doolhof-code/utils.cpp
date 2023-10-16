#include <Arduino.h> 
#include "utils.h"
bool in_array(uint8_t* arr, uint8_t value, size_t n){
	for(size_t i = 0; i < n; i++){
		if(arr[i] == value){
			return true;

		}

	}

	return false;
}
