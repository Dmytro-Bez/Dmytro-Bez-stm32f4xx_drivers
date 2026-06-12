#ifndef INC_STM32F4XX_SPI_DRIVER_H_
#define INC_STM32F4XX_SPI_DRIVER_H_

#include"stm32f407xx.h"



/*
 * Структура для конфігурації SPx переферії
*/
typedef struct{
	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	uint8_t SPI_SCLSpeed;
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;
}SPI_Config_t;

/*
 * Структура для SPx переферії
*/
typedef struct{
	SPI_TypeDef  *pSPIx;
	SPI_Config_t SPIConfig;
}SPI_Handler_t;

/****************************************************
 * APIs підтримка драйвер протів SPIx
 ***************************************************
*/

/*
 * Налаштування переферійного годинника SPI
 */
void SPI_PerClockControl(SPI_TypeDef *pSPIx, uint8_t EnorDi);

/*
 * Ініціалізація та Деініцілізація SPI
 */
void SPI_Init(SPI_Handler_t *pSPIHandler);
void SPI_DeInit(SPI_TypeDef *pSPIx);

/*
 * Передача і запис даних SPI
 */
void SPI_SendDate(SPI_TypeDef *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_SPI_RecevieDate(SPI_TypeDef *pSPIx, uint8_t *pTxBuffer, uint32_t Len);

/*
 * Переривання SPI ISR
 */
void SPI_IRQConfig(uint8_t IRQNumber,uint8_t EnonDi);
void SPI_IRQHanling(SPI_Handler_t *pSPIHandler);
void SPI_IRQPrior(uint32_t IRQPriorty,uint8_t IRQNumber);

/*
 *Інші переферійні ІРА
 */
void SPI_PeripheralControl(SPI_TypeDef *pSPIx, uint8_t EnorDi);

/*
 * Макрос дозволу таймера для GPIOx переферії
 */
#define SPI1_PCLK_EN() (RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN() (RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN() (RCC->APB1ENR |= (1 << 15))

#define ENABLE				1
#define DISEBALE 			0
#define SET					ENABLE
#define RESET				DISEBALE

/*
 * @SPI_DeviceMode
 */
#define SPI_DEVICE_MODE_MASTER			1
#define SPI_DEVICE_MODE_SLAVE			0

/*
 * @SPI_BusConfig
 */
#define SPI_BUS_CONFIG_FD				1
#define SPI_BUS_CONFIG_HD				2
#define SPI_BUS_CONFIG_SIMPLEX_TXONLY	3
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY	4

/*
 * SPI_SCLSpeed
 */
#define SPI_SCL_SPEED_DIV2				0
#define SPI_SCL_SPEED_DIV4				1
#define SPI_SCL_SPEED_DIV8				2
#define SPI_SCL_SPEED_DIV16				3
#define SPI_SCL_SPEED_DIV32				4
#define SPI_SCL_SPEED_DIV64				5
#define SPI_SCL_SPEED_DIV128			6
#define SPI_SCL_SPEED_DIV256			7

/*
 * @SPI_DFF
 */
#define SPI_DFF_8BITS					0
#define SPI_DFF_16BITS					1

/*
 * @SPI_CPOL
*/
#define SPI_CPOL_HIGH					1
#define SPI_CPOL_LOW					0

/*
 * @SPI_CPHA
*/
#define SPI_CPHA_HIGH					1
#define SPI_CPHA_LOW					0

/*
 * @SPI_SSM
*/
#define SPI_SSM_EN						1
#define SPI_SSM_DI						0

#define SPI_TXE_FLAG					(1 << SPI_SR_TXE)
#define SPI_RXE_FLAG					(1 << SPI_SR_RXE)
#define SPI_BUSY_FLAG					(1 << SPI_SR_BSY)

#define FLAG_RESET			RESET
#define FLAG_SET			SET
#endif /* INC_STM32F4XX_SPI_DRIVER_H_ */
