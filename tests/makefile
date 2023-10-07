all: build upload

build:
	~/local/bin/arduino-cli compile --fqbn arduino:avr:uno motor_test

upload:
	~/local/bin/arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno motor_test
console:
	 minicom -b 9600 -C output.log -D /dev/ttyACM0

