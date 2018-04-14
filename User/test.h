#ifndef __TEST_H__
#define __TEST_H__
#include <stdio.h>
#include <string.h>

#include "app_scheduler.h"
#include "uart.h"


//usart2 DMAÖĞ¶Ï²âÊÔ
void test_get_data(uint8_t *data,uint8_t len);
void test_parse(void *data,uint16_t len);

//×ªÂÖ±àÂë²âÊÔ
void test_RC_init_pin(void);
void test_RC_read_code(void);//uint8_t *data
void test_RC_read_code_value(uint8_t* data);//uint8_t *data
void set_channel(uint8_t cha);

#endif