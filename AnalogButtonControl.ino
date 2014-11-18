#include <AnalogButtonControl.h>

AnalogButtonControl control = AnalogButtonControl(0);

#define BUTTON_BLUE 1
#define BUTTON_RED 2
#define BUTTON_GREEN 3

void setup() {
	Serial.begin(9600);
	control.addButton(650, BUTTON_BLUE);
	control.addButton(490, BUTTON_RED);
	control.addButton(350, BUTTON_GREEN);
}

void loop() {

	if (control.isPullup(BUTTON_BLUE, 2000)) {
		Serial.println("BUTTON_BLUE released after 2 seconds");
	}

	if (control.isPullup(BUTTON_BLUE)) {
		Serial.println("BUTTON_BLUE");
	}

	if (control.isPulldown(BUTTON_RED)) {
		Serial.println("BUTTON_RED");
	}

	if (control.isPulldown(BUTTON_GREEN)) {
		Serial.println("BUTTON_GREEN");
	}

	delay(100);
}