/*
 * bluetooth.h
 *
 *  Created on: 28 de abr de 2021
 *      Author: mathe
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include "stdbool.h"
#include "stdint.h"
#include "string.h"
#include "fsl_uart.h"
#include "fsl_lpsci.h"

#define BT_LPSCI
#define BT_UART

extern bool (*bTooth_Send)(char *msg, size_t length);
extern bool (*bTooth_Receive)(char *msg, size_t length);
extern bool (*bTooth_SendAT)(char *msg);
extern bool (*bTooth_ReceiveAT)(char *msg);


enum _bt_uart_base_t
{
	kBt_UART0,
	kBt_UART1,
	kBt_UART2
};

typedef struct
{
	uint32_t baudRate_Bps;          /*!< UART baud rate  */
	enum _bt_uart_base_t uart_base;
}bt_config_t;

bool bTooth_Init(bt_config_t *config);

#ifdef BT_LPSCI
bool bTooth_LPSCI_Send(char *msg, size_t length);

bool bTooth_LPSCI_Receive(char *msg, size_t length);

bool bTooth_LPSCI_SendAT(char *msg);

bool bTooth_LPSCI_ReceiveAT(char *msg);
#endif

#ifdef BT_UART
bool bTooth_UART_Send(char *msg, size_t length);

bool bTooth_UART_Receive(char *msg, size_t length);

bool bTooth_UART_SendAT(char *msg);

bool bTooth_UART_ReceiveAT(char *msg);
#endif

#endif /* BLUETOOTH_H_ */
