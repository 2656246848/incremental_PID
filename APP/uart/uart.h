/*
 * uart.h
 *
 *  Created on: 2023年5月27日
 *      Author: gy
 */

#ifndef APP_UART_UART_H_
#define APP_UART_UART_H_

#include "DSP2833x_Device.h"     // DSP2833x 头文件
#include "DSP2833x_Examples.h"   // DSP2833x 例子相关头文件
#include "stdio.h"
#include "string.h"

void SCIa_SendByte(int dat);
void UARTa_Init(Uint32 baud);
void UARTa_SendByte(int a);
int fputs(const char *_ptr,FILE *_fp);
int fputc(int ch,FILE *fp);

#endif /* APP_UART_UART_H_ */
