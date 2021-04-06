#include <stdint.h>
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_gpio.h"

// Declaring functions

//void debounceInit();
//void deBounceInit(uint16_t pin, char port, int8_t mode);
//int8_t deBounceReadPin(uint16_t pin, char port, int8_t mode);

// Defining functions

//void debounceInit()
//{
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	GPIO_InitStruct.Pin = GPIO_PIN_8;
//
//}


void deBounceInit(uint16_t pin, char port, int8_t mode)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0}; // Declaring GPIO_initStruct as GPIO_InitTypeDef Struc
    switch(pin)
	{
    	case 3: GPIO_InitStruct.Pin = GPIO_PIN_3; break;
    	case 4: GPIO_InitStruct.Pin = GPIO_PIN_4; break;
    	case 7: GPIO_InitStruct.Pin = GPIO_PIN_7; break;
    	case 8: GPIO_InitStruct.Pin = GPIO_PIN_8; break;
	}

    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

int8_t deBounceReadPin(uint16_t pin, char port, int8_t mode)
{
	GPIO_PinState pinState = GPIO_PIN_RESET;
	int8_t pinStateWeAreLookingFor;

	int32_t msTimeStamp = HAL_GetTick();
	int32_t stableInterval = 0;

	pinState = HAL_GPIO_ReadPin(GPIOA, pin);

	if(pinState == GPIO_PIN_RESET) {
		pinStateWeAreLookingFor = 0;
	} else {
		pinStateWeAreLookingFor = 1;
	}
	while(HAL_GetTick() < (msTimeStamp + stableInterval)) {
		pinState = HAL_GPIO_ReadPin(GPIOA, pin);
		if(pinState != pinStateWeAreLookingFor) {
			pinStateWeAreLookingFor = !pinStateWeAreLookingFor;
			msTimeStamp = HAL_GetTick();
		}
	}
	return pinStateWeAreLookingFor;

}
