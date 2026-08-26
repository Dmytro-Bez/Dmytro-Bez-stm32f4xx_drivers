#include <stm32f4xx_spi_driver.h>
#include <stddef.h>

/********************************************************************
 * @fn			- SPI_PerClockControl
 *
 * @brief		- Ця функція дозвляє налаштувати переферійний годинник SPI
 *
 * @param[in]	- Базова адреса перефірійного пристрою pSPIx
 * @param[in]	- Дозвіл чи не дозвіл макроса(...)
 * @param[in]	-
 *
 * @return		- none
 *
 * @Note		- none
 */
void SPI_PerClockControl(SPI_TypeDef *pSPIx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(pSPIx == SPI1){
			SPI1_PCLK_EN();
		} else if(pSPIx == SPI2){
			SPI2_PCLK_EN();
		} else if(pSPIx == SPI3){
			SPI3_PCLK_EN();
		}
	}
	else{
		//Too
	}
}

/********************************************************************
 * @fn			- SPI_Init
 *
 * @brief		- Ця функція дозвляє налаштувати іціалізацію SPI
 *
 * @param[in]	- Базова адреса перефірійного пристрою pSPIx
 * @param[in]	-
 * @param[in]	-
 *
 * @return		- none
 *
 * @Note		- none
 */
void SPI_Init(SPI_Handler_t *pSPIHandler){

	uint32_t temp = 0;

	//Дозволити використання переферійного годинника для SPI
	SPI_PerClockControl(pSPIHandler->pSPIx, ENABLE);

	//1. Налаштування  SPIConfig
	temp |= pSPIHandler->SPIConfig.SPI_DeviceMode << 2;									//Дозвіл на роботу
	//2. Налаштування SPI_BusConfig
	if(pSPIHandler->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD){
		//BIDI тип на очистку
		temp &= ~(1 << 15);																//Тут змінено на мвкрос для зручності
	} else if(pSPIHandler->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD){
		//BIDI тип на отримання
		temp |= (1 << 15);
	} else if(pSPIHandler->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY){
		//BIDI тип на RX
		temp &= ~(1 << 15);
		temp |= (1 << 10);
	}
	//3. Налаштування  SPI_SCLSpeed
	temp |= pSPIHandler->SPIConfig.SPI_SCLSpeed << SPI_CR1_BR;
	//4. Налаштування  SPI_DFF
	temp |= pSPIHandler->SPIConfig.SPI_DFF << SPI_CR1_DFF;
	//5. Налаштування  SPI_CPOL
	temp |= pSPIHandler->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;
	//6. Налаштування  SPI_CPHA
	temp |= pSPIHandler->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;
	pSPIHandler->pSPIx ->CR1 = temp;

	//7. Налаштування  SPI_SSM


}

uint8_t SPI_GetFlagStatus(SPI_TypeDef *pSPIx, uint32_t FlagName){
			if(pSPIx->SR & FlagName){
				return FLAG_SET;
			}
	return FLAG_RESET;
}
/********************************************************************
 * @fn			- SPI_DeInit
 *
 * @brief		- Ця функція дозвляє налаштувати деіціалізацію SPI
 *
 * @param[in]	- Базова адреса перефірійного пристрою pSPIx
 * @param[in]	-
 * @param[in]	-
 *
 * @return		- none
 *
 * @Note		- none
 */
void SPI_DeInit(SPI_TypeDef *pSPIx){

}

/********************************************************************
 * @fn			- SPI_SendDate
 *
 * @brief		- Ця функція дозвляє налаштувати отримання даних по SPI
 *
 * @param[in]	- Базова адреса перефірійного пристрою pSPIx
 * @param[in]	- Буфер даних
 * @param[in]	- Довжина даних
 *
 * @return		- none
 *
 * @Note		- Можливість блокувати передачу
 */
void SPI_SendDate(SPI_TypeDef *pSPIx, uint8_t *pTxBuffer, uint32_t Len){
	while(Len > 0){
		//1. Чекаємо ТХЕ пердачу
		// while(!(pSPIx->SR & (1 << 1)));
		while(SPI_GetFlagStatus(pSPIx,SPI_TXE_FLAG) == FLAG_RESET);

		//2. Перевіряємо біт DFF в CR1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF)){
			//16 bit DFF
			//1. Завантаження  даних в DR
			pSPIx->DR = *((uint16_t*)pTxBuffer);
			Len--;
			Len--;
			(uint16_t*)pTxBuffer++;
		}else{
			//8 bit DFF
			pSPIx->DR = *pTxBuffer;
			Len--;
			pTxBuffer++;
		}
	}
}

/********************************************************************
 * @fn			- SPI_SPI_RecevieDate
 *
 * @brief		- Ця функція дозвляє налаштувати передачу даних по SPI
 *
 * @param[in]	- Базова адреса перефірійного пристрою pSPIx
 * @param[in]	- Буфер даних
 * @param[in]	- Довжина даних
 *
 * @return		- none
 *
 * @Note		- none
 */
void SPI_RecevieDate(SPI_TypeDef *pSPIx, uint8_t *pRxBuffer, uint32_t Len){
	while(Len > 0){
		//1. Чекаємо RХЕ пердачу
		// while(!(pSPIx->SR & (1 << 1)));
		while(SPI_GetFlagStatus(pSPIx,SPI_RXE_FLAG) == FLAG_RESET);

		//2. Перевіряємо біт DFF в CR1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF)){
			//16 bit DFF
			//1. Завантаження  даних в DR в RXE буфкр адреси
			*((uint16_t*)pRxBuffer) = pSPIx->DR;
			Len--;
			Len--;
			(uint16_t*)pRxBuffer++;
		}else{
			//8 bit DFF
			*(pRxBuffer) = pSPIx->DR;
			Len--;
			pRxBuffer++;
		}
	}
}

/********************************************************************
 * @fn			- SPI_IRQConfig
 *
 * @brief		- Ця функція дозвляє налаштувати преривання SPI
 *
 * @param[in]	- Номер перериваня
 * @param[in]	- Дозвіл на пеперивання
 * @param[in]	-
 *
 * @return		- none
 *
 * @Note		- none
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnonDi){

}

/********************************************************************
 * @fn			- SPI_IRQHanling
 *
 * @brief		- Ця функція дозвляє викликати преривання
 *
 * @param[in]	- Структура переферії SPI
 * @param[in]	-
 * @param[in]	-
 *
 * @return		- none
 *
 * @Note		- none
 */
__weak void SPI_ApplicationEventCallback(SPI_Handler_t *pSPIHandler, uint8_t AppEv){

}

static void spi_ovr_err_interrupt_handler(SPI_Handler_t *pSPIHandler){
	uint8_t temp;

	if(pSPIHandler->TxState != SPI_BUSY_IN_TX){
		temp = pSPIHandler->pSPIx->DR;
		temp = pSPIHandler->pSPIx->SR;
	}
	(void)temp;
	SPI_ApplicationEventCallback(pSPIHandler, SPI_EVENT_OVR_ERR);
}

static void spi_rxe_interrupt_handler(SPI_Handler_t *pSPIHandler){
	if(pSPIHandler->pSPIx->CR1 & ( 1 << 11))
	{
		//16 bit
		*((uint16_t*)pSPIHandler->pRxBuffer) = (uint16_t) pSPIHandler->pSPIx->DR;
		pSPIHandler->RxLen -= 2;
		pSPIHandler->pRxBuffer++;
		pSPIHandler->pRxBuffer++;

	}else
	{
		//8 bit
		*(pSPIHandler->pRxBuffer) = (uint8_t) pSPIHandler->pSPIx->DR;
		pSPIHandler->RxLen--;
		pSPIHandler->pRxBuffer++;
	}

	if(! pSPIHandler->RxLen)
	{
		//reception is complete
		SPI_CloseReception(pSPIHandler);
		SPI_ApplicationEventCallback(pSPIHandler,SPI_EVENT_RX_CMPLT);
	}
}

static void spi_txe_interrupt_handler(SPI_Handler_t *pSPIHandler){
	if(pSPIHandler->pSPIx->CR1 & (1 << SPI_CR1_DFF)){
			//16 bit DFF
			//1. Завантаження  даних в DR
			pSPIHandler->pSPIx->DR = *((uint16_t*)pSPIHandler->pTxBuffer);
			pSPIHandler->TxLen--;
			pSPIHandler->TxLen--;
			(uint16_t*)pSPIHandler->pTxBuffer++;
		}else{
			//8 bit DFF
			pSPIHandler->pSPIx->DR = *pSPIHandler->pTxBuffer;
			pSPIHandler->TxLen--;
			pSPIHandler->pTxBuffer++;
		}

		if(! pSPIHandler->TxLen){
			//TX є zero(0) тоді зупиняємо передачу даних
			SPI_CloseTransmisson(pSPIHandler);
			SPI_ApplicationEventCallback(pSPIHandler, SPI_EVENT_TX_CMPLT);
		}
}

void SPI_ClearOVRFlag(SPI_TypeDef *pSPIx){
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void) temp;
}

void SPI_CloseTransmisson(SPI_Handler_t *pSPIHandler){
	if(! pSPIHandler->TxLen){
		//TX є zero(0) тоді зупиняємо передачу даних
		pSPIHandler->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
		pSPIHandler->pTxBuffer = NULL;
		pSPIHandler->TxLen = 0;
		pSPIHandler->TxState = SPI_READ;
	}
}

void SPI_CloseReception(SPI_Handler_t *pSPIHandler){
	pSPIHandler->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
	pSPIHandler->pRxBuffer = NULL;
	pSPIHandler->RxLen = 0;
	pSPIHandler->RxState = SPI_READ;
}

void SPI_IRQHanling(SPI_Handler_t *pSPIHandler){
	uint8_t temp1, temp2;
	temp1 = pSPIHandler->pSPIx->SR & (1 << SPI_SR_TXE);
	temp2 = pSPIHandler->pSPIx->CR2 & (1 << SPI_CR2_TXEIE);
	if(temp1 && temp2){
		//Обробка ТХЕ
		spi_txe_interrupt_handler(pSPIHandler);
	}

	temp1 = pSPIHandler->pSPIx->SR & (1 << SPI_SR_RXNE);
	temp2 = pSPIHandler->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE);
	if(temp1 && temp2){
		//Обробка ТХЕ
		spi_rxe_interrupt_handler(pSPIHandler);  
	}

	temp1 = pSPIHandler->pSPIx->SR & (1 << SPI_SR_OVR);
	temp2 = pSPIHandler->pSPIx->CR2 & (1 << SPI_CR2_ERRIE);
	if(temp1 && temp2){
		//Обробка ТХЕ
		spi_ovr_err_interrupt_handler(pSPIHandler);
	}
}

/********************************************************************
 * @fn			- SPI_IRQPrior
 *
 * @brief		- Ця функція дозвляє встановити пріоріетет преривання
 *
 * @param[in]	- Використання пріоретету
 * @param[in]	- Використання номер преривання
 * @param[in]	-
 *
 * @return		- none
 *
 * @Note		- none
 */
void SPI_IRQPriorConfig(uint32_t IRQNumber,uint8_t IRQPriority){

}

uint8_t SPI_SendDateIT(SPI_Handler_t *pSPIHand, uint8_t *pTxBuffer, uint32_t Len){
	uint8_t state = pSPIHand->TxState;

	if(state != SPI_BUSY_IN_TX){
		// 1.Зберігаємо дані про адресу і довжину буфера ТХ
		pSPIHand->pTxBuffer = pTxBuffer;
		pSPIHand->TxLen = Len;

		// 2.Встановлюємо значення,що ТХ зайнятий
		pSPIHand->TxState = SPI_BUSY_IN_TX;

		// 3.Встановлюємо прапор ТХЕ в SR
		pSPIHand->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);

		// 4.Передачаю даних далі займеться ISR
	}

	return state;
}

uint8_t SPI_RecevieDateIT(SPI_Handler_t *pSPIHand, uint8_t *pRxBuffer, uint32_t Len){
	uint8_t state = pSPIHand->RxState;
	
	if(state != SPI_BUSY_IN_RX){
		// 1.Зберігаємо дані про адресу і довжину буфера ТХ
		pSPIHand->pRxBuffer = pRxBuffer;
		pSPIHand->RxLen = Len;

		// 2.Встановлюємо значення,що ТХ зайнятий
		pSPIHand->RxState = SPI_BUSY_IN_RX;

		// 3.Встановлюємо прапор ТХЕ в SR
		pSPIHand->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);

		// 4.Передачаю даних далі займеться ISR
	}

	return state;
}

/********************************************************************
 * @fn			- SPI_PeripheralControl
 *
 * @brief		- Ця функція дозвляє 
 *
 * @param[in]	- Використання пріоретету
 * @param[in]	- Використання номер преривання
 * @param[in]	-
 *
 * @return		- none
 *
 * @Note		- none
 */
void SPI_PeripheralControl(SPI_TypeDef *pSPIx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	}
	else{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}

/********************************************************************
 * @fn			- SPI_SSIConfig
 *
 * @brief		- Ця функція дозвляє 
 *
 * @param[in]	- Використання пріоретету
 * @param[in]	- Використання номер преривання
 * @param[in]	-
 *
 * @return		- none
 *
 * @Note		- none
 */
void SPI_SSIConfig(SPI_TypeDef *pSPIx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	}
	else{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
	}
}

/********************************************************************
 * @fn			- SPI_SSOEConfig
 *
 * @brief		- Ця функція дозвляє 
 *
 * @param[in]	- Використання пріоретету
 * @param[in]	- Використання номер преривання
 * @param[in]	-
 *
 * @return		- none
 *
 * @Note		- none
 */
void SPI_SSOEConfig(SPI_TypeDef *pSPIx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		pSPIx->CR2 |= (1 << SPI_CR2_SSOE);
	}
	else{
		pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE);
	}
}
