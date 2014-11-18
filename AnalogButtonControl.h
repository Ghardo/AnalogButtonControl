
class AnalogButtonControl {
	public:
		AnalogButtonControl(int pin);
		void addButton(int value, int button_id);
		bool isPulldown(int button_id);
		bool isPullup(int button_id);
		bool isPullup(int button_id, unsigned long wait);

		static const int INDEX_BTN_VALUE = 0;
		static const int INDEX_BTN_ID = 1;
		static const int BUTTON_NONE = 0;

	protected:
		int getAnalogValue();
		int getPressedButton();
		void updateLastButton();
		int m_analogValue;
		int m_pin;
		int m_lastButton;
		int **m_buttons;
		int m_buttonCnt;
		unsigned long m_time_start;
		unsigned long m_time_duration;
};