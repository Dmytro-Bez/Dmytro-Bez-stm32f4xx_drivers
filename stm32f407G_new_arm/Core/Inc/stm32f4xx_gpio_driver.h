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
	GPIO_TypeDef *pGPIOx;
	GPIO_PinConfig_t GPIO_PinConfig;		//Зберігання значення налаштування конфігурації виводів GPIOx
}GPIO_Handler_t;

/****************************************************
 * APIs підтримка драйверів протів і пристроїв
 ***************************************************/
/*
 * Налаштування переферійного годинника
 */
void GPIO_PerClockControl(GPIO_TypeDef *pGPIOx, uint8_t EnorDi);

/*
 * Ініціалізація і Деініціалізація
 */
void GPIO_Init(GPIO_Handler_t *pGPIOHandler);
void GPIO_DeInit(GPIO_TypeDef *pGPIOx);

/*
 * Читання і запис
 */
uint8_t GPIO_ReadFromInputPin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_TypeDef *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber, uint8_t value);
void GPIO_WriteToOutputPort(GPIO_TypeDef *pGPIOx, uint16_t value);
void GPIO_ToggleOutputPin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber);

/*
 * IRQ налаштування і ISR запит
 */
void GPIO_IRQConfig(uint8_t IRQNumber,uint8_t EnonDi);
void GPIO_IRQHanling(uint8_t PinNumber);
void GPIO_IRQPrior(uint32_t IRQPriorty,uint8_t IRQNumber);

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

#define ENABLE				1
#define DISEBALE 			0
#define SET					ENABLE
#define RESET				DISEBALE
#define GPIO_PIN_SET		SET
#define GPIO_PIN_RESET		RESET

#define HIGH 				1
#define BUT_PRESS 			HIGH

/* Макрос для деініціалізації портів */
#define GPIOA_REG_RESET()	do{ (RCC->AHB1RSTR |= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0)); }while(0)
#define GPIOB_REG_RESET()	do{ (RCC->AHB1RSTR |= (1 << 1)); (RCC->AHB1RSTR &= ~(1 << 1)); }while(0)
#define GPIOD_REG_RESET()	do{ (RCC->AHB1RSTR |= (1 << 2)); (RCC->AHB1RSTR &= ~(1 << 2)); }while(0)
#define GPIOE_REG_RESET()	do{ (RCC->AHB1RSTR |= (1 << 3)); (RCC->AHB1RSTR &= ~(1 << 3)); }while(0)
#define GPIOF_REG_RESET()	do{ (RCC->AHB1RSTR |= (1 << 4)); (RCC->AHB1RSTR &= ~(1 << 4)); }while(0)
#define GPIOG_REG_RESET()	do{ (RCC->AHB1RSTR |= (1 << 5)); (RCC->AHB1RSTR &= ~(1 << 5)); }while(0)
#define GPIOH_REG_RESET()	do{ (RCC->AHB1RSTR |= (1 << 6)); (RCC->AHB1RSTR &= ~(1 << 6)); }while(0)
#define GPIOI_REG_RESET()	do{ (RCC->AHB1RSTR |= (1 << 7)); (RCC->AHB1RSTR &= ~(1 << 7)); }while(0)

#define GPIOA_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 4))
#define GPIOF_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 5))
#define GPIOG_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 6))
#define GPIOH_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 7))
#define GPIOI_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 8))

/*
 * Макрос дозволу таймера для GPIOx переферії
 */
#define GPIOA_PCLK_EN()	(RCC->AHB1ENR |= (1 << 0))
#define GPIOB_PCLK_EN()	(RCC->AHB1ENR |= (1 << 1))
#define GPIOC_PCLK_EN()	(RCC->AHB1ENR |= (1 << 2))
#define GPIOD_PCLK_EN()	(RCC->AHB1ENR |= (1 << 3))
#define GPIOE_PCLK_EN()	(RCC->AHB1ENR |= (1 << 4))
#define GPIOF_PCLK_EN()	(RCC->AHB1ENR |= (1 << 5))
#define GPIOG_PCLK_EN()	(RCC->AHB1ENR |= (1 << 6))
#define GPIOH_PCLK_EN()	(RCC->AHB1ENR |= (1 << 7))
#define GPIOI_PCLK_EN()	(RCC->AHB1ENR |= (1 << 8))

#define SYSCFG_PCLK_EN()						(RCC->AHB2ENR |= (1 << 14))
#define GPIO_BASEADD_TO_CODE(x)					((x == GPIOA)?0:\
												(x == GPIOB)?1:\
												(x == GPIOC)?2:\
												(x == GPIOB)?3:\
												(x == GPIOA)?4:\
												(x == GPIOB)?5:\
												(x == GPIOA)?6:\
												(x == GPIOB)?7:0)
#define IRQn_NO_EXTI0		6
#define IRQn_NO_EXTI1		7
#define IRQn_NO_EXTI2		8
#define IRQn_NO_EXTI3		9
#define IRQn_NO_EXTI4		10
#define IRQn_NO_EXTI9_5		23
#define IRQn_NO_EXTI15_10	40

/*
 * ARM CORTEX M4 NVIC ISERx регітри адрес
 * */
#define NVIC_ISER0			((__IO uint32_t*)0xE000E100)
#define NVIC_ISER1			((__IO uint32_t*)0xE000E104)
#define NVIC_ISER2			((__IO uint32_t*)0xE000E108)
#define NVIC_ISER3			((__IO uint32_t*)0xE000E10C)

/*
 * ARM CORTEX M4 NVIC ICERx регітри адрес
 * */
#define NVIC_ICER0			((__IO uint32_t*)0xE000E180)
#define NVIC_ICER1			((__IO uint32_t*)0xE000E184)
#define NVIC_ICER2			((__IO uint32_t*)0xE000E188)
#define NVIC_ICER3			((__IO uint32_t*)0xE000E18C)

#define NVIC_PR_BASE_ADDR			((__IO uint32_t*)0xE000E400)
#define NO_PR_BITS_IMPLEMENT		4

#define NVIC_IRQ_PRI0				0
#define NVIC_IRQ_PRI15				15


#endif /* INC_STM32F4XX_GPIO_DRIVER_H_ */
