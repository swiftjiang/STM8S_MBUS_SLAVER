#include "watermeter.h"

void watermeter_get_data(uint8_t *data,uint8_t len)
{
  if(data==NULL)
    return;
  
  uint32_t temp = app_sched_event_put(data, len, watermeter_parse);

  if(temp != 0)
  {
    return;
  }

}
void watermeter_parse(void *data,uint16_t len)
{
  //if(len != 5)
    //return;
  
  //Uart_TX(data,len);
  
  if(((uint8_t*)data)[2] == 0x01 && ((uint8_t*)data)[3] == 0x01)
  {
    watermeter_send_value();
  }
  
}


//转轮编码测试
/*
 *bit1 --> C7
 *bit2 --> D2
 *bit3 --> D3
 *bit4 --> C4
 *bit5 --> C5
 */
void watermeter_PIN_init_pin(void)
{
  GPIO_Init( GPIOC, GPIO_PIN_7,GPIO_MODE_IN_FL_NO_IT);//GPIO_MODE_IN_PU_NO_IT   GPIO_MODE_IN_FL_NO_IT
  GPIO_Init( GPIOD, GPIO_PIN_2,GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init( GPIOD, GPIO_PIN_3,GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init( GPIOC, GPIO_PIN_4,GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init( GPIOC, GPIO_PIN_5,GPIO_MODE_IN_FL_NO_IT);
  
  GPIO_Init( GPIOA, GPIO_PIN_1,GPIO_MODE_OUT_PP_LOW_FAST);//个位
  GPIO_Init( GPIOA, GPIO_PIN_2,GPIO_MODE_OUT_PP_LOW_FAST);//十位
  GPIO_Init( GPIOA, GPIO_PIN_3,GPIO_MODE_OUT_PP_LOW_FAST);//百位
  GPIO_Init( GPIOC, GPIO_PIN_3,GPIO_MODE_OUT_PP_LOW_FAST);//千位
  
  GPIO_WriteLow( GPIOA, GPIO_PIN_1);//个位
  GPIO_WriteLow( GPIOA, GPIO_PIN_2);//十位
  GPIO_WriteLow( GPIOA, GPIO_PIN_3);//百位
  GPIO_WriteLow( GPIOC, GPIO_PIN_3);//千位
  
  printf("init read code PIN OK !\r\n");
  
}

void watermeter_set_channel(uint8_t cha)
{
  //GPIO_WriteLow  GPIO_WriteHigh
  GPIO_WriteLow( GPIOA, GPIO_PIN_1);//个位
  GPIO_WriteLow( GPIOA, GPIO_PIN_2);//十位
  GPIO_WriteLow( GPIOA, GPIO_PIN_3);//百位
  GPIO_WriteLow( GPIOC, GPIO_PIN_3);//千位
  switch(cha)
  {
  case 1:
    GPIO_WriteHigh( GPIOA, GPIO_PIN_1);//个位
    break;
  case 2:
    GPIO_WriteHigh( GPIOA, GPIO_PIN_2);//十位
    break;
  case 3:
    GPIO_WriteHigh( GPIOA, GPIO_PIN_3);//百位
    break;
  case 4:
    GPIO_WriteHigh( GPIOC, GPIO_PIN_3);//千位
    break;
    
  }
}

void watermeter_read_BIT_value(uint8_t* data,uint8_t len)//uint8_t *data
{
  if(len != 5)
    return;
  if(GPIO_ReadInputPin(GPIOC, GPIO_PIN_7))
    data[0] = 1;
  if(GPIO_ReadInputPin(GPIOD, GPIO_PIN_2))
    data[1] = 1;
  if(GPIO_ReadInputPin(GPIOD, GPIO_PIN_3))
    data[2] = 1;
  if(GPIO_ReadInputPin(GPIOC, GPIO_PIN_4))
    data[3] = 1;
  if(GPIO_ReadInputPin(GPIOC, GPIO_PIN_5))
    data[4] = 1;
}

uint8_t watermeter_code2value(uint8_t* data,uint8_t len)
{
  if(len != 5)
    return 0x0a;
  
  uint8_t i = 0;
  while(i<5)
  {
    if(data[i++] == 0)
      return i; 
  }
  return 5;
}

void watermeter_get_value(uint8_t* data,uint8_t len)
{
  if(len != 4)
    return;
  
  uint8_t code[5];
  uint8_t i;
  for(i = 0;i < 4;i++)
  {
    watermeter_set_channel(i+1);
    watermeter_read_BIT_value(code,sizeof(code));
    //计算code对应的值
    data[i] = watermeter_code2value(code,sizeof(code));
  }
  
}


void watermeter_send_value()
{
  uint8_t read[4];
  uint8_t mdata[8] = {0xff,0xff,0x02,0x01};
  
  watermeter_get_value(read,sizeof(read));
  mdata[4] = read[0];
  mdata[5] = read[1];
  mdata[6] = read[2];
  mdata[7] = read[3];
  
  Uart_TX(mdata,8);
}