#include "GMs_Button_Palooza.h"

/*****************
 * BUTTON_STATE
*****************/

void GM::ButtonState::operator=(bool newValue){
    buttonState <<= 1;          //Shimmy the var over
    buttonState |= newValue;    //and insert the new value
}
void GM::ButtonState::operator=(GM::ButtonState newValue){
    buttonState <<= 1;                      //Shimmy the var over
    buttonState |= newValue.getButton();    //and insert the new value
}
bool GM::ButtonState::getButton(){
    return buttonState&1;       //Read the first bit in the variable
}
bool GM::ButtonState::getButtonDown(){
    return (buttonState&1) && !(buttonState>>1&1);  //if first bit on and second bit off
}                                                   //(if is currently on but wasn't)
bool GM::ButtonState::getButtonUp(){
    return !(buttonState&1) && (buttonState>>1&1);  //if first bit off and second bit on
}                                                   //(if is currently not on but was)

/***********
 * BUTTON
***********/

GM::Button::Button(uint16_t pin):   //initialize constant
    pin(pin)
{ pinMode(pin, INPUT_PULLUP); }     //and set pin mode

GM::ButtonState& GM::Button::getButtonState(){  //set value in button state
    static GM::ButtonState buttonState;
    buttonState = !digitalRead(pin); //and return it to call as reference
    return buttonState;
}

/*****************
 * BUTTON_BUNCH
*****************/

GM::ButtonBunch::ButtonBunch(uint16_t pin, uint8_t numberOfButtons, uint32_t groundResistance, const uint32_t* resistances):   //Initilize constants and set pin mode, that's all
    length(numberOfButtons),            pin(pin),   
    numberOfVoltages(pow(2,length)-1),     voltages(voltagesInit(length, groundResistance, resistances)),
    prevButtonStates{new GM::ButtonState[length]}
{ pinMode(pin, INPUT); }

GM::ButtonState* const GM::ButtonBunch::getButtonStates(){
    static GM::ButtonState* const buttonStates = new GM::ButtonState[length]; //Creates static ButtonState class to send

    int16_t voltage = analogRead(pin);             //Get the mapped voltage
    if (voltage){                                   //if you're getting something from this pin
        uint16_t closestIndex;
        int16_t  closestDist=1024;    //declare some handy vars

        for (uint16_t i = 0; i < numberOfVoltages; i++){   //go through all the voltages
            int16_t currentDist = abs(voltages[i] - voltage);    //find the difference between the voltage you're on and the one you're getting
            if (currentDist < closestDist){         //if the difference is less than the previousely closest distance
                closestDist = currentDist;          //set the closest distance to the current distance
                closestIndex=i;                     //and set the closest distance's index value
            }
        }

        if (closestDist > 4)    //If voltage not recognized, set state to previous state (to prevent flicker)
            for (uint8_t i = 0; i < length; i++)
                buttonStates[i] = prevButtonStates[i];
        else                    //Now shimmy through that index value because each bit in it represents a button state
            for (uint8_t i = 0; i < length; i++)
                buttonStates[i] = closestIndex+1>>i&1;
    } else {                                        //if you're getting nothing
        for (uint8_t i = 0; i < length; i++)        //just set all the button states to false
            buttonStates[i] = false;
    }
    for (uint8_t i = 0; i < length; i++)    prevButtonStates[i] = buttonStates[i];

    return buttonStates;
}

/*******************************
 * BUTTON_BUNCH PRIVATE PARTS
*******************************/

const int16_t* const GM::ButtonBunch::voltagesInit(const uint8_t numberOfButtons, uint32_t groundResistance, const uint32_t* resistances){    //Function for initing const voltages

    int16_t* voltageBuff = new int16_t[numberOfVoltages];    //create a buffer for the voltages to be stored in

    for (uint16_t i = 0; i < numberOfVoltages; i++){    //Now go through for each voltage value
        double sum = 0;                                 //handy var (sum of (1/r)s)
        for (uint8_t j = 0; j < numberOfButtons; j++)   //Now go through for each button value
            if (i+1>>j&1) sum += 1.0/resistances[j];    //and if the button's bit is on in the voltage value, add the inverse of the button's resistance to sum (I know this can be done better but if it aint broke...)
                                                        //I don't know what the hell I'm doing so just look here: https://www.khanacademy.org/science/electrical-engineering/ee-circuit-analysis-topic/ee-resistor-circuits/a/ee-parallel-resistors
        double r1 = 1.0/sum, r2 = groundResistance;     //set both resistances
                                                        
                                                    //assume that [V_Pin*R_Buttons = V_Ground*R_Ground] (idk what I'm doing but it works)
        voltageBuff[i] = (r2/(r1+r2))*1024.0;       //Ugh equation derived from the one right there^
    }

    return voltageBuff;
}



