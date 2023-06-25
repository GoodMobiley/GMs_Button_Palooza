#include "GMs_Button_Palooza.h"

/******************
 * BUTTON_STATES
******************/

GM::ButtonStates::ButtonStates(uint8_t numberOfButtons):   //Yay, Initialize constants!
    buttonStates(new bool[numberOfButtons]),
    length(numberOfButtons)                 {}

bool& GM::ButtonStates::operator[](uint16_t index) {return buttonStates[index];}    //Yeah so basically just reference the pointer when using [] operator
void  GM::ButtonStates::operator=(ButtonStates& newButtonStates){
    for (uint8_t i = 0; i < length; i++) buttonStates[i] = newButtonStates[i];      //When setting button states with another button states just change the vals in the bool list
}
void  GM::ButtonStates::operator=(bool newButtonStates){
    for (uint8_t i = 0; i < length; i++) buttonStates[i] = newButtonStates;         //when setting to a bool set all states to said bool
}
void  GM::ButtonStates::operator=(uint16_t newButtonStates){
    for (uint8_t i = 0; i < length; i++) buttonStates[i] = newButtonStates>>i&1;    //when setting to a uint16 set states to bits in variable
}

/*****************
 * BUTTON_BUNCH
*****************/

GM::ButtonBunch::ButtonBunch(uint16_t pin, uint8_t numberOfButtons, uint32_t groundResistance, const uint32_t* resistances):   //Initilize constants and set pin mode, that's all
    length(numberOfButtons),            pin(pin),   
    numberOfVoltages(pow(2,length)-1),     voltages(voltagesInit(length, groundResistance, resistances)),
    prevButtonStates{length}
{ pinMode(pin, INPUT); }

GM::ButtonStates GM::ButtonBunch::getButtonStates(){
    static GM::ButtonStates buttonStates{length}; //Creates static ButtonState class to send

    uint16_t voltage = analogRead(pin);             //Get the mapped voltage
    if (voltage){                                   //if you're getting something from this pin
        uint16_t closestIndex;
        int16_t  closestDist=1024;    //declare some handy vars

        for (uint16_t i = 0; i < numberOfVoltages; i++){   //go through all the voltages
            int16_t currentDist = abs(int16_t(voltages[i]) - int16_t(voltage));    //find the difference between the voltage you're on and the one you're getting
            if (currentDist < closestDist){         //if the difference is less than the previousely closest distance
                closestDist = currentDist;          //set the closest distance to the current distance
                closestIndex=i;                     //and set the closest distance's index value
            }
        }

        if (closestDist > 4)
            buttonStates = prevButtonStates;    //If voltage not recognized, set state to previous state (to prevent flicker)
        else
            buttonStates = closestIndex+1;  //Now shimmy through that index value because each bit in it represents a button state

    } else {                                        //if you're getting nothing
        buttonStates = false; //just set all the button states to false
    }
    prevButtonStates = buttonStates;

    return buttonStates;
}

/*******************************
 * BUTTON_BUNCH PRIVATE PARTS
*******************************/

const uint16_t* const GM::ButtonBunch::voltagesInit(const uint8_t numberOfButtons, uint32_t groundResistance, const uint32_t* resistances){    //Function for initing const voltages

    uint16_t* voltageBuff = new uint16_t[numberOfVoltages];    //create a buffer for the voltages to be stored in

    for (uint16_t i = 0; i < numberOfVoltages; i++){   //Now go through for each voltage value
        double sum = 0;                             //handy var (sum of (1/r)s)
        for (uint8_t j = 0; j < numberOfButtons; j++)  //Now go through for each button value
            if (i+1>>j&1) sum += 1.0/resistances[j];    //and if the button's bit is on in the voltage value, add the inverse of the button's resistance to sum (I know this can be done better but if it aint broke...)
                                                        //I don't know what the hell I'm doing so just look here: https://www.khanacademy.org/science/electrical-engineering/ee-circuit-analysis-topic/ee-resistor-circuits/a/ee-parallel-resistors
        double r1 = 1.0/sum, r2 = groundResistance;     //set both resistances
                                                        
                                                    //assume that [V_Pin*R_Buttons = V_Ground*R_Ground] (idk what I'm doing but it works)
        voltageBuff[i] = (r2/(r1+r2))*1024.0;       //Ugh equation derived from the one right there^
    }

    return voltageBuff;
}



