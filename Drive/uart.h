#ifndef __UART_H__
#define __UART_H__
#include <stdio.h>
//#include "stm8s.h"
#include "stm8s_clk.h"
#include "stm8s_gpio.h"
#include "stm8s_it.h"
#include "stm8s_uart1.h"
#include "timer.h"


//UART硬件接口IO定义
#define UART_TX_GPIO_PORT			GPIOD
#define UART_TX_GPIO_PIN			GPIO_PIN_5

#define UART_RX_GPIO_PORT			GPIOD
#define UART_RX_GPIO_PIN			GPIO_PIN_6


void Uart_Init( uint32_t UartBaudRate );
void Uart_TX(uint8_t *data,uint8_t len);


//void Uart_TXRXEnable();
//void Uart_TXRXDisable();

//void Uart_Send_bytes( uint8_t* TxBuffer, uint8_t len );
//uint8_t uart_rx_bytes( uint8_t* RxBuffer );

#endif 

