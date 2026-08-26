#include "main.h"

void delay_t(void){
	for (int32_t i = 0; i < 500000; i++);
}

int main(void)
{
	GPIO_Handler_t gpiox;

	gpiox.pGPIOx = GPIOD;
//	gpiox.GPIO_PinConfig.GPIO_PinNummber = GPIO_PIN_NO_12;
	gpiox.GPIO_PinConfig.GPIO_PinNummber = GPIO_PIN_NO_13;
	gpiox.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	gpiox.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	gpiox.GPIO_PinConfig.GPIO_PinOPType = GPIO_OPT_TYPE_OD;
//	gpiox.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PD;	//Для використання завнішнього резистора
	gpiox.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;	//Для використання внутрішнього резистора в 32-470Ом(перепмикання світлодіода)

	GPIO_PerClockControl(GPIOD,ENABLE);
	GPIO_Init(&gpiox);

	while (1)
	{
//		GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_12);
		GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_13);
		delay_t();
	}
}
