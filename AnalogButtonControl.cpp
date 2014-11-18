#include <analogButtonControl.h>
#include <Arduino.h>

AnalogButtonControl::AnalogButtonControl(int pin) 
: m_pin(pin), m_lastButton(0), m_analogValue(0) {}

void AnalogButtonControl::addButton(int value, int button_id) {
	if (this->m_buttonCnt == 0) {
		this->m_buttons = (int**) malloc(1*sizeof(int*));
	} else {
		this->m_buttons = (int**) realloc(this->m_buttons, (2*sizeof(int*)));
	}

	this->m_buttons[this->m_buttonCnt] = (int*)malloc(2*sizeof(int));
	this->m_buttons[this->m_buttonCnt][this->INDEX_BTN_VALUE] = value;
	this->m_buttons[this->m_buttonCnt][this->INDEX_BTN_ID] = button_id;
	this->m_buttonCnt++;
}

bool AnalogButtonControl::isPulldown(int button_id) {
	return (this->getPressedButton() == button_id);
}

bool AnalogButtonControl::isPullup(int button_id) {
	if (this->m_lastButton == this->BUTTON_NONE) {
		int button = this->getPressedButton();
		if (button != this->BUTTON_NONE && button == button_id) {
			this->m_lastButton = button;
		}
		return false;
	} else if (
			this->m_lastButton == button_id && 
			this->getPressedButton() == this->BUTTON_NONE
		) {

		this->m_lastButton = this->BUTTON_NONE;
		return true;
	}

	return false;
}

bool AnalogButtonControl::isPullup(int button_id, unsigned long wait) {
	if (this->m_lastButton == this->BUTTON_NONE) {
		int button = this->getPressedButton();
		if (button != this->BUTTON_NONE && button == button_id) {
			this->m_lastButton = button;
		}
		return false;
	} else if (
			this->m_lastButton == button_id && 
			this->getPressedButton() == this->BUTTON_NONE &&
			this->m_time_duration >= wait
		) {

		this->m_time_duration == 0;
		this->m_lastButton = this->BUTTON_NONE;
		return true;
	} 

	return false;
}

int AnalogButtonControl::getAnalogValue() {
	return analogRead(this->m_pin); 
}


int AnalogButtonControl::getPressedButton() {
  int button = 0;
  int value = 0;

  value = this->getAnalogValue();
  for (int i = 0; i < this->m_buttonCnt; i++) {
    if (value > this->m_buttons[i][this->INDEX_BTN_VALUE]) {
      button = this->m_buttons[i][this->INDEX_BTN_ID];
      break;
    }
  }

  if (button == this->BUTTON_NONE) {
  	if (this->m_time_start > 0) {
   		this->m_time_duration = millis() - this->m_time_start;
   		this->m_time_start = 0;
  	}
  	return this->BUTTON_NONE;
  }

  delay(10);

  value = this->getAnalogValue();
  for (int i = 0; i < this->m_buttonCnt; i++) {
    if (value > this->m_buttons[i][this->INDEX_BTN_VALUE]) {
      if (button != this->m_buttons[i][this->INDEX_BTN_ID])  {
      	button = this->BUTTON_NONE;
      }
      break;
    }
  }

  if (this->m_time_start == 0 && button != this->BUTTON_NONE)
  	this->m_time_start = millis();

  this->m_lastButton = button;
  
  return button;
}
