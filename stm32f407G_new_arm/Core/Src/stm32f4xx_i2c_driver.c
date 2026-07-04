#include"stm32f4xx_i2c_driver.h"

uint16_t AHB_PreScaler[8] = {2, 4, 8, 16, 64, 128, 256, 512};
uint16_t APB1_PreScaler[4] = {2, 4, 8, 16};

static uint32_t RCC_GetPLLOutputClock(){
//    	pI2Cx->CR1 |= ( 1 << I2C_CR1_STOP);
}

static uint32_t RCC_GETPCLK1Value(void){
    uint32_t pclk1, SystemCLK;

    uint8_t clksrc, temp, ahbp, apb1p;
    clksrc = ((RCC->CFGR >> 2) & 0x3);
    if(clksrc == 0){
        SystemCLK = 16000000;
    } else if(clksrc == 1){
        SystemCLK = 8000000;
    } else if(clksrc == 2){
        SystemCLK = RCC_GetPLLOutputClock();
    }
    //ahb
    temp = ((RCC->CFGR >> 4) & 0xF);
    if(temp < 8){
        ahbp = 1;
    } else {
        ahbp = AHB_PreScaler[temp - 8];
    }
    //apb1
     temp = ((RCC->CFGR >> 10) & 0x7);
    if(temp < 4){
        apb1p = 1;
    } else {
        apb1p = APB1_PreScaler[temp - 4];
    }

    pclk1 = (SystemCLK / ahbp) / apb1p;
    return pclk1;
}
/********************************************************************
 * @fn			- I2C_PerClockControl
 *
 * @brief		- Ця функція дозвляє налаштувати переферійний годинник I2C
 *
 * @param[in]	- Базова адреса перефірійного пристрою pI2Cx
 * @param[in]	- Дозвіл чи не дозвіл макроса(...)
 * @param[in]	-
 *
 * @return		- none
 *
 * @Note		- none
 */
void I2C_PerClockControl(I2C_TypeDef *pI2Cx, uint8_t EnorDi){
    
}

/********************************************************************
 * @fn			- I2C_Init
 *
 * @brief		- Ця функція дозвляє налаштувати іціалізацію I2C
 *
 * @param[in]	- Базова адреса перефірійного пристрою pI2CHandler
 * @param[in]	-
 * @param[in]	-
 *
 * @return		- none
 *
 * @Note		- none
 */
void I2C_Init(I2C_Handler_t *pI2CHandler){
    uint32_t temp = 0;

    //ack bit
    temp |= pI2CHandler->pI2C_Config->I2C_ACKControl << 10;
    pI2CHandler ->pI2Cx->CR1 = temp;

    //FREQ i CR2
    temp = 0;
    temp |= RCC_GETPCLK1Value() / 1000000U;
    pI2CHandler->pI2Cx->CR2 = (temp & 0x3F);

    //Адреса підлеглого пристрою
    temp |= pI2CHandler->pI2C_Config->I2C_DeviceAddress << 1;
    temp |= (1 << 14);
    pI2CHandler->pI2Cx->OAR1 = temp;

    //CCR
    temp = 0;
    uint16_t ccr_value;
    if(pI2CHandler->pI2C_Config->I2C_SCLKSpeed <= I2C_SCLK_Speed_SM){
        //Стандвртний мод
        ccr_value = RCC_GETPCLK1Value() / (2 * pI2CHandler->pI2C_Config->I2C_SCLKSpeed);
        temp |= (ccr_value & 0xFFF); 
    } else {
        //Швидкий мод
        temp |= (1 << 15);
        temp |= (pI2CHandler->pI2C_Config->I2C_FMDutyCycle << 14);

        if(pI2CHandler->pI2C_Config->I2C_FMDutyCycle == I2C_FM_Duty_2){
            ccr_value = RCC_GETPCLK1Value() / (3 * pI2CHandler->pI2C_Config->I2C_SCLKSpeed);
        } else{
            ccr_value = RCC_GETPCLK1Value() / (25 *pI2CHandler->pI2C_Config->I2C_SCLKSpeed);
        }
        temp |= (ccr_value & 0xFFF); 
    }
    pI2CHandler->pI2Cx->CCR = temp;
}

/********************************************************************
 * @fn			- I2C_DeInit
 *
 * @brief		- Ця функція дозвляє налаштувати деіціалізацію I2C
 *
 * @param[in]	- Базова адреса перефірійного пристрою pI2Cx
 * @param[in]	-
 * @param[in]	-
 *
 * @return		- none
 *
 * @Note		- none
 */
void I2C_DeInit(I2C_TypeDef *pI2Cx){

}

/********************************************************************
 * @fn			- I2C_MasterSendDate
 *
 * @brief		- Ця функція дозвляє почати передачу даних МАЙСИРА I2C
 *
 * @param[in]	- Базова адреса перефірійного пристрою pI2CHandler
 * @param[in]	- Буфер передачі
 * @param[in]	- Довжина передачі даних
 * @param[in]	- Адресва підлеглого
 * 
 * @return		- none
 *
 * @Note		- none
 */

static void I2C_GenerateStartCondition(I2C_TypeDef *pI2Cx){
    pI2Cx->CR1 |= (1 << I2C_CR1_START);
}

void I2C_MasterSendDate(I2C_Handler_t *pI2CHandler, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlavAdd){
    // 1. Генерація СТАРТУ
    I2C_GenerateStartCondition(pI2CHandler->pI2Cx);

    // 2. Перевірка прапору SB
    while (I2C_GetFlag(pI2CHandler->pI2Cx, I2C_FLAG_SB))
    {
        /* code */
    }
    
}
