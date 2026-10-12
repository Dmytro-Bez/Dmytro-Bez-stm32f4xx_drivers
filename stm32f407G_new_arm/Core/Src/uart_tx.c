#include"main.h"

void USART2_GPIOInit(void);
void USART2_Init(void);

char msg[1024] = "UARTTxtesting...\n\r";

USART_Handler_t usart2_handle;

int main(void){
    USART2_GPIOInit();
    USART2_Init();
    USART_PeripheralControl(USART2, ENABLE);
    USART_SendData(&usart2_handle, (uint8_t*)msg, strlen(msg));

    while (1);

    return 0;
    
}

void USART2_Init(void){
    usart2_handle.pUSARTx = USART2;
    usart2_handle.USART_Config.USART_Baud = USART_STD_BAUD_9600;
    usart2_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
    usart2_handle.USART_Config.USART_Mode = USART_MODE_TXRX;
    usart2_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
    usart2_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
    usart2_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;

    USART_Init(&usart2_handle);
}

void USART2_GPIOInit(void){
    GPIO_Handler_t usart2_gpoios;

    usart2_gpoios.pGPIOx = GPIOA;
    usart2_gpoios.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    usart2_gpoios.GPIO_PinConfig.GPIO_PinOPType = GPIO_OPT_TYPE_PP;
    usart2_gpoios.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    usart2_gpoios.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    usart2_gpoios.GPIO_PinConfig.GPIO_PinAltFunMode = 7;

    //USART TX 
    usart2_gpoios.GPIO_PinConfig.GPIO_PinNummber = GPIO_PIN_NO_2;
    GPIO_Init(&usart2_gpoios);
    //USART RX
    usart2_gpoios.GPIO_PinConfig.GPIO_PinNummber = GPIO_PIN_NO_3;
    GPIO_Init(&usart2_gpoios);
}
