#include <stm32f4xxx.h>
#include "stm32f4xx_i2c_driver.h"
#include "stm32f4xx_gpio_driver.h"

#define MY_ADDR			0x61
#define SLAVE_ADDR		0x68

void delay_ms(void);
void I2C1_GPIOInit(void);
void I2C1_Inits(void);
void GPIO_ButtonInit();

/*
 * PB6 -> SCL
 * PB7 -> SDA
 */

GPIO_Handler_t I2CPins, GPIOBtn, GpioLed;
I2C_Handler_t I2C1Handle;

uint8_t some_date[] = "We are testing I2C master tX\n";

int main(void){

	printf("Hello world!\n");
	GPIO_ButtonInit();
	I2C1_GPIOInit();
	I2C1_Inits();

	I2C_PeripheralControl(I2C1,ENABLE);

	while(1){
		while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0));
		delay_ms();

		I2C_MasterSendDate(&I2C1Handle,some_date,strlen((char*)some_date),SLAVE_ADDR);
		printf("Send masseger!\n");
	}

	return 0;
}

/*
 * Функція налаштування пінів порта для роботи з I2C
 */
void I2C1_GPIOInit(void){

//	printf("Work I2C1_GPIOInit\n");

	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OPT_TYPE_OD;
	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCL
	I2CPins.GPIO_PinConfig.GPIO_PinNummber = GPIO_PIN_NO_6;
	GPIO_Init(&I2CPins);
	//SDA
	I2CPins.GPIO_PinConfig.GPIO_PinNummber = GPIO_PIN_NO_7;
	GPIO_Init(&I2CPins);
}

/* 
 * Фунція ініціалізації I2C1
 */
void I2C1_Inits(void){
	printf("Work I2C1_Inits\n");

	I2C1Handle.pI2Cx = I2C1;
	I2C1Handle.pI2C_Config->I2C_ACKControl = I2C_ACK_ENABLE;
	I2C1Handle.pI2C_Config->I2C_FMDutyCycle = I2C_FM_Duty_2;
	I2C1Handle.pI2C_Config->I2C_DeviceAddress = MY_ADDR;
	I2C1Handle.pI2C_Config->I2C_SCLKSpeed = I2C_SCLK_Speed_SM;

	I2C_Init(&I2C1Handle);
}

/*
 * Функція затримки
 */
void delay_ms(void){
	for(uint32_t i = 0; i < 500000 / 2; i++);
}

/*
 * Функція ініціації кнопки
 */
void GPIO_ButtonInit(){

	//this is btn gpio configuration
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNummber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&GPIOBtn);

	//this is led gpio configuration
	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNummber = GPIO_PIN_NO_12;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OPT_TYPE_OD;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PerClockControl(GPIOD,ENABLE);

	GPIO_Init(&GpioLed);
}
