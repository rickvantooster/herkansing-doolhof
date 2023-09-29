all: build upload

build:
	~/local/bin/arduino-cli compile -v --fqbn arduino:avr:uno doolhof-code

upload:
	~/local/bin/arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno doolhof-code
console:
	 minicom -b 9600 -D /dev/ttyACM0

