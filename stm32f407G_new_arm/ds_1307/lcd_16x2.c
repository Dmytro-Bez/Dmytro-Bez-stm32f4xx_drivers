#include"lcd_16x2.h"

static void mdelay(uint32_t delay){
    for(uint32_t i = 0; i < (delay * 1000); i++);
}

static void udelay(uint32_t delay){
    for(uint32_t i = 0; i < (delay * 1); i++);
}

static void lcd_eneble(void){
    GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_EN,GPIO_PIN_SET);
    udelay(10);
    GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_RS,GPIO_PIN_RESET);
    udelay(100);
}

//Функція для 4 бітів інформації екрана
static void write_4_bits(uint8_t value){
    GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D4,((value >> 0) & 0x1));
    GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D5,((value >> 1) & 0x1));
    GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D6,((value >> 2) & 0x1));
    GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D7,((value >> 3) & 0x1));

   lcd_eneble();
}

void lcd_set_cursore(uint8_t row, uint8_t column){
    column--;
    switch (row)
    {
    case 1:
        lcd_send_command((column |= 0x80));
        break;
    case 2:
        lcd_send_command((column |= 0xC0));
        break;
    
    default:
        break;
    }
}

void lcd_send_command(uint8_t cmd){
    //RS = 0
    GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_RS,GPIO_PIN_RESET);
    //R/nW = 0
    GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_RW,GPIO_PIN_RESET);

    write_4_bits(cmd << 4);
    write_4_bits(cmd & 0x0F);
}

void lcd_print_char(uint8_t date){
    //RS = 1
    GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_EN,GPIO_PIN_SET);
     //R/nW = 0
    GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_RW,GPIO_PIN_RESET);

    write_4_bits(date << 4);
    write_4_bits(date & 0x0F);
}

void lcd_print_string(char *messege){
    do{
        lcd_print_char((uint8_t)* messege++);
    }
    while(*messege != '\0');
}

void lcd_display_clear(void){
    lcd_send_command(LCD_CMD_DIS_CLEAR);
    mdelay(2);
}

void lcd_display_return_home(void){
    lcd_send_command(LCD_CMD_DIS_RETURN_HOME);
    mdelay(2);
}
    
void lcd_init(void){
    //1. Нпалаштування роботу пінів контролера з екраном
    GPIO_Handler_t lcd_signal;
    lcd_signal.pGPIOx = LCD_GPIO_PORT;
    lcd_signal.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    lcd_signal.GPIO_PinConfig.GPIO_PinNummber = LCD_GPIO_RS;
    lcd_signal.GPIO_PinConfig.GPIO_PinOPType = GPIO_OPT_TYPE_PP;
    lcd_signal.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    lcd_signal.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GPIO_Init(&lcd_signal);

    lcd_signal.GPIO_PinConfig.GPIO_PinNummber = LCD_GPIO_RW;
    GPIO_Init(&lcd_signal);
    lcd_signal.GPIO_PinConfig.GPIO_PinNummber = LCD_GPIO_EN;
    GPIO_Init(&lcd_signal);
    lcd_signal.GPIO_PinConfig.GPIO_PinNummber = LCD_GPIO_D4;
    GPIO_Init(&lcd_signal);
    lcd_signal.GPIO_PinConfig.GPIO_PinNummber = LCD_GPIO_D5;
    GPIO_Init(&lcd_signal);
    lcd_signal.GPIO_PinConfig.GPIO_PinNummber = LCD_GPIO_D6;
    GPIO_Init(&lcd_signal);
    lcd_signal.GPIO_PinConfig.GPIO_PinNummber = LCD_GPIO_D7;
    GPIO_Init(&lcd_signal);

    GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_RS,GPIO_PIN_RESET);
    GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_RW,GPIO_PIN_RESET);
    GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_EN,GPIO_PIN_RESET);
    GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D4,GPIO_PIN_RESET);
    GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D5,GPIO_PIN_RESET);
    GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D6,GPIO_PIN_RESET);
    GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D7,GPIO_PIN_RESET);

    //2. Зробити ініціалізацію екрана
    mdelay(40);
    //RS = 0
    GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_RS,GPIO_PIN_RESET);
    //RW = 0
    GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_RW,GPIO_PIN_RESET);
    write_4_bits(0x3);
    mdelay(5);
    write_4_bits(0x3);
    udelay(150);
    write_4_bits(0x3);
    write_4_bits(0x2);

    lcd_send_command(LCD_CMD_4DL_2N_5N8F);
    lcd_send_command(LCD_CMD_DON_CURON);
    lcd_display_clear();

    lcd_send_command(LCD_CMD_ICADD);
}
