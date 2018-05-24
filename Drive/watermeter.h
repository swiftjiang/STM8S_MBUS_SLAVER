#ifndef __WATERMETER_H__
#define __WATERMETER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "watermeter.h"
#include "app_scheduler.h"
#include "uart.h"
#include "debug.h"




void watermeter_get_data(uint8_t *data,uint8_t len);

void watermeter_parse(void *data,uint16_t len);


//×ªÂÖ±àÂë²âÊÔ
/*
 *bit1 --> C7
 *bit2 --> D2
 *bit3 --> D3
 *bit4 --> C4
 *bit5 --> C5
 */
void watermeter_PIN_init_pin(void);

void watermeter_set_channel(uint8_t cha);

void watermeter_set_shine(uint8_t s);

uint8_t watermeter_binary2dec(uint8_t *data);

void watermeter_read_binary(uint8_t *data, uint8_t cha);

uint16_t watermeter_read_dec(void);







#endif