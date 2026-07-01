#ifndef INC_STM32F4XX_I2C_DRIVER_H_
#define INC_STM32F4XX_I2C_DRIVER_H_

#include"stm32f407xx.h"

/**************************************************************
 *  Дерективи конролеру на дозвіл тактування І2С шини
***************************************************************/
#define I2C1_PCLK_EN()               (RCC->APB2ENR |= (1 << 21))
#define I2C2_PCLK_EN()               (RCC->APB2ENR |= (1 << 22))
#define I2C2_PCLK_EN()               (RCC->APB2ENR |= (1 << 23))

/**************************************************************
 *  Конфігурайційна структура переферії І2С 
***************************************************************/
typedef struct 
{
    uint32_t I2C_SCLKSpeed;
    uint8_t I2C_DeviceAddress;
    uint8_t I2C_ACKControl;
    uint8_t I2C_FMDutyCycle;
}I2C_Config_t;
/*************************************************************
 *  Структура хендлера для І2Сх переферії
**************************************************************/
typedef struct
{
    I2C_TypeDef *pI2Cx;
    I2C_Config_t *pI2C_Config;
}I2C_Handler_t;

/**************************************************************
 *  Конфігурайція I2C_SCLKSpeed 
***************************************************************/
#define I2C_SCLK_Speed_SM              100000                   //Стандартна швидкість
#define I2C_SCLK_Speed_FM4K            400000                   //Швидкість 4К
#define I2C_SCLK_Speed_FM2K            200000                   //Швидкість 2К

/**************************************************************
 *  Конфігурайція I2C_ACKControl
***************************************************************/
#define I2C_ACK_ENABLE                 1
#define I2C_ACK_DESABLE                0
    
/**************************************************************
 *  Конфігурайція I2C_FMDutyCycle
***************************************************************/
#define I2C_FM_Duty_2                  0
#define I2C_FM_Duty_16_9               1

/****************************************************
 * APIs підтримка драйвер протів I2Cx
 ***************************************************
*/

/*
 * Налаштування переферійного годинника I2C
 */
void I2C_PerClockControl(I2C_TypeDef *pSPIx, uint8_t EnorDi);

/*
 * Ініціалізація та Деініцілізація SPI
 */
void I2C_Init(I2C_Handler_t *pI2CHandler);
void I2C_DeInit(I2C_TypeDef *pI2Cx);

/*
 * Передача і запис даних SPI
 */


/*
 * Переривання SPI ISR
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnonDi);
void I2C_IRQPriorConfig(uint32_t IRQPriorty,uint8_t IRQNumber);


/*
 *Інші переферійні ІРА
 */
void I2C_PeripheralControl(I2C_TypeDef *pI2Cx, uint8_t EnorDi);

void I2C_SSOEConfig(I2C_TypeDef *pI2Cx, uint8_t EnorDi);
uint8_t I2C_GetFlag(I2C_TypeDef *pI2Cx, uint32_t FlagName);

void I2C_ApplicationEventCallback(I2C_Handler_t *pI2CHandler, uint8_t AppEv);

#endif /* INC_STM32F4XX_I2C_DRIVER_H_ */
