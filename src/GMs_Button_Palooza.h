#include <Arduino.h>

#ifndef GM_BUTTON_PALOOZA_H
#define GM_BUTTON_PALOOZA_H
namespace GM
{
    class ButtonState{
        public:
         bool getButton();
         bool getButtonDown();
         bool getButtonUp();

         void operator=(bool newValue);
         void operator=(ButtonState newState);

        private:
         uint8_t buttonState;
    };

    class Button{
        public:
                          Button(uint16_t pin);
         GM::ButtonState& getButtonState();

        private:
         const uint16_t pin;
         GM::ButtonState buttonState;
    };

    class ButtonBunch{
        public:
         const uint8_t      length;
         
                                ButtonBunch(uint16_t pin, uint8_t numberOfButtons, uint32_t groundResistance, const uint32_t* resistances);
         GM::ButtonState* const getButtonStates();

        private:
         const uint16_t         pin, numberOfVoltages;
         const  int16_t* const  voltages;

         GM::ButtonState* const buttonStates,* const prevButtonStates;
         
         const int16_t* const voltagesInit(uint8_t numberOfButtons, uint32_t groundResistance, const uint32_t* resistances);
    };
} // namespace GM
#endif