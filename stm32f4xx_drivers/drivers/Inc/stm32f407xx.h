/*
 * stm32f407xx.h
 *
 *  Created on: Feb 11, 2026
 *      Author: chunin1988
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stdint.h>
#include "stm32f4xx_gpio_driver.h"

#define _vo					volatile
#define ENABLE				1
#define DISEBALE 			0
#define SET					ENABLE
#define RESET				DISEBALE
#define GPIO_PIN_SET		SET
#define GPIO_PIN_RESET		RESET

/*Вказуємо розмір пам'яті SRAM i FLASH*/
#define FLASH_BASEADDR		0x08000000U
#define SRAM1_BASEADDR		0x20000000U	//112KBt
#define SRAM2_BASEADDR		0x20001C00U
#define ROM_BASEADD	R		0x1FFFF000U
#define SRAM				SRAM1_BASEADDR

/*AHBx та APBx шина адрес переферії*/
#define PERIPH_BASE			0x40000000U
#define APB1PERIPH_BASE		PERIPH_BASE
#define APB2PERIPH_BASE		0x40010000U
#define AHB1PERIPH_BASE		0x400200000U
#define AHB2PERIPH_BASE		0x50000000U	//Використовується для продвинутих контрлерів

/*Визначення адрес пристроїв,що висять на шині АНВ1*/
#define GPIOA_BASEADDR		(AHB1PERIPH_BASE + 0x0000)
#define GPIOB_BASEADDR		(AHB1PERIPH_BASE + 0x0400)
#define GPIOC_BASEADDR		(AHB1PERIPH_BASE + 0x0800)
#define GPIOD_BASEADDR		(AHB1PERIPH_BASE + 0x0C00)
#define GPIOE_BASEADDR		(AHB1PERIPH_BASE + 0x1000)
#define GPIOF_BASEADDR		(AHB1PERIPH_BASE + 0x1400)
#define GPIOG_BASEADDR		(AHB1PERIPH_BASE + 0x1800)
#define GPIOH_BASEADDR		(AHB1PERIPH_BASE + 0x1C00)
#define GPIOI_BASEADDR		(AHB1PERIPH_BASE + 0x2000)

#define RCC_BASEADDR		(AHB1PERIPH_BASE + 0x3800)

#define I2C1_BASEADDR		(AHB1PERIPH_BASE + 0x5400)
#define I2C2_BASEADDR		(AHB1PERIPH_BASE + 0x5800)
#define I2C3_BASEADDR		(AHB1PERIPH_BASE + 0x5C00)

#define SPI2_BASEADDR		(AHB1PERIPH_BASE + 0x3800)
#define SPI3_BASEADDR		(AHB1PERIPH_BASE + 0x3C00)

#define USART2_BASEADDR		(AHB1PERIPH_BASE + 0x4400)
#define USART3_BASEADDR		(AHB1PERIPH_BASE + 0x4800)
#define UART4_BASEADDR		(AHB1PERIPH_BASE + 0x4C00)
#define UART5_BASEADDR		(AHB1PERIPH_BASE + 0x5000)

/*Визначення адрес пристроїв,що висять на шині АНВ2*/
#define EXTI_BASEADDR		(APB2PERIPH_BASE + 0x3C00)
#define SPI1_BASEADDR		(APB2PERIPH_BASE + 0x3000)
#define USART1_BASEADDR		(APB2PERIPH_BASE + 0x1000)
#define USART6_BASEADDR		(APB2PERIPH_BASE + 0x1400)
#define USART6_BASEADDR		(APB2PERIPH_BASE + 0x1400)
#define SYSCFG_BASEADDR		(APB2PERIPH_BASE + 0x3800)

/*Структура переферійних регістрів портів GPIOx*/
typedef struct{
	/*GPIO*/
	_vo uint32_t MODER;		//0x00
	_vo uint32_t OTYPER;	//0x04
	_vo uint32_t OSPEEDR;	//0x08
	_vo uint32_t PUPDR;		//0x0C
	_vo uint32_t IDR;		//0x10
	_vo uint32_t ODR;		//0x14
	_vo uint32_t BSRRL;		//0x18
	_vo uint32_t BSRRH;		//0x1A
	_vo uint32_t LCKR;		//0x1C
	_vo uint32_t AFR[2];	//0x20-24

}GPIO_RegDef_t;

/*Переферійні змінні для портів*/
#define GPIOA		((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB		((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC		((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD		((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOF		((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOE		((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOG		((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH		((GPIO_RegDef_t*)GPIOH_BASEADDR)
#define GPIOI		((GPIO_RegDef_t*)GPIOI_BASEADDR)

/*Структура регістрів переферійного годинника RTC*/
typedef struct{
	_vo uint32_t CR;			//0x00
	_vo uint32_t PLLCFGR;		//0x04
	_vo uint32_t CFGR;			//0x08
	_vo uint32_t CIR;			//0x0C
	_vo uint32_t AHB1RSTR;		//0x10
	_vo uint32_t AHB2RSTR;		//0x14
	_vo uint32_t AHB3RSTR;		//0x18
	uint32_t RESERVED0;
	_vo uint32_t APB1RSTR;		//0x20
	_vo uint32_t APB2RSTR;		//0x24
	uint32_t RESERVED01[2];
	_vo uint32_t AHB1ENR;		//0x30
	_vo uint32_t AHB2ENR;		//0x34
	_vo uint32_t AHB3ENR;		//0x38
	uint32_t RESERVED2;
	_vo uint32_t APB1ENR;		//0x40
	_vo uint32_t APB2ENR;		//0x44
	uint32_t RESERVED3[2];
	_vo uint32_t AHB1LPENR;		//0x50
	_vo uint32_t AHB2LPENR;		//0x54
	_vo uint32_t AHB3LPENR;		//0x58
	_vo uint32_t APB1LPENR;		//0x60
	_vo uint32_t APB2LPENR;		//0x64
	uint32_t RESERVED4[2];
	_vo uint32_t BDCR;			//0x70
	_vo uint32_t CSR;			//0x74
	uint32_t RESERVED5[2];
	_vo uint32_t SSCGR;			//0x80
	_vo uint32_t PLLI2SCFGR;	//0x84
	_vo uint32_t PLLSAICFGR;	//0x88
	_vo uint32_t DCKCFGR;		//0x8C
	_vo uint32_t CKGATENR;		//0x90
	_vo uint32_t DCKCFGR2;		//0x94
}RCC_RegDef_t;

#define RCC	((RCC_RegDef_t*)RCC_BASEADDR)

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

/*
 * Макрос дозволу таймера для I2Cx переферії
 */
#define I2C1_PCLK_EN()	(RCC->APB1ENR |= (1 << 21))
//#define I2C2_PCLK_EN()	(RCC->RCC_APB1ENR |= (1 << 22))

/*
 * Макрос дозволу таймера для I2Cx переферії
 */
#define SPI1_PCLK_EN()	(RCC->APB2ENR |= (1 << 12))
//#define SPI2_PCLK_EN()	(RCC->RCC_APB1ENR |= (1 << 14))
//#define SPI3_PCLK_EN()	(RCC->RCC_APB1ENR |= (1 << 15))

/*
 * Макрос дозволу таймера для USARTx переферії
 */
//#define USART1_PCLK_EN()	(RCC->RCC_APB2ENR |= (1 << 14))
//#define USART2_PCLK_EN()	(RCC->RCC_APB1ENR |= (1 << 17))
//#define USART3_PCLK_EN()	(RCC->RCC_APB1ENR |= (1 << 18))

/*
 * Макрос дозволу таймера для UARTx переферії
 */
//#define UART4_PCLK_EN()	(RCC->RCC_APB1ENR |= (1 << 19))
//#define UART5_PCLK_EN()	(RCC->RCC_APB1ENR |= (1 << 20))

#define GPIOA_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 4))
#define GPIOF_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 5))
#define GPIOG_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 6))
#define GPIOH_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 7))
#define GPIOI_PCLK_DI()	(RCC->AHB1ENR &= ~(1 << 8))

/* Макрос для деініціалізації портів */
#define GPIOA_REG_RESET()	do{ (RCC->AHB1RSTR |= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0)); }while(0)
#define GPIOB_REG_RESET()	do{ (RCC->AHB1RSTR |= (1 << 1)); (RCC->AHB1RSTR &= ~(1 << 1)); }while(0)
#define GPIOD_REG_RESET()	do{ (RCC->AHB1RSTR |= (1 << 2)); (RCC->AHB1RSTR &= ~(1 << 2)); }while(0)
#define GPIOE_REG_RESET()	do{ (RCC->AHB1RSTR |= (1 << 3)); (RCC->AHB1RSTR &= ~(1 << 3)); }while(0)
#define GPIOF_REG_RESET()	do{ (RCC->AHB1RSTR |= (1 << 4)); (RCC->AHB1RSTR &= ~(1 << 4)); }while(0)
#define GPIOG_REG_RESET()	do{ (RCC->AHB1RSTR |= (1 << 5)); (RCC->AHB1RSTR &= ~(1 << 5)); }while(0)
#define GPIOH_REG_RESET()	do{ (RCC->AHB1RSTR |= (1 << 6)); (RCC->AHB1RSTR &= ~(1 << 6)); }while(0)
#define GPIOI_REG_RESET()	do{ (RCC->AHB1RSTR |= (1 << 7)); (RCC->AHB1RSTR &= ~(1 << 7)); }while(0)

#endif /* INC_STM32F407XX_H_ */
