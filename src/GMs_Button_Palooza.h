#include <Arduino.h>

#ifndef GM_BUTTON_PALOOZA_H
#define GM_BUTTON_PALOOZA_H
namespace GM
{
    class ButtonStates{
        public:
         const uint8_t length;

                ButtonStates(uint8_t numberOfButtons);
         bool&  operator[](uint16_t index);
         void   operator=(ButtonStates& newButtonStates);
         void   operator=(bool newButtonStates);
         void   operator=(uint16_t newButtonStates);

        private:
         bool* const buttonStates;
    };

    class ButtonBunch{
        public:
         const uint8_t      length;
         
                            ButtonBunch(uint16_t pin, uint8_t numberOfButtons, uint32_t groundResistance, const uint32_t* resistances);
         GM::ButtonStates   getButtonStates();

        private:
         const uint16_t         pin, numberOfVoltages;
         const uint16_t* const  voltages;

         GM::ButtonStates prevButtonStates;
         
         const uint16_t* const voltagesInit(uint8_t numberOfButtons, uint32_t groundResistance, const uint32_t* resistances);
    };
} // namespace GM
#endif