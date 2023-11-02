# AnalogJoyFan
Microcontroller Project - Analog joystick-controlled fan with safety killswitch
Parts used:
	7 Female-Male jumper wires
	22 Male-Male jumper wires
	1 220 Ohm Resistor
	1 Passive Buzzer
	1 Potentiometer
	1 Power Supply Module & 9V Battery
	1 L293D Motor Driver & DC Motor
	1 LCD1602A Module
	1 Tilt Ball Switch
	1 Analog Joystick module
  1 Arduino Uno
  
Function:
	The device is a DC motor fan that can be controlled by the input of an analog joystick. The analog joystick has 3 functions: switch mode, power on/off, and fan speed up/down. These states are continuously updated and output on the LCD display. The passive buzzer chimes when the analog joystick sends input to the Arduino. When in pulse mode, the fan pulses between two predetermined speeds. When in manual mode, the fan speed can be manually changed with the analog joystick. The joystick can power off the fan and so can the tilt ball switch. When the tilt ball switch is in the HIGH position, the device is OFF. When the tilt switch is in the LOW position, the device is ON.

Purpose:
	The purpose of this device is to provide a way to control the DC motor fan manually as well as automatically. It provides a way to control fan speeds and modes and provides a user-friendly graphical output. The passive buzzer provides auditory feedback. The tilt ball switch serves as a safety mechanism. In an industrial setting, one would want a large industrial-sized cooling fan, for example, to shut itself off if it were to fall out of place or fall over. The tilt ball switch provides this functionality, and, in the case of failure of the tilt switch, the analog joystick provides the same functionality.

