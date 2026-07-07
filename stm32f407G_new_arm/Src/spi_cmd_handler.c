#include"main.h"
#include"string.h"
/*
 * PB14 --> SP2_MISO
 * PB15 --> SP2_MOSI
 * PB13 --> SP2_SCL
 * PB12 --> SP2_NSS
 * ALF --> 5 mode
 */
extern void initialise_monitor_handler();

#define LED_PIN 9
#define COMMAND_LED_CTRL      		0x50
#define COMMAND_SENSOR_READ      	0x51
#define COMMAND_LED_READ      		0x52
#define COMMAND_PRINT      			0x53
#define COMMAND_ID_READ      		0x54

#define LED_ON     1
#define LED_OFF    0

//arduino analog pins
#define ANALOG_PIN0 	0
#define ANALOG_PIN1 	1
#define ANALOG_PIN2 	2
#define ANALOG_PIN3 	3
#define ANALOG_PIN4 	4

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

uint8_t SPI_VerifyResponse(uint8_t ackbyte){
    if(ackbyte == 0xF5){
        //ack
        return 1;
    } else {
        return 0;
    }
}

void delay(){
for(uint32_t i = 0; i < 500000; i++);
}

int main(void){
//	initialise_monitor_handler();
	printf("1");
	uint8_t demmy_write = 0xff;
    uint8_t dummy_read;
    
    GPIO_BUTTON();

	SPI2_GPIOInits();
	SPI2_Init();
    
    SPI_SSOEConfig(SPI2, ENABLE);
    while (1){
        while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
        delay();
		SPI_PeripheralControl(SPI2, ENABLE);

		//1. CMD_LED_CTRL <pin on (1)>  <value (1)>
        uint8_t ackbyte;
        uint8_t commandcode = COMMAND_LED_CTRL; 
        uint8_t args[2];

	    SPI_SendDate(SPI2, &commandcode, 1);
        SPI_SPI_RecevieDate(SPI2, &dummy_read, 1);
        //відправимо хибний біт
        SPI_SendDate(SPI2, &demmy_write, 1);
        //Читаємо ack byte
        SPI_SPI_RecevieDate(SPI2, &ackbyte, 1);
        if(SPI_VerifyResponse(ackbyte)){
            //надсилання аргументу
            args[0] = LED_PIN;    
            args[1] = LED_ON;   
            SPI_SendDate(SPI2, args, 2);
        }
        //2. CMD_SENSOR_READ <pin on (1)>  <value (1)>
        while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
        delay();
        commandcode = COMMAND_SENSOR_READ;
        SPI_SendDate(SPI2, &commandcode, 1);
        SPI_SPI_RecevieDate(SPI2, &dummy_read, 1);
        //відправимо хибний біт
        SPI_SendDate(SPI2, &demmy_write, 1);
        //Читаємо ack byte
        SPI_SPI_RecevieDate(SPI2, &ackbyte, 1);
        if(SPI_VerifyResponse(ackbyte)){
            //надсилання аргументу
            args[0] = ANALOG_PIN0; 
            SPI_SendDate(SPI2, args, 1);
            SPI_SPI_RecevieDate(SPI2, &dummy_read, 1);
            delay();
            SPI_SendDate(SPI2, &demmy_write, 1);
            uint8_t analog_read;
            SPI_SPI_RecevieDate(SPI2, &analog_read, 1);
        }
        
		while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));
		//Дозвіл SPI2
	    SPI_PeripheralControl(SPI2, DISABLE);
    }
    
	return 0;
}
