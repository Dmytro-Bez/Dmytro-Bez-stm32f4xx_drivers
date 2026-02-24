/*
 * stm32f4xx_gpio_driver.h
 *
 *  Created on: Feb 12, 2026
 *      Author: chunin1988
 */

#ifndef INC_STM32F4XX_GPIO_DRIVER_H_
#define INC_STM32F4XX_GPIO_DRIVER_H_

#include"stm32f407xx.h"

typedef struct{
	uint8_t GPIO_PinNummber;			//Значення з @GPIO_PinNummber
	uint8_t GPIO_PinMode;				//Значення з @GPIO_PIN_MODE
	uint8_t GPIO_PinSpeed;				//Значення з @GPIO_PinSpeed
	uint8_t GPIO_PinPuPdControl;		//Значення з @GPIO_PinPuPdControl
	uint8_t GPIO_PinOPType;				//Значення з @GPIO_PinOPType
	uint8_t GPIO_PinAltFunMode;
}GPIO_PinConfig_t;

/*
 * Створення хедлера портів
 */
typedef struct{
	//має бути вказівник на базову адресу переферійного пристрою GPIOx
	GPIO_RegDef_t *pGPIOx;
	GPIO_PinConfig_t GPIO_PinConfig;		//Зберігання значення налаштування конфігурації виводів GPIOx
}GPIO_Handler_t;

/****************************************************
 * APIs підтримка драйверів протів і пристроїв
 ***************************************************/
/*
 * Налаштування переферійного годинника
 */
void GPIO_PerClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);

/*
 * Ініціалізація і Деініціалізація
 */
void GPIO_Init(GPIO_Handler_t *pGPIOHandler);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

/*
 * Читання і запис
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/*
 * IRQ налаштування і ISR запит
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriorty,uint8_t EnonDi);
void GPIO_IRQHanling(uint8_t PinNumber);

/*
 *	@GPIO_PinNummber
 */
#define GPIO_PIN_NO_0			0
#define GPIO_PIN_NO_1			1
#define GPIO_PIN_NO_2			2
#define GPIO_PIN_NO_3			3
#define GPIO_PIN_NO_4			4
#define GPIO_PIN_NO_5			5
#define GPIO_PIN_NO_6			6
#define GPIO_PIN_NO_7			7
#define GPIO_PIN_NO_8			8
#define GPIO_PIN_NO_9			9
#define GPIO_PIN_NO_10			10
#define GPIO_PIN_NO_11			11
#define GPIO_PIN_NO_12			12
#define GPIO_PIN_NO_13			13
#define GPIO_PIN_NO_14			14
#define GPIO_PIN_NO_15			15

/*
 *	@GPIO_PIN_MODE 
 */
#define GPIO_MODE_IN			0
#define GPIO_MODE_OUT			1
#define GPIO_MODE_ALTFN			2
#define GPIO_MODE_ANALOG		3
#define GPIO_MODE_IT_FT			4
#define GPIO_MODE_IT_RT			5
#define GPIO_MODE_IT_RTF		6

/*
 *	@GPIO_PinOPType
 */
#define GPIO_OPT_TYPE_PP		0
#define GPIO_OPT_TYPE_OD		1

/*
 *	@GPIO_PinSpeed
 */
#define GPIO_SPEED_LOW			0
#define GPIO_SPEED_MEDIUM		1
#define GPIO_SPEED_FAST			2
#define GPIO_SPEED_HIGH			3

/*
 *	@GPIO_PinPuPdControl
 */
#define GPIO_NO_PUPD			0
#define GPIO_PIN_PU				1
#define GPIO_PIN_PD				2

#endif /* INC_STM32F4XX_GPIO_DRIVER_H_ */
