#include "GMs_Button_Palooza.h"

GM::Button button{9};	//declare a button at pin 13

uint32_t resistances[3] = {1000,2000,5000};	//Resistances used by each button
GM::ButtonBunch buttons{A0, 3, 1000, resistances};	//declare the buttons at pin A0 with the resistances from above



void setup()
{
    Serial.begin(115200);
}

void loop()
{
	GM::ButtonState* const buttonStates = buttons.getButtonStates();	//Nab those button values from the button bunch
	GM::ButtonState& buttonState = button.getButtonState();

	Serial.print("Button Bunch              ");

	String voltageStr = String(analogRead(A0));		//for formatting the serial output
	uint8_t len = voltageStr.length();

	for (uint8_t i = 0; i < len; i++) Serial.print(" ");	//Aligns "Button" with the button vals

	Serial.println("| Button");

	for (uint16_t i = 0; i < 3; i++){		//for each button
		Serial.print(" " + String(buttonStates[i].getButton()) + "," +	//print out the button state
		 			 String(buttonStates[i].getButtonDown()) + "," +
					 String(buttonStates[i].getButtonUp()) + " -");		
	}
	Serial.println(" " + voltageStr + " |  " + 	//now print out the mapped voltage
			       String(buttonState.getButton()) + "," + 
				   String(buttonState.getButtonDown()) + "," + 
				   String(buttonState.getButtonUp()));	//and the lone button state

	delay(100);
}
