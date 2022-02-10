#ifndef BUZZER
#define BUZZER

#ifdef COMPILE_CPP_NOT_ARDUINO
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"
#else
#include <Arduino.h>
#endif

// Buzzer class.
//This class is a very heavily modified version of Basic_LED.h class
class buzzer {
protected:
	//pin number
	int out_pin;
	// pitch between 31 and 4000+. I'm not sure, it doesn't matter that much.
	int pitch;
	//Is buzzer enabled?
	bool buzzer_enabled;
	//Is buzzer on?
	bool buzzer_on;

	void setEnabled(boolean enabled) { buzzer_enabled = enabled; }
	void setOn(boolean isOn) { buzzer_on = isOn; }
	void set_pin_on() { analogWrite(out_pin, pitch); }
	void set_pin_off() { analogWrite(out_pin, 0); }

	// initialize variables to default values
	void set_default_vals()
	{
		out_pin = -1;
		setEnabled(false);
		//C by default
		pitch = 261;
	}

public:
	buzzer()
	{
		set_default_vals();
	}
	//
	buzzer(int out_pin_numb)
	{
		set_default_vals();
		setup_buzzer(out_pin_numb);
	}
	buzzer(int out_pin_numb, int pitch)
	{
		set_default_vals();
		setup_buzzer(out_pin_numb);
		set_pitch(pitch);
	}
	//
	void switch_on()
	{
		if (isEnabled() && !isOn())
		{
			// switch LED ON
			tone(out_pin, pitch)
			set_pin_on();
			setOn(true);
		}

	}
	void switch_off()
	{
		if (isEnabled() && isOn())
		{
			noTone(out_pin)
			set_pin_off();
			setOn(false);
		}

	}
	void setup_buzzer(int out_pin_numb)
	{
		// This function is used only once        
		if (isEnabled())
			return;

		if (out_pin_numb >= 0)
		{
			out_pin = out_pin_numb;
			pinMode(out_pin, OUTPUT);
			setEnabled(true);
			//switch off by default
			setOn(true);
			switch_off();
			//
		}
	}
	//
	boolean isEnabled() { return buzzer_enabled; }
	boolean isOn() { return buzzer_on; }



	// set the level of pitch
	void set_pitch(int in_pwm)
	{
		if (isEnabled())
		{
			boolean switch_back_off = false;
			in_pwm = abs(in_pwm);
			// set a  value
			pitch = min(in_pwm, 4000);
			set_pin_on();
			// switch off if needed
			if (switch_back_off)
				switch_off();
		}
	}

	int get_pitch() { return pitch; }
};

#endif