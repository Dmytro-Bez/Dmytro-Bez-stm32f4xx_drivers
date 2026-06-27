#include"main.h"
#include"string.h"
/*
 * PB14 --> SP2_MISO
 * PB15 --> SP2_MOSI
 * PB13 --> SP2_SCL
 * PB12 --> SP2_NSS
 * ALF --> 5 mode
 */

void SPI2_GPIOInits(){
	GPIO_Handler_t SPIPins;
	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OPT_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	//SLC
	SPIPins.GPIO_PinConfig.GPIO_PinNummber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);
	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNummber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);
	//MISO
	SPIPins.GPIO_PinConfig.GPIO_PinNummber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);
	//NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNummber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);
}

void SPI2_Init(void){
	SPI_Handler_t SPI2handler;
	SPI2handler.pSPIx = SPI2;
	SPI2handler.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handler.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handler.SPIConfig.SPI_SCLSpeed = SPI_SCL_SPEED_DIV8;		//8MG
	SPI2handler.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handler.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handler.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handler.SPIConfig.SPI_SSM = SPI_SSM_DI;

	SPI_Init(&SPI2handler);
}

void GPIO_BUTTON(void){
    GPIO_Handler_t gpioButt;
    gpioButt.pGPIOx = GPIOA;
	gpioButt.GPIO_PinConfig.GPIO_PinNummber = GPIO_PIN_NO_0;
	gpioButt.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	gpioButt.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	gpioButt.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&gpioButt);
}

void delay(){
for(uint32_t i = 0; i < 500000; i++);
}

int main(void){
	char user_text[] = "Hello World!";

    GPIO_BUTTON();

	SPI2_GPIOInits();
	SPI2_Init();
    
    SPI_SSOEConfig(SPI2, ENABLE);
    while (1){
        while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
        delay();
		SPI_PeripheralControl(SPI2, ENABLE);
		//Передача спочатку довжину слова
		uint8_t dataLen = strlen(user_text);
	    SPI_SendDate(SPI2, &dataLen, 1);

		while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));
		//Дозвіл SPI2
	    SPI_PeripheralControl(SPI2, DISABLE);
    }
    
	return 0;
}
