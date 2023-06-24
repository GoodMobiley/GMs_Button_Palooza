#include <Arduino.h>

#ifndef GM_BUTTON_PALOOZA_H
#define GM_BUTTON_PALOOZA_H
namespace GM
{
    class ButtonStates{
        private:
         bool* const buttonStates;

        public:
                ButtonStates(uint16_t num);
         bool&  operator[](uint16_t index);
         
         const uint16_t length;
    };

    class ButtonBunch{
        private:
         const uint16_t         pin, numOfButtons, numOfVoltages;
         const uint16_t* const  voltages;
         
         const uint16_t* const voltagesInit(uint16_t num, uint32_t groundResistance, const uint32_t* resistances);

        public:
                            ButtonBunch(uint16_t pin, uint16_t num, uint32_t groundResistance, const uint32_t* resistances);
         GM::ButtonStates   getButtonStates();
    };
} // namespace GM
#endif