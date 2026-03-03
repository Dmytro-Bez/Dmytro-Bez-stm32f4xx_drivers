#include "main.h"

void delay_t(void){
	for (int32_t i = 0; i < 500000/2; i++);
}

int main(void)
{
	GPIO_Handler_t gpiox, gpioButt;

	gpiox.pGPIOx = GPIOD;
	gpiox.GPIO_PinConfig.GPIO_PinNummber = GPIO_PIN_NO_12;
	gpiox.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	gpiox.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	gpiox.GPIO_PinConfig.GPIO_PinOPType = GPIO_OPT_TYPE_PP;
	gpiox.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PD;	//Для використання внутрішнього резистора

	GPIO_PerClockControl(GPIOD,ENABLE);
	GPIO_Init(&gpiox);

	gpioButt.pGPIOx = GPIOA;
	gpioButt.GPIO_PinConfig.GPIO_PinNummber = GPIO_PIN_NO_0;
	gpioButt.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	gpioButt.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	gpioButt.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PerClockControl(GPIOA,ENABLE);
	GPIO_Init(&gpioButt);

	while (1)
	{
		if(GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) == BUT_PRESS){
			delay_t();
			GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_12);
		}
	}
}
