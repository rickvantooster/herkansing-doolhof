all: build upload

build:
	~/local/bin/arduino-cli compile --optimize-for-debug -v -e --fqbn arduino:avr:uno doolhof-code

upload:
	~/local/bin/arduino-cli upload -p /dev/ttyACM1 --fqbn arduino:avr:uno doolhof-code
