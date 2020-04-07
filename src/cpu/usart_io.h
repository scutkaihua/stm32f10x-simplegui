#ifndef _INCLUDE_USART_IO_H_
#define _INCLUDE_USART_IO_H_
#include "stm32f10x_usart.h"
#include "Typedefine.h"
#include "GPIO.h"

void    USART_Config(USART_TypeDef* st_USARTx, uint32_t ui_Baudrate, bool b_Enable);
void    DebugSerialPort_Initialize(uint32_t Baudrate);
int     USART_Putc(int iChar);

#endif //_INCLUDE_USART_IO_H_
