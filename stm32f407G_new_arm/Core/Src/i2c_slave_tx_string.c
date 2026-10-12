#include"main.h"
#include<stdio.h>

#define MY_ADDR		1

uint8_t tx_buff[32] = "STM32 slave mode...";
I2C_Handler_t I2C1Handler;

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}

/*
 * PB6 -> SCL
 * PB& -> SDA
 */

void I2C1_Inits(void){
	printf("Work I2C1_Inits\n");

	I2C1Handler.pI2Cx = I2C1;
	I2C1Handler.pI2C_Config->I2C_ACKControl = I2C_ACK_ENABLE;
	I2C1Handler.pI2C_Config->I2C_FMDutyCycle = I2C_FM_Duty_2;
	I2C1Handler.pI2C_Config->I2C_DeviceAddress = MY_ADDR;
	I2C1Handler.pI2C_Config->I2C_SCLKSpeed = I2C_SCLK_Speed_SM;

	I2C_Init(&I2C1Handler);
}

void I2C1_GPIOInit(void){
	GPIO_Handler_t I2CPins;

	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OPT_TYPE_OD;
	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

}

void GPIO_ButtonInit(void)
{
	GPIO_Handler_t GPIOBtn;

	//this is btn gpio configuration
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNummber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&GPIOBtn);

}

void I2C1_ER_IRQHandler(void){
//	I2C1_ER_IRQHand(&I2CHandler);
}

int main(void){

	GPIO_ButtonInit();
	I2C1_GPIOInit();
	//i2c peripheral configuration
	I2C1_Inits();


	//I2C IRQ configurations
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV,ENABLE);
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER,ENABLE);

	I2C_SlaveEnableDisableCallbackEvents(I2C1,ENABLE);

	//enable the i2c peripheral
	I2C_PeripheralControl(I2C1,ENABLE);

	//ack bit is made 1 after PE=1
	I2C_ManageAcking(I2C1,I2C_ACK_ENABLE);
	printf("Hello world!\n");
	
	while(1);

	return 1;
}
