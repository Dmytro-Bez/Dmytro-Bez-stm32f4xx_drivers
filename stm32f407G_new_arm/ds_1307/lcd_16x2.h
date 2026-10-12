#ifndef LCD_16X2_H_
#define LCD_16X2_H_

#include"main.h"

#define LCD_GPIO_PORT           GPIOD
#define LCD_GPIO_RS             GPIO_PIN_NO_0
#define LCD_GPIO_RW             GPIO_PIN_NO_1
#define LCD_GPIO_EN             GPIO_PIN_NO_2
#define LCD_GPIO_D4             GPIO_PIN_NO_3
#define LCD_GPIO_D5             GPIO_PIN_NO_4
#define LCD_GPIO_D6             GPIO_PIN_NO_5
#define LCD_GPIO_D7             GPIO_PIN_NO_6

#define LCD_CMD_4DL_2N_5N8F     0x28
#define LCD_CMD_DON_CURON       0xE0
#define LCD_CMD_ICADD           0x06
#define LCD_CMD_DIS_CLEAR       0x01
#define LCD_CMD_DIS_RETURN_HOME 0x02

void lcd_init(void);
void lcd_send_command(uint8_t cmd);
void lcd_print_char(uint8_t date);
void lcd_display_clear(void);
void lcd_send_string(char *messeger);
void lcd_display_return_home(void);
void lcd_print_string(char*);
void lcd_set_cursore(uint8_t row, uint8_t column);

#endif /* LCD_16X2_H_ */
