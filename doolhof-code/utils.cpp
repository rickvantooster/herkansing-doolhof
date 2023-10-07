#include <Arduino.h> 
#include "utils.h"
bool in_array(uint8_t* arr, uint8_t value, size_t n){
	/*
	size_t pivot = n / 2;
	size_t i = 0;
	size_t end = n;

	if(arr[pivot] == value){
		return true;
	}

	if(value < arr[pivot]){
		end = pivot;
	}else{
		i = pivot;

	}


	for(i = 0; i < end; i++){
		if(arr[i] == value)
			return true;
	}
	*/
	for(size_t i = 0; i < n; i++){
		if(arr[i] == value){
			return true;

		}

	}

	return false;
}
