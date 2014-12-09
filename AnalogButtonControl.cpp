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
	this->m_isReset = true;
}

bool AnalogButtonControl::isPulldown(int button_id) {
	int button = this->getPressedButton();
	if (!this->m_isReset) {
		return false;
	}

	return (button == button_id);
}

bool AnalogButtonControl::isPullup(int button_id) {
	int button = this->getPressedButton();
	if (this->m_lastButton == this->BUTTON_NONE) {
		if (button != this->BUTTON_NONE && button == button_id) {
			this->m_lastButton = button;
		}
		return false;
	} else if (
			this->m_isReset &&
			this->m_lastButton == button_id && 
			button == this->BUTTON_NONE
		) {
		this->m_isReset = false;
		return true;
	}

	return false;
}

bool AnalogButtonControl::isHold(int button_id, unsigned long wait) {
	int button = this->getPressedButton();
	if (this->m_lastButton == this->BUTTON_NONE) {
		if (button != this->BUTTON_NONE && button == button_id) {
			this->m_lastButton = button;
		}
		return false;
	} else if (
			this->m_isReset &&
			this->m_lastButton == button_id && 
			button == button_id &&
			this->m_time_current_duration >= wait
		) {

		this->m_isReset = false;
		this->m_time_current_duration = 0;
		this->m_time_duration == 0;
		this->m_time_start = 0;
		return true;
	} 

	return false;
}

bool AnalogButtonControl::isPullup(int button_id, unsigned long wait) {
	int button = this->getPressedButton();
	if (this->m_lastButton == this->BUTTON_NONE) {
		if (button != this->BUTTON_NONE && button == button_id) {
			this->m_lastButton = button;
		}
		return false;
	} else if (
			this->m_isReset &&
			this->m_lastButton == button_id && 
			button == this->BUTTON_NONE &&
			this->m_time_duration >= wait
		) {

		this->m_isReset = false;
		this->m_time_current_duration = 0;
		this->m_time_duration == 0;
		this->m_time_start = 0;
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
 
  Serial.println(this->m_isReset);

  if (button == this->BUTTON_NONE) {
  	if (this->m_time_start > 0) {
   		this->m_time_duration = millis() - this->m_time_start;
   		this->m_time_start = 0;
  	}

  	if (!this->m_isReset) {
  		this->m_lastButton = this->BUTTON_NONE;
  		this->m_isReset = true;
  	}
  	return this->BUTTON_NONE;
  }

  if (this->m_time_start > 0) 
  	this->m_time_current_duration = millis() - this->m_time_start;


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

  if (this->m_time_start == 0 && button != this->BUTTON_NONE) {
  	this->m_time_start = millis();
  }

  this->m_lastButton = button;
  
  return button;
}
