#include<stdio.h>
#include"ds_1307.h"
#include"lcd_16x2.h"

#define SYSTICK_TIM_CLK			16000000UL

char* get_to_of_week(uint8_t i);
char* time_to_string(RTC_time_t *rtc_time);
void nummber_to_string(uint8_t num, char* buff);
char* date_to_string(RTC_date_t *rtc_date);
void init_systick_timer(uint32_t tick_hz);
static void mdelay(uint32_t delay);

int main(void){

	RTC_time_t current_time;
	RTC_date_t current_date;

	lcd_set_cursore(1,1);
	printf("Test DS1307\n");
	lcd_init();
	lcd_print_string("RTC test..");
	mdelay(2000);
	lcd_display_clear();
	lcd_display_return_home();
	if(ds1307_init()){
		printf("RTC init  has failed\n");
		while(1);
	}

	init_systick_timer(1);

	current_date.day = FRIDAY;
	current_date.date = 14;
	current_date.month = 2;
	current_date.year = 26;

	current_time.hours = 12;
	current_time.minutes = 45;
	current_time.second = 23;
	current_time.time_format = TIME_FORMAT_12HRS_PM;

	ds1307_set_current_date(&current_date);
	ds1307_set_current_time(&current_time);

	ds1307_get_current_date(&current_date);
	ds1307_get_current_time(&current_time);

	char *am_pm;
	if(current_time.time_format != TIME_FORMAT_24HRS){
		am_pm = (current_time.time_format) ? "PM" : "AM";

		printf("Перевірка часу: %s %s\n",time_to_string(&current_time),am_pm);
		lcd_print_string(time_to_string(&current_time));
		lcd_print_string(am_pm);
	} else{
		printf("Перевірка часу: %s\n",time_to_string(&current_time));
		lcd_print_string(time_to_string(&current_time));
	}

	printf("Перевірка дня: %s <%s>\n",date_to_string(&current_date),get_to_of_week(current_date.day));
	lcd_set_cursore(2,1);
	lcd_print_string(date_to_string(&current_date));

	while(1);
	return 0;
}

static void mdelay(uint32_t delay){
    for(uint32_t i = 0; i < (delay * 1000); i++);
}

char* get_to_of_week(uint8_t i){
	char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

	return days[i - 1];
}

char* time_to_string(RTC_time_t *rtc_time){
	static char buff[9];

	buff[2] = ':';
	buff[5] = ':';

	nummber_to_string(rtc_time->hours, buff);
	nummber_to_string(rtc_time->minutes, &buff[3]);
	nummber_to_string(rtc_time->second, &buff[6]);

	buff[8] = '\0';
	return buff;
}

void nummber_to_string(uint8_t num, char* buff){
	if(num < 10){
		buff[0] = '0';
		buff[1] = num + 48;
	} else if(num >= 10 && num < 99){
		buff[0] = (num / 10) + 48;
		buff[1] = (num % 10) + 48;
	}
}

char* date_to_string(RTC_date_t *rtc_date){static char buff[9];

	buff[2] = '/';
	buff[5] = '/';

	nummber_to_string(rtc_date->date, buff);
	nummber_to_string(rtc_date->month, &buff[3]);
	nummber_to_string(rtc_date->year, &buff[6]);

	buff[8] = '\0';
	return buff;
}

void init_systick_timer(uint32_t tick_hz){
	uint32_t *pSRVR = (uint32_t*)0xE000E014;
	uint32_t *pSCSR = (uint32_t*)0xE000E010;

    /* calculation of reload value */
    uint32_t count_value = (SYSTICK_TIM_CLK/tick_hz)-1;

    //Clear the value of SVR
    *pSRVR &= ~(0x00FFFFFFFF);

    //load the value in to SVR
    *pSRVR |= count_value;

    //do some settings
    *pSCSR |= ( 1 << 1); //Enables SysTick exception request:
    *pSCSR |= ( 1 << 2);  //Indicates the clock source, processor clock source

    //enable the systick
    *pSCSR |= ( 1 << 0); //enables the counter
}

void SysTick_Handler(){
	RTC_time_t current_time;
	RTC_date_t current_date;

	ds1307_get_current_date(&current_date);
	char *am_pm;
	if(current_time.time_format != TIME_FORMAT_24HRS){
		am_pm = (current_time.time_format) ? "PM" : "AM";

		printf("Перевірка часу: %s %s\n",time_to_string(&current_time),am_pm);
		lcd_print_string(time_to_string(&current_time));
		lcd_print_string(am_pm);
	} else{
		printf("Перевірка часу: %s\n",time_to_string(&current_time));
		lcd_print_string(time_to_string(&current_time));
	}
	ds1307_get_current_time(&current_time);
	printf("Перевірка дня: %s <%s>\n",date_to_string(&current_date),get_to_of_week(current_date.day));
	lcd_set_cursore(2,1);
	lcd_print_string(date_to_string(&current_date));
	lcd_print_char('<');
	lcd_print_string(get_to_of_week(current_date.day));
	lcd_print_char('>');

}
