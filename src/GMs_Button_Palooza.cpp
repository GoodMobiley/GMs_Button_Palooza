#include "GMs_Button_Palooza.h"

/******************
 * BUTTON_STATES
******************/

GM::ButtonStates::ButtonStates(uint16_t num):   //Yay, Initialize constants!
    buttonStates(new bool[num]),
    length(num)                             {}

bool& GM::ButtonStates::operator[](uint16_t index) {return buttonStates[index];}    //Yeah so basically just reference the pointer when using [] operator

/*****************
 * BUTTON_BUNCH
*****************/

GM::ButtonBunch::ButtonBunch(uint16_t pin, uint16_t num, uint32_t groundResistance, const uint32_t* resistances):   //Initilize constants and set pin mode, that's all
    numOfButtons(num),              pin(pin),   
    numOfVoltages(pow(2,num)-1),    voltages(voltagesInit(numOfButtons, groundResistance, resistances))
{ pinMode(pin, INPUT); }

GM::ButtonStates GM::ButtonBunch::getButtonStates(){
    static GM::ButtonStates buttonStates{numOfButtons}; //Creates static ButtonState class to send

    uint16_t voltage = analogRead(pin);             //Get the mapped voltage
    if (voltage){                                   //if you're getting something from this pin
        uint16_t closestIndex, closestDist=1024;    //declare some handy vars

        for (uint16_t i = 0; i < numOfVoltages; i++){   //go through all the voltages
            uint16_t currentDist = abs(int16_t(voltages[i]) - int16_t(voltage));    //find the difference between the voltage you're on and the one you're getting
            if (currentDist < closestDist){         //if the difference is less than the previousely closest distance
                closestDist = currentDist;          //set the closest distance to the current distance
                closestIndex=i;                     //and set the closest distance's index value
            }
        }

        for (uint16_t i = 0; i < numOfButtons; i++) buttonStates[i] = closestIndex+1>>i&1;  //Now shimmy through that index value
                                                                                            //because each bit in it represents a button state
    } else {                                        //if you're getting nothing
        for (uint16_t i = 0; i < numOfButtons; i++) buttonStates[i] = false; //just set all the button states to false
    }
    return buttonStates;
}

/*******************************
 * BUTTON_BUNCH PRIVATE PARTS
*******************************/

const uint16_t* const GM::ButtonBunch::voltagesInit(const uint16_t num, uint32_t groundResistance, const uint32_t* resistances){    //Function for initing const voltages

    uint16_t* voltageBuff = new uint16_t[numOfVoltages];    //create a buffer for the voltages to be stored in

    for (uint16_t i = 0; i < numOfVoltages; i++){   //Now go through for each voltage value
        double sum = 0;                             //handy var (sum of (1/r)s)
        for (uint16_t j = 0; j < num; j++)          //Now go through for each button value
            if (i+1>>j&1) sum += 1.0/resistances[j];    //and if the button's bit is on in the voltage value, add the inverse of the button's resistance to sum (I know this can be done better but if it aint broke...)
                                                        //I don't know what the hell I'm doing so just look here: https://www.khanacademy.org/science/electrical-engineering/ee-circuit-analysis-topic/ee-resistor-circuits/a/ee-parallel-resistors
        double r1 = 1.0/sum, r2 = groundResistance;     //set both resistances
                                                        
        voltageBuff[i] = (1.0 - (r1*r2)                 //and assume that [V_Pin*R_Buttons = V_Ground*R_Ground] (idk what I'm doing but it works)
                             /(r2*(r1+r2))) *1024.0;    //Ugh equation derived from the one right there^
    }
    return voltageBuff;
}



