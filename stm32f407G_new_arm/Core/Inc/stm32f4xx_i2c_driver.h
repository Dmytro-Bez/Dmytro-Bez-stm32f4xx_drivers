#ifndef INC_STM32F4XX_I2C_DRIVER_H_
#define INC_STM32F4XX_I2C_DRIVER_H_

#include"stm32f4xx.h"
#include<stdio.h>
#include<stm32f4xx_gpio_driver.h>

#define I2C_FLAG_SB					(1 << I2C_SR1_SB)
#define I2C_FLAG_TXE                (1 << I2C_SR1_TXE)
#define I2C_FLAG_RXNE               (1 << I2C_SR1_TXE)
#define I2C_FLAG_OVR                (1 << I2C_SR1_OVR)
#define I2C_FLAG_AF                 (1 << I2C_SR1_AF)
#define I2C_FLAG_ARLO               (1 << I2C_SR1_ARLO)
#define I2C_FLAG_BERR               (1 << I2C_SR1_BERR)
#define I2C_FLAG_STOPF              (1 << I2C_SR1_STOPF)
#define I2C_FLAG_ADD10              (1 << I2C_SR1_ADD10)
#define I2C_FLAG_BTF                (1 << I2C_SR1_BTF)
#define I2C_FLAG_ADDR               (1 << I2C_SR1_ADDR)
#define I2C_FLAG_TIMEOUT            (1 << I2C_SR1_TIMEOUT)

/**************************************************************
 *  Дерективи конролеру на дозвіл тактування І2С шини
***************************************************************/
#define I2C1_PCLK_EN()              (RCC->APB2ENR |= (1 << 21))
#define I2C2_PCLK_EN()              (RCC->APB2ENR |= (1 << 22))
#define I2C3_PCLK_EN()              (RCC->APB2ENR |= (1 << 23))

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
    uint8_t *pTxBuffer;                                     /* !< App Tx буфер адркси > */
    uint8_t *pRxBuffer;                                     /* !< App Rx буфер адркси > */
    uint32_t TxLen;                                         /* !< Tx довжина > */
    uint32_t RxLen;                                         /* !< Rx довжина > */
    uint8_t TxRxState;                                      /* !< Стан комуеікації > */
    uint8_t DevAddr;                                        /* !< Стан підлеглого пристрою > */
    uint32_t RxSize;                                        /* !< Розмір Rx > */
    uint8_t SR;                                             /* !< Повторення значення СТАРТУ > */
}I2C_Handler_t;

//I2C ствтус
#define I2C_READ                    0
#define I2C_BUSY_IN_RX              1
#define I2C_BUSY_IN_TX              2
/**************************************************************
 *  Конфігурайція I2C_SCLKSpeed 
***************************************************************/
#define I2C_SCLK_Speed_SM           100000                   //Стандартна швидкість
#define I2C_SCLK_Speed_FM4K         400000                   //Швидкість 4К
#define I2C_SCLK_Speed_FM2K         200000                   //Швидкість 2К

/**************************************************************
 *  Конфігурайція I2C_ACKControl
***************************************************************/
#define I2C_ACK_ENABLE              1
#define I2C_ACK_DESABLE             0
    
/**************************************************************
 *  Конфігурайція I2C_FMDutyCycle
***************************************************************/
#define I2C_FM_Duty_2               0
#define I2C_FM_Duty_16_9            1

/****************************************************
 * APIs підтримка драйвер протів I2Cx
 ***************************************************
*/

/*
 * Налаштування переферійного годинника I2C
 */
void I2C_PerClockControl(I2C_TypeDef *pSPIx, uint8_t EnorDi);

/*
 * Ініціалізація та Деініцілізація I2C
 */
void I2C_Init(I2C_Handler_t *pI2CHandler);
void I2C_DeInit(I2C_TypeDef *pI2Cx);

/*
* Функції підлеглого STMx
*/
void I2C_SlaveSendDate(I2C_TypeDef *pI2C, uint8_t date);
uint8_t I2C_SlaveReceiveData(I2C_TypeDef *pI2C);

/*
 * Передача і запис даних I2C
 */
void I2C_MasterSendDate(I2C_Handler_t *pI2CHandler, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlavAdd, uint8_t Sr);
void I2C_MasterReceiveData(I2C_Handler_t *pI2CHandler, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlavAdd, uint8_t Sr);
uint8_t I2C_MasterSendDateIT(I2C_Handler_t *pI2CHandler, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlavAdd, uint8_t Sr);
uint8_t I2C_MasterReceiveDataIT(I2C_Handler_t *pI2CHandler, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlavAdd);
/*
 * Переривання I2C ISR
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnonDi);
void I2C_IRQPriorConfig(uint32_t IRQPriorty,uint8_t IRQNumber);
void I2C_EV_IRQHandling(I2C_Handler_t *pI2CHandler);
void I2C_ERR_IRQHandling(I2C_Handler_t *pI2CHandler);

/*
 *Інші переферійні ІРА
 */
void I2C_PeripheralControl(I2C_TypeDef *pI2Cx, uint8_t EnorDi);
void I2C_ManageAcking(I2C_TypeDef *pSPIx, uint8_t EnorDi);

void I2C_SSOEConfig(I2C_TypeDef *pI2Cx, uint8_t EnorDi);
uint8_t I2C_GetFlag(I2C_TypeDef *pI2Cx, uint32_t FlagName);

void I2C_ApplicationEventCallback(I2C_Handler_t *pI2CHandler, uint8_t AppEv);

void I2C_CloseReceiveData(I2C_Handler_t *pI2CHandle);
void I2C_CloseSendDate(I2C_Handler_t *pI2CHandler);

#define I2C_DISABLE_SR  	RESET
#define I2C_ENABLE_SR   	SET

#define I2C_EV_TX_CMPLT  	 	0
#define I2C_EV_RX_CMPLT  	 	1
#define I2C_EV_STOP       		2
#define I2C_ERROR_BERR 	 		3
#define I2C_ERROR_ARLO  		4
#define I2C_ERROR_AF    		5
#define I2C_ERROR_OVR   		6
#define I2C_ERROR_TIMEOUT 		7
#define I2C_EV_DATA_REQ         8
#define I2C_EV_DATA_RCV         9

void I2C_CloseSendData(I2C_Handler_t *pI2CHandlr);
void I2C_SlaveEnableDisableCallbackEvents(I2C_TypeDef *pI2Cx,uint8_t EnorDi);
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);

#endif /* INC_STM32F4XX_I2C_DRIVER_H_ */
