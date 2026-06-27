#include <stm32f4xx_spi_driver.h>

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
void SPI_SPI_RecevieDate(SPI_TypeDef *pSPIx, uint8_t *pRxBuffer, uint32_t Len){
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
void SPI_IRQConfig(uint8_t IRQNumber,uint8_t EnonDi){

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
void SPI_IRQHanling(SPI_Handler_t *pHandler){

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
void SPI_IRQPriority(uint32_t IRQNumber,uint8_t IRQPriority){

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
