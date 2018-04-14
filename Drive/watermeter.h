#ifndef __WATERMETER_H__
#define __WATERMETER_H__


#include "watermeter.h"
#include "test.h"

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

void watermeter_read_BIT_value(uint8_t* data,uint8_t len);

uint8_t watermeter_code2value(uint8_t* data,uint8_t len);

void watermeter_get_value(uint8_t* data,uint8_t len);

void watermeter_send_value();


#endif