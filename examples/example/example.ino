#include "GMs_Button_Palooza.h"

uint32_t resistances[3] = {1000,2000,5000};	//Resistances used by each button

GM::ButtonBunch buttons{A0, 3, 1000, resistances};	//declare the buttons with the resistances from above

void setup()
{
    Serial.begin(115200);
}

void loop()
{
	GM::ButtonStates buttonStates = buttons.getButtonStates();	//Nab those button values from the button bunch

	for (uint16_t i = 0; i < buttonStates.length; i++){		//for each button
		Serial.print(String(buttonStates[i]) + " | ");		//print out the button state
	}
	Serial.println(analogRead(A0));	//now print out the mapped voltage
}
