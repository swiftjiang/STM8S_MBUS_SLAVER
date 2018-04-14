#include "test.h"

void test_get_data(uint8_t *data,uint8_t len)
{
  if(data==NULL || len==0)
    return;
  uint16_t temp = app_sched_event_put(data, len, test_parse);

  if(temp != 0)
  {
    //printf("[error]%s,%d,temp = %d\r\n",__FILE__,__LINE__,temp);
    return;
  }
  //else
  //{
    //printf("[success]app_sched_event_put\r\n");
  //}
}
void test_parse(void *data,uint16_t len)
{
  
  
  uint8_t mdata[8] = {0xff,0xff,0x02,0x01};
  /*
  uint8_t i;
  for(i=0;i<len;i++)
  {
    printf("%02x,",((uint8_t*)data)[i]);
  }
  printf("\r\n");
  */
  
  if(((uint8_t*)data)[2] == 0x01 && ((uint8_t*)data)[3] == 0x01)
  {
    set_channel(1);////////////////////////////////////
    test_RC_read_code_value(&mdata[4]);
    Uart_TX(mdata,8);
  }
  
  /*********************************************************   old     *****************************/
  
//  uint8_t cmd,arg,seq;
//  uint8_t send[5] = {0xff};
//    
//  if((len != 5) || (((uint8_t*)data)[0] != 0xff) || (((uint8_t*)data)[1] != 0xff) )
//    return;
//  //if( (((uint8_t*)data)[2] != 0xc0) )
//  cmd = ((uint8_t*)data)[2];
//  arg = ((uint8_t*)data)[3];
//  seq = ((uint8_t*)data)[4];
//  
//  switch( cmd )
//  {
//  case 0xc0:
//    send[2] = cmd;
//    send[3] = arg;
//    send[4] = seq;
//    Uart_TX(send,len);
//    break;
//  default:
//    ;
//  }
  
    //Uart_TX((uint8_t*)data,len);
  
  
//  for(i=0;i<len;i++)
//  {
//    uint8_t temp = ((uint8_t*)data)[i];
//    Uart_TX(&temp,1);
//  }
//	printf("[test_parse info]\r\n*****");
//	for(i=0;i<len;i++)
//	{
//		printf("0x%02x,",data)[i]);
//	}
//	printf("\r\n");
}


//转轮编码测试
/*
 *bit1 --> C7
 *bit2 --> D2
 *bit3 --> D3
 *bit4 --> C4
 *bit5 --> C5
 */
void test_RC_init_pin(void)
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
void test_RC_read_code(void)//uint8_t *data
{
  uint8_t data[5]={0,0,0,0,0};
  /*
  data[0] = GPIO_ReadInputPin(GPIOD, GPIO_PIN_1);
  data[1] = GPIO_ReadInputPin(GPIOD, GPIO_PIN_2);
  data[2] = GPIO_ReadInputPin(GPIOD, GPIO_PIN_3);
  data[3] = GPIO_ReadInputPin(GPIOC, GPIO_PIN_4);
  data[4] = GPIO_ReadInputPin(GPIOC, GPIO_PIN_5);
  */
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
  /*
  GPIO_WriteHigh( GPIOA, GPIO_PIN_1);//个位
  GPIO_WriteHigh( GPIOA, GPIO_PIN_2);//十位
  GPIO_WriteHigh( GPIOA, GPIO_PIN_3);//百位
  GPIO_WriteHigh( GPIOC, GPIO_PIN_3);//千位
*/
  /*
  set_channel(1);
  set_channel(2);
  set_channel(3);
  set_channel(4);
  */
  
  printf(" - - - - -\r\n");
  printf(" %d %d %d %d %d\r\n",data[0],data[1],data[2],data[3],data[4]);
}

void test_RC_read_code_value(uint8_t* data)//uint8_t *data
{
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

void set_channel(uint8_t cha)
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