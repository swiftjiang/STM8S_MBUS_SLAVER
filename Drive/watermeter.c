#include "watermeter.h"

#define DEBUG_READ 1
/*********************字符编码表****************************/

/*
uint8_t codeTable[30][5] = {
    {0,0,1,1,1},
    {0,0,1,1,0},
    {0,0,0,1,0},

    {0,0,0,1,1},
    {1,0,0,1,1},
    {1,0,0,0,1},

    {0,0,0,0,1},
    {0,1,0,0,1},
    {0,1,0,0,0},

    {0,0,0,0,0},
    {0,0,1,0,0},
    {1,0,1,0,0},

    {1,0,0,0,0},
    {1,0,0,1,0},
    {1,1,0,1,0},

    {1,1,0,0,0},
    {1,1,0,0,1},
    {1,1,1,0,1},

    {1,1,1,0,0},
    {0,1,1,0,0},
    {0,1,1,1,0},

    {1,1,1,1,0},
    {1,0,1,1,0},
    {1,0,1,1,1},

    {1,1,1,1,1},
    {1,1,0,1,1},
    {0,1,0,1,1},

    {0,1,1,1,1},
    {0,1,1,0,1},
    {0,0,1,0,1},

};
*/

uint8_t codeTable[10][3][5] = {
  {
    {0,0,1,1,1},
    {0,0,1,1,0},
    {0,0,0,1,0},
  },
  {
    {0,0,0,1,1},
    {1,0,0,1,1},
    {1,0,0,0,1},
  },
  {
    {0,0,0,0,1},
    {0,1,0,0,1},
    {0,1,0,0,0},
  },
  {
    {0,0,0,0,0},
    {0,0,1,0,0},
    {1,0,1,0,0},
  },
  {
    {1,0,0,0,0},
    {1,0,0,1,0},
    {1,1,0,1,0},
  },
  {
    {1,1,0,0,0},
    {1,1,0,0,1},
    {1,1,1,0,1},
  },
  {
    {1,1,1,0,0},
    {0,1,1,0,0},
    {0,1,1,1,0},
  },
  {
    {1,1,1,1,0},
    {1,0,1,1,0},
    {1,0,1,1,1},
  },
  {
    {1,1,1,1,1},
    {1,1,0,1,1},
    {0,1,0,1,1},
  },
  {
    {0,1,1,1,1},
    {0,1,1,0,1},
    {0,0,1,0,1},
  },
};

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
  GPIO_Init( GPIOC, GPIO_PIN_7,GPIO_MODE_IN_FL_NO_IT);//r1
  GPIO_Init( GPIOD, GPIO_PIN_2,GPIO_MODE_IN_FL_NO_IT);//r2
  GPIO_Init( GPIOD, GPIO_PIN_3,GPIO_MODE_IN_FL_NO_IT);//r3
  GPIO_Init( GPIOC, GPIO_PIN_4,GPIO_MODE_IN_FL_NO_IT);//r4
  GPIO_Init( GPIOC, GPIO_PIN_5,GPIO_MODE_IN_FL_NO_IT);//r5
  
  GPIO_Init( GPIOA, GPIO_PIN_1,GPIO_MODE_OUT_PP_LOW_FAST);//个位
  GPIO_Init( GPIOA, GPIO_PIN_2,GPIO_MODE_OUT_PP_LOW_FAST);//十位
  GPIO_Init( GPIOD, GPIO_PIN_1,GPIO_MODE_OUT_PP_LOW_FAST);//百位
  GPIO_Init( GPIOB, GPIO_PIN_4,GPIO_MODE_OUT_PP_LOW_FAST);//千位
  
  GPIO_Init( GPIOC, GPIO_PIN_3,GPIO_MODE_OUT_PP_LOW_FAST);//d1
  GPIO_Init( GPIOB, GPIO_PIN_5,GPIO_MODE_OUT_PP_LOW_FAST);//d2
  GPIO_Init( GPIOC, GPIO_PIN_6,GPIO_MODE_OUT_PP_LOW_FAST);//d3
  GPIO_Init( GPIOA, GPIO_PIN_3,GPIO_MODE_OUT_PP_LOW_FAST);//d4
  GPIO_Init( GPIOD, GPIO_PIN_4,GPIO_MODE_OUT_PP_LOW_FAST);//d5
  
  GPIO_WriteLow( GPIOA, GPIO_PIN_1);//个位
  GPIO_WriteLow( GPIOA, GPIO_PIN_2);//十位
  GPIO_WriteLow( GPIOD, GPIO_PIN_1);//百位
  GPIO_WriteLow( GPIOB, GPIO_PIN_4);//千位
  
  GPIO_WriteLow( GPIOC, GPIO_PIN_3);
  GPIO_WriteLow( GPIOB, GPIO_PIN_5);
  GPIO_WriteLow( GPIOC, GPIO_PIN_6);
  GPIO_WriteLow( GPIOA, GPIO_PIN_3);
  GPIO_WriteLow( GPIOD, GPIO_PIN_4);
  
}

void watermeter_set_channel(uint8_t cha)
{
  GPIO_WriteLow( GPIOA, GPIO_PIN_1);//个位
  GPIO_WriteLow( GPIOA, GPIO_PIN_2);//十位
  GPIO_WriteLow( GPIOD, GPIO_PIN_1);//百位
  GPIO_WriteLow( GPIOB, GPIO_PIN_4);//千位
  if(cha == 0)
    return;
  if(cha == 1)
    GPIO_WriteHigh( GPIOA, GPIO_PIN_1);//个位
  if(cha == 2)
    GPIO_WriteHigh( GPIOA, GPIO_PIN_2);//十位
  if(cha == 3)
    GPIO_WriteHigh( GPIOD, GPIO_PIN_1);//百位
  if(cha == 4)
    GPIO_WriteHigh( GPIOB, GPIO_PIN_4);//千位
}

void watermeter_set_shine(uint8_t s)
{
  s = s&0x07;
  GPIO_WriteLow( GPIOC, GPIO_PIN_3);
  GPIO_WriteLow( GPIOB, GPIO_PIN_5);
  GPIO_WriteLow( GPIOC, GPIO_PIN_6);
  GPIO_WriteLow( GPIOA, GPIO_PIN_3);
  GPIO_WriteLow( GPIOD, GPIO_PIN_4);
  
  if(s == 0)
    return;
  if(s == 1)
  {
    GPIO_WriteHigh( GPIOC, GPIO_PIN_3);//
    return;
  }
  if(s == 2)
  {
    GPIO_WriteHigh( GPIOB, GPIO_PIN_5);//
    return;
  }
  if(s == 3)
  {
    GPIO_WriteHigh( GPIOC, GPIO_PIN_6);//
    return;
  }
  if(s == 4)
  {
    GPIO_WriteHigh( GPIOA, GPIO_PIN_3);//
    return;
  }
  if(s == 5)
  {
    GPIO_WriteHigh( GPIOD, GPIO_PIN_4);//
    return;
  }
}

uint8_t watermeter_binary2dec(uint8_t *data)
{
  uint8_t i,j;

  for(i=0;i<10;i++)
  {
    for(j=0;j<3;j++)
    {
      if(0 == memcmp(codeTable[i][j],data,5))
      {
        return i;
      }
    }
  }
  return 0x0a;
}


uint8_t watermeter_read_bit(uint8_t bit)
{
  uint8_t value=0;
  
  if(bit == 1)
  {
    if(GPIO_ReadInputPin(GPIOC, GPIO_PIN_7))
      value = 1;
    return value;
  }
  if(bit == 2)
  {
    if(GPIO_ReadInputPin(GPIOD, GPIO_PIN_2))
      value = 1;
    return value;
  }
  if(bit == 3)
  {
    if(GPIO_ReadInputPin(GPIOD, GPIO_PIN_3))
      value = 1;
    return value;
  }
  if(bit == 4)
  {
    if(GPIO_ReadInputPin(GPIOC, GPIO_PIN_4))
      value = 1;
    return value;
  }
  if(bit == 5)
  {
    if(GPIO_ReadInputPin(GPIOC, GPIO_PIN_5))
      value = 1;
    return value;
  }
  return 0x0a;
}

  
 void watermeter_read_binary(uint8_t *recdata, uint8_t cha)
{  
  memcpy(recdata,"55555",5);
  uint8_t j;

  for(j=0;j<5;j++)
  {
    watermeter_set_channel(cha);
    watermeter_set_shine(j+1);
    recdata[j] = watermeter_read_bit(j+1);
    watermeter_set_shine(0);//close shine
    watermeter_set_channel(0);
  }

}


uint8_t predata[4][5];
uint8_t recdata[4][5];
uint8_t dec[4];

uint16_t watermeter_read_dec(void)
{
  //watermeter_read_binary(data,cha);
  
  uint8_t i;
  uint16_t value = 0;
  
  for(i=0;i<4;i++)
  {
    watermeter_read_binary(recdata[i],i+1);
    dec[i] = watermeter_binary2dec(recdata[i]);
  }
#ifdef DEBUG_READ
  if(
     memcmp(recdata[0],predata[0],5)!=0 ||
     memcmp(recdata[1],predata[1],5)!=0 ||
     memcmp(recdata[2],predata[2],5)!=0 ||
     memcmp(recdata[3],predata[3],5)!=0 )
  {
    memcpy(predata[0],recdata[0],5);
    memcpy(predata[1],recdata[1],5);
    memcpy(predata[2],recdata[2],5);
    memcpy(predata[3],recdata[3],5);
    
    //
    printf("[K] ");
    for(i=0;i<5;i++)
    {
      printf("%d ",recdata[3][i]);
    }
    printf("\r\n");
    //printf("[DEC] = %d\r\n",dec[3]);
    //
    printf("[H] ");
    for(i=0;i<5;i++)
    {
      printf("%d ",recdata[2][i]);
    }
    printf("\r\n");
    //printf("[DEC] = %d\r\n",dec[2]);
    //
    printf("[D] ");
    for(i=0;i<5;i++)
    {
      printf("%d ",recdata[1][i]);
    }
    printf("\r\n");
    //printf("[DEC] = %d\r\n",dec[1]);
    //
    printf("[U] ");
    for(i=0;i<5;i++)
    {
      printf("%d ",recdata[0][i]);
    }
    printf("\r\n");
    //printf("[DEC] = %d\r\n",dec[0]);
    
    printf("[DEC]%d %d %d %d\r\n",dec[3],dec[2],dec[1],dec[0]);
    printf("___________________________________\r\n");
  }
  
#endif
  
  
  value = dec[0] + dec[1]*10 + dec[2]*100 + dec[3]*1000;
  return value;

}

uint8_t send[6] = {0xff,0xff,0x02,0x01,0x0,0x0};
void watermeter_get_data(uint8_t *data,uint8_t len)
{
  if(data==NULL)
    return;

  if(((uint8_t*)data)[2] == 0x01 && ((uint8_t*)data)[3] == 0x01)
  {
    ;//watermeter_send_value();
    uint16_t rec=watermeter_read_dec();
    printf("%d ",rec);
    printf("\r\n");
    send[4] = rec >> 8;
    send[5] = (uint8_t)(rec & 0x00ff);
    
    Uart_TX(send,6);
  }
  /*
  uint32_t temp = app_sched_event_put(data, len, watermeter_parse);

  if(temp != 0)
  {
    return;
  }
*/

}
void watermeter_parse(void *data,uint16_t len)
{
  //if(len != 5)
    //return;
  
  //Uart_TX(data,len);
  
  if(((uint8_t*)data)[2] == 0x01 && ((uint8_t*)data)[3] == 0x01)
  {
    ;//watermeter_send_value();
    
  }
  
}