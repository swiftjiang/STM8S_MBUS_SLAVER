/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Main program body
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "uart.h"
#include "app_scheduler.h"
//#include "timer.h"
#include "watermeter.h"

#include "test.h"

/* Private defines -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void Delay_Ms(uint16_t time)  //延时函数
{ 
	uint16_t i,j;
	for(i=0;i<time;i++)
  		for(j=0;j<1026;j++);
}

uint8_t TX_gpio;

timer_t testTimerTyp;
void timerHandler(void* arg)
{
  (void)arg;
  
  //uint8_t t = GPIO_ReadInputPin(GPIOB, GPIO_PIN_4);
  //printf("pin = %d\r\n",t>>4);
  //GPIO_WriteReverse(GPIOB, GPIO_PIN_5);
  GPIO_WriteReverse(GPIOB, GPIO_PIN_5);
  
  //set_channel(1);////////////////////////////////////
  //test_RC_read_code();

}
void testTimer(void)
{
  timer_create(&testTimerTyp,1000,OPT_TMR_PERIODIC,timerHandler,NULL);
  timer_start(&testTimerTyp);
}

void Delay_100ns(uint16_t time)  //延时函数
{ 
	uint16_t i,j,k;
        for(k=0;k<time;k++)
        {
	for(i=0;i<350;i++)
  		for(j=0;j<3000;j++)
                {
                  i=i;
                }
        }
}

void main(void)
{
  CLK_HSICmd(ENABLE);//开始内部高频RC
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV8);//不分频 
  //CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);  
  //scheduler_init();
  //timer_init();
  
  Uart_Init(9600);
  //printf("testing...\n");
  
  //watermeter_PIN_init_pin();

/* Infinite loop */

  //watermeter_set_channel(1);
  
  GPIO_Init( GPIOA, GPIO_PIN_1,GPIO_MODE_OUT_PP_LOW_FAST);//个位
  GPIO_Init( GPIOA, GPIO_PIN_2,GPIO_MODE_OUT_PP_LOW_FAST);//十位
  GPIO_Init( GPIOD, GPIO_PIN_1,GPIO_MODE_OUT_PP_LOW_FAST);//百位
  GPIO_Init( GPIOB, GPIO_PIN_4,GPIO_MODE_OUT_PP_LOW_FAST);//千位
  
  GPIO_Init( GPIOC, GPIO_PIN_3,GPIO_MODE_OUT_PP_LOW_FAST);//d1
  GPIO_Init( GPIOB, GPIO_PIN_5,GPIO_MODE_OUT_PP_LOW_FAST);//d2
  GPIO_Init( GPIOC, GPIO_PIN_6,GPIO_MODE_OUT_PP_LOW_FAST);//d3
  GPIO_Init( GPIOA, GPIO_PIN_3,GPIO_MODE_OUT_PP_LOW_FAST);//d4
  GPIO_Init( GPIOD, GPIO_PIN_4,GPIO_MODE_OUT_PP_LOW_FAST);//d5
  
  GPIO_WriteHigh( GPIOA, GPIO_PIN_1);//个位
  GPIO_WriteHigh( GPIOA, GPIO_PIN_2);//十位
  GPIO_WriteHigh( GPIOD, GPIO_PIN_1);//百位
  GPIO_WriteHigh( GPIOB, GPIO_PIN_4);//千位
  
  GPIO_WriteHigh( GPIOC, GPIO_PIN_3);
  GPIO_WriteHigh( GPIOB, GPIO_PIN_5);
  GPIO_WriteHigh( GPIOC, GPIO_PIN_6);
  GPIO_WriteHigh( GPIOA, GPIO_PIN_3);
  GPIO_WriteHigh( GPIOD, GPIO_PIN_4);
  
  while(1)
  {
    ;
    //watermeter_read_dec();
    //app_sched_execute();
    /*
    Delay_100ns(65535);
    GPIO_WriteReverse( GPIOB, GPIO_PIN_5);
    GPIO_WriteReverse( GPIOC, GPIO_PIN_3);
    
    GPIO_WriteReverse( GPIOC, GPIO_PIN_6);
    */
  }

  
  
  /*
  uint8_t cha = 2;
  uint8_t recdata[5];
  uint8_t predata[5];
  uint8_t i,dec;  
  watermeter_PIN_init_pin();
  while(1)
  {
    watermeter_read_binary(recdata, cha);
    dec = watermeter_binary2dec(recdata);
    
    if(memcmp(predata,recdata,5)!=0)
    {
      memcpy(predata,recdata,5);
      for(i=0;i<5;i++)
      {
        printf("%d ",recdata[i]);
      }
      printf("  ->  dec = %d\r\n",dec);
    }
  }
*/


  
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
/*
void Delay_Ms(uint16_t time)  //延时函数
{ 
	uint16_t i,j,k;
        for(k=0;k<time*3;k++)
        {
	for(i=0;i<550;i++)
  		for(j=0;j<3000;j++)
                {
                  i=i;
                }
        }
}
void Delay_100ns(uint16_t time)  //延时函数
{ 
	uint16_t i,j,k;
        for(k=0;k<time;k++)
        {
	for(i=0;i<350;i++)
  		for(j=0;j<3000;j++)
                {
                  i=i;
                }
        }
}
*/