#include "main.h"

void delay_t(void){
	for (int32_t i = 0; i < 500000/2; i++);
}

int main(void)
{
	GPIO_Handler_t gpiox, gpioButt;
	memset(&gpiox,0,sizeof(gpiox));
	memset(&gpioButt,0,sizeof(gpioButt));

	gpiox.pGPIOx = GPIOD;
	gpiox.GPIO_PinConfig.GPIO_PinNummber = GPIO_PIN_NO_12;
	gpiox.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	gpiox.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	gpiox.GPIO_PinConfig.GPIO_PinOPType = GPIO_OPT_TYPE_PP;
//		gpiox.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PD;				//Для використання внутрішнього резистора
	gpiox.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PerClockControl(GPIOD,ENABLE);
	GPIO_Init(&gpiox);

//	gpioButt.pGPIOx = GPIOA;
	gpioButt.pGPIOx = GPIOD;
	gpioButt.GPIO_PinConfig.GPIO_PinNummber = GPIO_PIN_NO_0;
	gpioButt.GPIO_PinConfig.GPIO_PinNummber = GPIO_PIN_NO_5;
	gpioButt.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT; 			//Переривання будуть відбуватись при кожному натисканні кнопки
	gpioButt.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
//	gpioButt.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	gpioButt.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

//	GPIO_PerClockControl(GPIOA,ENABLE);
	GPIO_PerClockControl(GPIOD,ENABLE);
	GPIO_Init(&gpioButt);

//	GPIO_IRQPrior(IRQn_NO_EXTI0,NVIC_IRQ_PRI0);
//	GPIO_IRQConfig(IRQn_NO_EXTI0,ENABLE);
	GPIO_IRQPrior(IRQn_NO_EXTI9_5,NVIC_IRQ_PRI15);
	GPIO_IRQConfig(IRQn_NO_EXTI9_5,ENABLE);

	while(1);
}

void EXTI0_IRQHandler(void){
//	GPIO_IRQHanling(GPIO_PIN_NO_0);
//	GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_12);

	GPIO_IRQHanling(GPIO_PIN_NO_5);
	GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_12);
}
