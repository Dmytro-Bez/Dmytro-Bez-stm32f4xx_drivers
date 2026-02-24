/*
 * stm32f4xx_gpio_driver.c
 *
 *  Created on: Feb 12, 2026
 *      Author: chunin1988
 */

#include"stm32f4xx_gpio_driver.h"

/********************************************************************
 * @fn			- GPIO_PerClockControl
 *
 * @brief		- Ця функція дозвляє включити тактування потрі
 *
 * @param[in]	- Базова адреса перефірійного пристрою GPIOх
 * @param[in]	- Дозвіл чи не дозвіл макроса(...)
 * @param[in]	-
 *
 * @return		- none
 *
 * @Note		- none
 */
void GPIO_PerClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(pGPIOx == GPIOA){
			GPIOA_PCLK_EN();
		} else if(pGPIOx == GPIOB){
			GPIOB_PCLK_EN();
		} else if(pGPIOx == GPIOC){
			GPIOC_PCLK_EN();
		} else if(pGPIOx == GPIOD){
			GPIOD_PCLK_EN();
		} else if(pGPIOx == GPIOE){
			GPIOE_PCLK_EN();
		} else if(pGPIOx == GPIOF){
			GPIOF_PCLK_EN();
		} else if(pGPIOx == GPIOG){
			GPIOG_PCLK_EN();
		}
	}
	else{
		//Too
	}
}

/********************************************************************
 * @fn			- GPIO_Init
 *
 * @brief		- Ця функція дозвляє ініціалізувати роботу певного порту
 *
 * @param[in]	- Використання Хендлера порту
 * @param[in]	-
 * @param[in]	-
 *
 * @return		- none
 *
 * @Note		- none
 */
void GPIO_Init(GPIO_Handler_t *pGPIOHandler){
	uint32_t temp = 0;
	if(pGPIOHandler->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG){
		temp = (pGPIOHandler->GPIO_PinConfig.GPIO_PinMode << (2 *pGPIOHandler->GPIO_PinConfig.GPIO_PinNummber));
		pGPIOHandler->pGPIOx->MODER &= ~(0x3 << pGPIOHandler->GPIO_PinConfig.GPIO_PinNummber);		//Очистика бітів 
		pGPIOHandler->pGPIOx->MODER |= temp;		//налаштування
	} else{
		//Тут ми будемо працювати з прериванням
	}
/*
 * розібратись з налаштуванням швидкості порту!!!!
 * налаштування типу роботи
 * і підтягнення*/
	temp = 0;
	temp = (pGPIOHandler->GPIO_PinConfig.GPIO_PinSpeed << (2 *pGPIOHandler->GPIO_PinConfig.GPIO_PinNummber));
	pGPIOHandler->pGPIOx->OSPEEDR &= ~(0x3 << pGPIOHandler->GPIO_PinConfig.GPIO_PinNummber);		//Очистика бітів 
	pGPIOHandler->pGPIOx->OSPEEDR |= temp;

	temp = 0;
	temp = (pGPIOHandler->GPIO_PinConfig.GPIO_PinPuPdControl << (2 *pGPIOHandler->GPIO_PinConfig.GPIO_PinNummber));
	pGPIOHandler->pGPIOx->PUPDR &= ~(0x1 << pGPIOHandler->GPIO_PinConfig.GPIO_PinNummber);		//Очистика бітів 
	pGPIOHandler->pGPIOx->PUPDR |= temp;

	temp = 0;
	temp = (pGPIOHandler->GPIO_PinConfig.GPIO_PinOPType << pGPIOHandler->GPIO_PinConfig.GPIO_PinNummber);
	pGPIOHandler->pGPIOx->OTYPER &= ~(0x3 << pGPIOHandler->GPIO_PinConfig.GPIO_PinNummber);		//Очистика бітів 
	pGPIOHandler->pGPIOx->OTYPER |= temp;
/*
 * Робота з альтернативними функціями
*/
	if (pGPIOHandler->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ALTFN)
	{
		/* Налаштування альтернативних функцій*/
		uint8_t temp1, temp2;
		temp1 = pGPIOHandler->GPIO_PinConfig.GPIO_PinNummber/8;
		temp2 = pGPIOHandler->GPIO_PinConfig.GPIO_PinNummber%8;
		pGPIOHandler->pGPIOx->AFR[temp1] &= ~(0xF << (4 * temp2));		//Очистка
		pGPIOHandler->pGPIOx->AFR[temp1] |= (pGPIOHandler->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * temp2));
	}
	
}

/********************************************************************
 * @fn			- GPIO_DeInit
 *
 * @brief		- Ця функція дозвляє Деініціалізувати роботу певного порту
 *
 * @param[in]	- Використання структури переферійних пристроїв порту
 * @param[in]	-
 * @param[in]	-
 *
 * @return		- none
 *
 * @Note		- none
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx){
	if(pGPIOx == GPIOA){
		GPIOA_REG_RESET();
	} else if(pGPIOx == GPIOB){
		GPIOB_PCLK_EN();
	} else if(pGPIOx == GPIOC){
		GPIOC_PCLK_EN();
	} else if(pGPIOx == GPIOD){
		GPIOD_PCLK_EN();
	} else if(pGPIOx == GPIOE){
		GPIOE_PCLK_EN();
	} else if(pGPIOx == GPIOF){
		GPIOF_PCLK_EN();
	} else if(pGPIOx == GPIOG){
		GPIOG_PCLK_EN();
	} else if(pGPIOx == GPIOH){
		GPIOH_PCLK_EN();
	} else if(pGPIOx == GPIOI){
		GPIOI_PCLK_EN();
	}
}

/********************************************************************
 * @fn			- GPIO_ReadFromInputPin
 *
 * @brief		- Ця функція дозвляє читати дані з певного контакту порта
 *
 * @param[in]	- Використання структури переферійних пристроїв порту
 * @param[in]	- Використовує номер контакту порта
 * @param[in]	-
 *
 * @return		- Повертає значення котакта порту
 *
 * @Note		- none
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
	uint8_t value = 0;
	value = (uint8_t)((pGPIOx->IDR << PinNumber) & 0x00000001);
	return value;
}

/********************************************************************
 * @fn			- GPIO_ReadFromInputPort
 *
 * @brief		- Ця функція дозвляє читати дані з певного порта мікроконтролера
 *
 * @param[in]	- Використання структури переферійних пристроїв порту
 * @param[in]	-
 * @param[in]	-
 *
 * @return		- Повертає значення порту
 *
 * @Note		- none
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx){
	uint16_t value = 0;
	value = (uint16_t)pGPIOx->IDR;
	return value;
}

/********************************************************************
 * @fn			- GPIO_WriteToOutputPin
 *
 * @brief		- Ця функція дозвляє записувати дані в певний контакт порту мікроконтролера
 *
 * @param[in]	- Використання структури переферійних пристроїв порту
 * @param[in]	- Використовує номер контакту порта
 * @param[in]	- Використовує значення
 *
 * @return		- none
 *
 * @Note		- none
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t value){
	if(value == GPIO_PIN_SET){
		//Записуємо 1
		pGPIOx->ODR |= (1 << PinNumber);
	}else{
		//Записуємо 0
		pGPIOx->ODR &= ~(1 << PinNumber);
	}
}

/********************************************************************
 * @fn			- GPIO_WriteToOutputPort
 *
 * @brief		- Ця функція дозвляє записувати дані в певний порт мікроконтролера
 *
 * @param[in]	- Використання структури переферійних пристроїв порту
 * @param[in]	-
 * @param[in]	- Використовує значення
 *
 * @return		- none
 *
 * @Note		- none
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value){
	pGPIOx->ODR = value;
}

/********************************************************************
 * @fn			- GPIO_ToggleOutputPin
 *
 * @brief		- Ця функція дозвляє записувати дані в певний порт мікроконтролера зі зміною значень HIGH/LOW
 *
 * @param[in]	- Використання структури переферійних пристроїв порту
 * @param[in]	- Використовує номер контакту порта
 * @param[in]	-
 *
 * @return		- none
 *
 * @Note		- none
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
	pGPIOx->ODR ^= (1 << PinNumber);
}

/********************************************************************
 * @fn			- GPIO_IRQConfig
 *
 * @brief		- Ця функція дозвляє налаштувати преривання
 *
 * @param[in]	- Використання номер преривання
 * @param[in]	- Використовує значення пріорітету преривання
 * @param[in]	- Використовує значення дозвіл/не дозвіл преривання
 *
 * @return		- none
 *
 * @Note		- none
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriorty,uint8_t EnonDi){

}

/********************************************************************
 * @fn			- GPIO_IRQHanling
 *
 * @brief		- Ця функція дозвляє викликати преривання
 *
 * @param[in]	- Використання номер контакту порту
 * @param[in]	-
 * @param[in]	-
 *
 * @return		- none
 *
 * @Note		- none
 */
void GPIO_IRQHanling(uint8_t PinNumber){

}
