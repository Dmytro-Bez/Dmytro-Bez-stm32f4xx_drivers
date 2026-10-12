#include"stm32f4xx_i2c_driver.h"
#include"stm32f4xx_rcc_driver.h"

uint16_t AHB_PreScaler_i2c[8] = {2, 4, 8, 16, 64, 128, 256, 512};
uint16_t APB1_PreScaler_i2c[4] = {2, 4, 8, 16};

void I2C_CloseReceiveData(I2C_Handler_t *pI2CHandler){
    //Implement the code to disable ITBUFEN Control Bit
	pI2CHandler->pI2Cx->CR2 &= ~( 1 << I2C_CR2_ITBUFEN);

	//Implement the code to disable ITEVFEN Control Bit
	pI2CHandler->pI2Cx->CR2 &= ~( 1 << I2C_CR2_ITEVTEN);

	pI2CHandler->TxRxState = I2C_READ;
	pI2CHandler->pRxBuffer = NULL;
	pI2CHandler->RxLen = 0;
	pI2CHandler->RxSize = 0;

	if(pI2CHandler->pI2C_Config->I2C_ACKControl == I2C_ACK_ENABLE)
	{
		I2C_ManageAcking(pI2CHandler->pI2Cx,ENABLE);
	}
}

void I2C_CloseSendDate(I2C_Handler_t *pI2CHandler){
    pI2CHandler->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);
    pI2CHandler->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);

    pI2CHandler->TxRxState = I2C_READ;
    pI2CHandler->pTxBuffer = NULL;
    pI2CHandler->RxLen = 0;
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
        ahbp = AHB_PreScaler_i2c[temp - 8];
    }
    //apb1
     temp = ((RCC->CFGR >> 10) & 0x7);
    if(temp < 4){
        apb1p = 1;
    } else {
        apb1p = APB1_PreScaler_i2c[temp - 4];
    }

    pclk1 = (SystemCLK / ahbp) / apb1p;
    return pclk1;
}
/********************************************************************
 * @fn			- I2C_PeripheralControl
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
void I2C_PeripheralControl(I2C_TypeDef *pI2Cx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
			pI2Cx->CR1 |= (1 << I2C_CR1_PE);
			//pI2cBaseAddress->CR1 |= I2C_CR1_PE_Bit_Mask;
	}else {
		pI2Cx->CR1 &= ~(1 << 0);
	}
}

void I2C_PeriClockControl(I2C_TypeDef *pI2Cx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(pI2Cx == I2C1){
			I2C1_PCLK_EN();
		}else if (pI2Cx == I2C2){
			I2C2_PCLK_EN();
		}else if (pI2Cx == I2C3){
			I2C3_PCLK_EN();
		}
	} else {
	}
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

    I2C_PeriClockControl(pI2CHandler->pI2Cx,ENABLE);
    //ack bit
    temp |= pI2CHandler->pI2C_Config->I2C_ACKControl << 10;
    pI2CHandler->pI2Cx->CR1 = temp;

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

    //TRISE налаштування
     if(pI2CHandler->pI2C_Config->I2C_SCLKSpeed <= I2C_SCLK_Speed_SM){
        //Стандвртний мод
       temp = (RCC_GETPCLK1Value() / 1000000U) + 1;
    } else {
        //Швидкий мод
        temp = ((RCC_GETPCLK1Value() / 300) / 1000000000U) + 1;
    }
    pI2CHandler->pI2Cx->TRISE = (temp & 0x3F);
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

uint8_t I2C_GetFlag(I2C_TypeDef *pI2Cx, uint32_t FlagName){
    if(pI2Cx->SR1 & FlagName){
        return FLAG_SET;
    }
    return FLAG_RESET;
}
static void  I2C_ExecuteAddressPhaseWrite(I2C_TypeDef *pI2Cx, uint8_t SlavAdd){
    SlavAdd = SlavAdd << 1;
    SlavAdd &= ~(1);
    pI2Cx->DR = SlavAdd;
}

static void I2C_ClearADDRFlag(I2C_Handler_t *pI2CHandler){
    uint32_t dummy_read;
    if(pI2CHandler->pI2Cx->SR2 & (1 << I2C_SR2_MSL)){
        if(pI2CHandler->TxRxState == I2C_BUSY_IN_RX){
            if(pI2CHandler->RxSize == 1){
                I2C_ManageAcking(pI2CHandler->pI2Cx,DISEBALE);

                dummy_read = pI2CHandler->pI2Cx->SR1;
                dummy_read = pI2CHandler->pI2Cx->SR2;
                (void)dummy_read;
            }
        } else {
            dummy_read = pI2CHandler->pI2Cx->SR1;
            dummy_read = pI2CHandler->pI2Cx->SR2;
                (void)dummy_read;
        }
    } else {
        dummy_read = pI2CHandler->pI2Cx->SR1;
        dummy_read = pI2CHandler->pI2Cx->SR2;
        (void)dummy_read;
    }
}

static void I2C_GenerateSopCandition(I2C_TypeDef *pI2Cx){
    pI2Cx->CR1 |= (1 << I2C_CR1_STOP);
}

void I2C_MasterSendDate(I2C_Handler_t *pI2CHandler, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlavAdd,uint8_t Sr){
    // 1. Генерація СТАРТУ
    I2C_GenerateStartCondition(pI2CHandler->pI2Cx);

    // 2. Перевірка прапору SB
    while ( ! I2C_GetFlag(pI2CHandler->pI2Cx, I2C_FLAG_SB));

    // 3. Відправка 8 бітів даних підлеглого
    I2C_ExecuteAddressPhaseWrite(pI2CHandler->pI2Cx, SlavAdd);

    //4. ADDR прапор SR1
    while ( ! I2C_GetFlag(pI2CHandler->pI2Cx, I2C_FLAG_ADDR));

    //5. Очищення флагу ADDR
    I2C_ClearADDRFlag(pI2CHandler);

    // 6.Передача даних поки довжиною  не стане(0)
    while (Len > 0)
    {
        while ( ! I2C_GetFlag(pI2CHandler->pI2Cx, I2C_FLAG_TXE));            //Чи встановлений TXE
        pI2CHandler->pI2Cx->DR = *pTxBuffer;
        pTxBuffer++;    
        Len--;
    }

    // 7. Коли довдмна стає (0), чекаємо TXE=1 i BTF=1, першніж згенерувати STOP
    while ( ! I2C_GetFlag(pI2CHandler->pI2Cx, I2C_FLAG_TXE)); 
    while ( ! I2C_GetFlag(pI2CHandler->pI2Cx, I2C_FLAG_BTF)); 

    // 8. Генерація STOP
    if(Sr == I2C_DISABLE_SR )
        I2C_GenerateSopCandition(pI2CHandler->pI2Cx);
}

static void  I2C_ExecuteAddressPhaseRead(I2C_TypeDef *pI2Cx, uint8_t SlavAdd){
    SlavAdd = SlavAdd << 1;
    SlavAdd |= 1;
    pI2Cx->DR = SlavAdd;
}

void I2C_ManageAcking(I2C_TypeDef *pI2Cx, uint8_t EnorDi){
    if(EnorDi == I2C_ACK_ENABLE){
        //Дозвіл на АСК
        pI2Cx->CR1 |= (1 << I2C_CR1_ACK);
    } else {
        //Не доозвіл на АСК
        pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK);
    }
}

void I2C_MasterReceiveData(I2C_Handler_t *pI2CHandler, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlavAdd, uint8_t Sr){
    // 1. Генерація біта Старту
        I2C_GenerateStartCondition(pI2CHandler->pI2Cx);
    // 2. Налаштування біта SB прапора
        while ( ! I2C_GetFlag(pI2CHandler->pI2Cx, I2C_FLAG_SB));
    // 3. Передача адресу підлеглого на запис чи читання(8 bit)
        I2C_ExecuteAddressPhaseRead(pI2CHandler->pI2Cx, SlavAdd);
    // 4. Чекаємо фазу адресного прапора
        while ( ! I2C_GetFlag(pI2CHandler->pI2Cx, I2C_FLAG_ADDR));

    //Читання 1 біта підлеглого
    if(Len == 1){
        // Не має дозволу на ASKing
        I2C_ManageAcking(pI2CHandler->pI2Cx,I2C_ACK_DESABLE);
        // Генерація зупинки
        I2C_GenerateSopCandition(pI2CHandler->pI2Cx);
        // Очистка прапора ADDR
        I2C_ClearADDRFlag(pI2CHandler);
        // Чекаємо RXNE в 1
        while ( ! I2C_GetFlag(pI2CHandler->pI2Cx, I2C_FLAG_RXNE));

        if(Sr == I2C_DISABLE_SR )
        	I2C_GenerateSopCandition(pI2CHandler->pI2Cx);
        // Читаємо данні з буфера
        *pRxBuffer = pI2CHandler->pI2Cx->DR;

        return;
    }

    if(Len > 1){
        // Очистка прапора ADDR
        I2C_ClearADDRFlag(pI2CHandler);
        // ЧИтаємо данні Len з (0)
        for(uint32_t i = Len; i > 0; i--){
            // Чекаємо RXNE в 1
            while ( ! I2C_GetFlag(pI2CHandler->pI2Cx, I2C_FLAG_RXNE));
            if(i == 2){
                // Очистка ASKing
                I2C_ManageAcking(pI2CHandler->pI2Cx,I2C_ACK_DESABLE);
                // Генерація біта СТОП
                if(Sr == I2C_DISABLE_SR )
                	I2C_GenerateSopCandition(pI2CHandler->pI2Cx);
            }
            // Читаємо данні з буфера
            *pRxBuffer = pI2CHandler->pI2Cx->DR;
            // Інкремент(збільшення) буфера
            pRxBuffer++;
        }
    }
    // Повторюємо  дозвіл на ASKing
    if(pI2CHandler->pI2C_Config->I2C_ACKControl == I2C_ACK_ENABLE){
        I2C_ManageAcking(pI2CHandler->pI2Cx,I2C_ACK_ENABLE);
    }
}

/*********************************************************************
 * @fn      		  - I2C_MasterSendDataIT
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
uint8_t I2C_MasterSendDateIT(I2C_Handler_t *pI2CHandler, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlavAdd, uint8_t Sr){
    uint8_t busystate = pI2CHandler->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandler->pTxBuffer = pTxBuffer;
		pI2CHandler->TxLen = Len;
		pI2CHandler->TxRxState = I2C_BUSY_IN_TX;
		pI2CHandler->DevAddr = SlavAdd;
		pI2CHandler->SR = Sr;

		//Implement code to Generate START Condition
		I2C_GenerateSopCandition(pI2CHandler->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
		pI2CHandler->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		pI2CHandler->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2CHandler->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITERREN);
	}
    return busystate;
}

/*********************************************************************
 * @fn      		  - I2C_MasterReceiveDataIT
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
uint8_t I2C_MasterReceiveDataIT(I2C_Handler_t *pI2CHandler, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlavAdd){
//     uint8_t busystate = pI2CHandler->pI2CHandler;

//	if( (busystate != I2C_BUST_IN_TX) && (busystate != I2C_BUSY_IN_RX))
//	{
//		// pI2CHandler->pRxBuffer = ?;
//		// pI2CHandler->RxLen = ?;
//		// pI2CHandler->TxRxState = ?;
//		// pI2CHandler->RxSize = Len; //Rxsize is used in the ISR code to manage the data reception
//		// pI2CHandler->DevAddr = ?;
//		// pI2CHandler->Sr = Sr;
//
//		//Implement code to Generate START Condition
//
//
//		//Implement the code to enable ITBUFEN Control Bit
//
//
//		//Implement the code to enable ITEVFEN Control Bit
//
//
//		//Implement the code to enable ITERREN Control Bit
//
//	}

//	return busystate;
	return 0;
}

void I2C_EV_IRQHandling(I2C_Handler_t *pI2CHandler){
    //1. Перевірка наявність події SB
    uint32_t temp1,temp2,temp3;
    temp1 = pI2CHandler->pI2Cx->CR2 & (1 << I2C_CR2_ITEVTEN);
    temp2 = pI2CHandler->pI2Cx->CR2 & (1 << I2C_CR2_ITBUFEN);
    temp3 = pI2CHandler->pI2Cx->SR1 & (1 << I2C_SR1_SB);

    if(temp1 && temp3){
        //Встановлено прапор SB
        if(pI2CHandler->TxRxState == I2C_BUSY_IN_TX){
            I2C_ExecuteAddressPhaseWrite(pI2CHandler->pI2Cx,pI2CHandler->DevAddr);
        }else if(pI2CHandler->TxRxState == I2C_BUSY_IN_RX){
            I2C_ExecuteAddressPhaseRead(pI2CHandler->pI2Cx,pI2CHandler->DevAddr);
        }
    }

    //2. Піддтверджуємо генеацію ADDR
    temp3 = pI2CHandler->pI2Cx->SR1 & (1 << I2C_SR1_ADDR);
    if(temp1 && temp3){
        //Встановлено прапор ADDR
        I2C_ClearADDRFlag(pI2CHandler);
    }

    //3. Перевіремо встановлення BTF
    temp3 = pI2CHandler->pI2Cx->SR1 & (1 << I2C_SR1_BTF);
    if(temp1 && temp3){
        //Встановлено прапор BTF
        if(pI2CHandler->TxRxState == I2C_BUSY_IN_TX){
            if(pI2CHandler->pI2Cx->SR1 & (1 << I2C_SR1_TXE)){
                if(pI2CHandler->TxLen == 0){
                //TXE (1)
                if(pI2CHandler->SR == I2C_DISABLE_SR)
                    I2C_GenerateSopCandition(pI2CHandler->pI2Cx);

                I2C_CloseSendDate(pI2CHandler);
                I2C_ApplicationEventCallback(pI2CHandler, I2C_EV_TX_CMPLT);
                }
            }
        } else if(pI2CHandler->TxRxState == I2C_BUSY_IN_RX){
            //
        }
    }

    //4. Перевіремо встановлення STOPF
    temp3 = pI2CHandler->pI2Cx->SR1 & (1 << I2C_SR1_STOPF);
    if(temp1 && temp3){
        //Встановлено прапор STOPF
        pI2CHandler->pI2Cx->CR1 |= 0x0000;
        I2C_ApplicationEventCallback(pI2CHandler, I2C_EV_STOP);
    }

    //5. Перевіремо встановлення TXE
    temp3 = pI2CHandler->pI2Cx->SR1 & (1 << I2C_SR1_TXE);
    if(temp1 && temp2 && temp3){
        if(pI2CHandler->pI2Cx->SR2 & (1 << I2C_SR2_MSL)){
        //Встановлено прапор TXE
            if(pI2CHandler->TxRxState == I2C_BUSY_IN_TX){
                if(pI2CHandler->TxLen > 0){
                    pI2CHandler->pI2Cx->DR = *(pI2CHandler->pTxBuffer);
                    pI2CHandler->TxLen--;
                    pI2CHandler->pTxBuffer++;
                } else {
                    //Робота пілеглого
                    if(pI2CHandler->pI2Cx->SR2 & (1 << I2C_SR2_TRA)){
                        I2C_ApplicationEventCallback(pI2CHandler, I2C_EV_DATA_REQ);
                    }
                }
            }
        }
    }

    //6. Перевіремо встановлення RXNE
    temp3 = pI2CHandler->pI2Cx->SR1 & (1 << I2C_SR1_RXNE);
    if(temp1 && temp2 && temp3){
        //Встановлено прапор RXNE
        if(pI2CHandler->pI2Cx->SR2 & (1 << I2C_SR2_MSL)){
            if(pI2CHandler->TxRxState == I2C_BUSY_IN_RX){
                if(pI2CHandler->RxSize == 1){
                    *pI2CHandler->pRxBuffer = pI2CHandler->pI2Cx->DR;
                    pI2CHandler->RxLen--;
                }
                if(pI2CHandler->RxSize > 1){
                    if(pI2CHandler->RxSize == 2){
                        I2C_ManageAcking(pI2CHandler->pI2Cx,DISEBALE);
                    }
                    *pI2CHandler->pRxBuffer = pI2CHandler->pI2Cx->DR;
                    pI2CHandler->pRxBuffer++;
                    pI2CHandler->RxLen--;
                }
                if(pI2CHandler->RxLen == 0){
                    if(pI2CHandler->SR == I2C_DISABLE_SR)
                        I2C_GenerateSopCandition(pI2CHandler->pI2Cx);
                        I2C_CloseReceiveData(pI2CHandler);
                        I2C_ApplicationEventCallback(pI2CHandler,I2C_EV_RX_CMPLT);
                }
            } else {
                //Підлеглий
                if(pI2CHandler->pI2Cx->SR2 & (1 << I2C_SR2_TRA)){
                        I2C_ApplicationEventCallback(pI2CHandler, I2C_EV_DATA_RCV);
                }
            }
        }
    }
}

/*********************************************************************
 * @fn      		  - I2C_ER_IRQHandling
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -
 */

void I2C_ER_IRQHandling(I2C_Handler_t *pI2CHandle)
{

	uint32_t temp1,temp2;

    //Know the status of  ITERREN control bit in the CR2
	temp2 = (pI2CHandle->pI2Cx->CR2) & ( 1 << I2C_CR2_ITERREN);


/***********************Check for Bus error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1<< I2C_SR1_BERR);
	if(temp1  && temp2 )
	{
		//This is Bus error

		//Implement the code to clear the buss error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_BERR);

		//Implement the code to notify the application about the error
	   I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_BERR);
	}

/***********************Check for arbitration lost error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_ARLO );
	if(temp1  && temp2)
	{
		//This is arbitration lost error

		//Implement the code to clear the arbitration lost error flag

		//Implement the code to notify the application about the error

	}

/***********************Check for ACK failure  error************************************/

	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_AF);
	if(temp1  && temp2)
	{
		//This is ACK failure error

	    //Implement the code to clear the ACK failure error flag

		//Implement the code to notify the application about the error
	}

/***********************Check for Overrun/underrun error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_OVR);
	if(temp1  && temp2)
	{
		//This is Overrun/underrun

	    //Implement the code to clear the Overrun/underrun error flag

		//Implement the code to notify the application about the error
	}

/***********************Check for Time out error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_TIMEOUT);
	if(temp1  && temp2)
	{
		//This is Time out error

	    //Implement the code to clear the Time out error flag

		//Implement the code to notify the application about the error
	}

}

void I2C_SlaveSendDate(I2C_TypeDef *pI2C, uint8_t date){
    pI2C->DR = date;
}

uint8_t I2C_SlaveReceiveData(I2C_TypeDef *pI2C){
    return 0;
}
void I2C_SlaveEnableDisableCallbackEvents(I2C_TypeDef *pI2Cx,uint8_t EnorDi){
    if(EnorDi == ENABLE)
	 {
			pI2Cx->CR2 |= ( 1 << I2C_CR2_ITEVTEN);
			pI2Cx->CR2 |= ( 1 << I2C_CR2_ITBUFEN);
			pI2Cx->CR2 |= ( 1 << I2C_CR2_ITERREN);
	 }else {
			pI2Cx->CR2 &= ~( 1 << I2C_CR2_ITEVTEN);
			pI2Cx->CR2 &= ~( 1 << I2C_CR2_ITBUFEN);
			pI2Cx->CR2 &= ~( 1 << I2C_CR2_ITERREN);
	 }
}

void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi){
    if(EnorDi == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			//program ISER0 register
			*NVIC_ISER0 |= ( 1 << IRQNumber );

		}else if(IRQNumber > 31 && IRQNumber < 64 ) //32 to 63
		{
			//program ISER1 register
			*NVIC_ISER1 |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 64 && IRQNumber < 96 )
		{
			//program ISER2 register //64 to 95
			*NVIC_ISER3 |= ( 1 << (IRQNumber % 64) );
		}
	}else
	{
		if(IRQNumber <= 31)
		{
			//program ICER0 register
			*NVIC_ICER0 |= ( 1 << IRQNumber );
		}else if(IRQNumber > 31 && IRQNumber < 64 )
		{
			//program ICER1 register
			*NVIC_ICER1 |= ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 6 && IRQNumber < 96 )
		{
			//program ICER2 register
			*NVIC_ICER3 |= ( 1 << (IRQNumber % 64) );
		}
	}
}
