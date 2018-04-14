#include "uart.h"

/*ϵͳprintf����ʵ��*/
/*
int fputc(int ch, FILE *f)//STM8S105,printf�����ض��� 
{   while (!(UART1->SR&0x80)); 
  UART1->DR=ch;
  return ch; 
} 
*/
#define RECV_Size 5 
uint8_t RECV_Buffer[RECV_Size] = {0};
__IO uint8_t RECV_Counter = 0;

uint8_t IncrementVar_RECV_Counter(void);
uint8_t GetVar_RECV_Counter(void);
uint8_t ResetVar_RECV_Counter(void);

timer_t uart_rx_timer;
void uart_rx_handler(void *arg)
{
  if(GetVar_RECV_Counter() != 0)
  {
    ResetVar_RECV_Counter();
    //watermeter_get_data(RECV_Buffer,RECV_Size);
  }
}
void Uart_Init( uint32_t UartBaudRate )
{
  
  //������������ TX������� RX��������
  GPIO_Init( UART_TX_GPIO_PORT, UART_TX_GPIO_PIN,GPIO_MODE_OUT_PP_HIGH_SLOW  );
  GPIO_Init( UART_RX_GPIO_PORT, UART_RX_GPIO_PIN,GPIO_MODE_IN_PU_NO_IT );
  
  //USART��������
  CLK_PeripheralClockConfig( CLK_PERIPHERAL_UART1, ENABLE);	//ʹ�ܴ���ʱ��
  UART1_DeInit();		//���ڸ�λ
  //���ڳ�ʼ�� 8λ���� 1��ֹͣλ ��У�� ���ͽ��� �����ʿɱ�
  UART1_Init( UartBaudRate, 
             UART1_WORDLENGTH_8D, 
             UART1_STOPBITS_1, 
             UART1_PARITY_NO,
             UART1_SYNCMODE_CLOCK_DISABLE,
             UART1_MODE_TXRX_ENABLE );
  UART1_ITConfig(UART1_IT_RXNE_OR,ENABLE);
  //UART1_ITConfig(UART1_IT_IDLE,ENABLE);
  UART1_Cmd(ENABLE);	//ʹ�ܴ��� 
  enableInterrupts();
  
  //timer_create(&uart_rx_timer,3000,OPT_TMR_PERIODIC,uart_rx_handler,NULL);
  //timer_start(&uart_rx_timer);
}

void Uart_TX(uint8_t *data,uint8_t len)
{
  while( len-- )
  {
    while( RESET == UART1_GetFlagStatus( UART1_FLAG_TXE ));
    UART1_SendData8( *data );
    data++;
  }
}



#include "watermeter.h"
INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
  //In order to detect unexpected events during development,
  //it is recommended to set a breakpoint on the following instruction.
  //
  //RECV_Buffer[IncrementVar_RECV_Counter()]  = UART1_ReceiveData8();
  /*
  if(UART1_GetITStatus(UART1_IT_IDLE) != RESET)
  {
    UART1_ClearITPendingBit(UART1_IT_IDLE);
    //ResetVar_RECV_Counter();
    //watermeter_get_data(RECV_Buffer,RECV_Size);
  }
*/
  
  
 RECV_Buffer[IncrementVar_RECV_Counter()]  = UART1_ReceiveData8();
 
 if(GetVar_RECV_Counter() ==  RECV_Size)
 {
   ResetVar_RECV_Counter();
   watermeter_get_data(RECV_Buffer,RECV_Size);
 }

  
}

uint8_t IncrementVar_RECV_Counter(void)
{
  return RECV_Counter++;
}
uint8_t GetVar_RECV_Counter(void)
{
  return RECV_Counter;
}
uint8_t ResetVar_RECV_Counter(void)
{
  return RECV_Counter=0;
}


/*
void Uart_TXRXEnable()
{
  GPIO_Init( UART_TX_GPIO_PORT, UART_TX_GPIO_PIN,GPIO_MODE_OUT_PP_HIGH_SLOW  );
  GPIO_Init( UART_RX_GPIO_PORT, UART_RX_GPIO_PIN,GPIO_MODE_IN_PU_NO_IT  );
  UART1_Cmd(ENABLE);
}
void Uart_TXRXDisable()
{
  UART1_Cmd(DISABLE);
  GPIO_Init( UART_TX_GPIO_PORT, UART_TX_GPIO_PIN,GPIO_MODE_IN_FL_NO_IT  );
  GPIO_Init( UART_RX_GPIO_PORT, UART_RX_GPIO_PIN,GPIO_MODE_IN_FL_NO_IT  );
  
}
*/