/*
 * bluetooth.c
 *
 *  Created on: 28 de abr de 2021
 *      Author: mathe
 */
#include "bluetooth.h"
#include "fsl_debug_console.h"

bool (*bTooth_Send)(char *msg, size_t length);
bool (*bTooth_Receive)(char *msg, size_t length);
bool (*bTooth_SendAT)(char *msg);
bool (*bTooth_ReceiveAT)(char *msg);

static UART_Type *uart_base;
static uart_config_t uart_config;
static lpsci_config_t lpsci_config;
//static char bt_at_response[BT_MAX_DATA_LENGHT];

bool bTooth_Init(bt_config_t *config)
{
	bool returnVal = false;
	volatile char cleanChar;

#ifdef BT_LPSCI
	if(config->uart_base == kBt_UART0)
	{
	    LPSCI_GetDefaultConfig(&lpsci_config);
	    uart_config.baudRate_Bps = config->baudRate_Bps;
	    uart_config.enableTx = true;
	    uart_config.enableRx = true;

	   	CLOCK_SetLpsci0Clock(1);
		LPSCI_Init(UART0, &lpsci_config, CLOCK_GetPllFllSelClkFreq());

		bTooth_Send = bTooth_LPSCI_Send;
		bTooth_Receive = bTooth_LPSCI_Receive;
		bTooth_SendAT = bTooth_LPSCI_SendAT;
		bTooth_ReceiveAT = bTooth_LPSCI_ReceiveAT;
		/* Clean TX e RX data registers. */
		while (!(UART0->S1 & UART_S1_TC_MASK)) /* It is transmitting?*/
		{}
		UART0->D = cleanChar;
		cleanChar = UART0->D;

		returnVal = true;
	}
#endif

#ifdef BT_UART
	if((config->uart_base == kBt_UART1) || (config->uart_base == kBt_UART2))
	{
		if((config->uart_base == kBt_UART1))
		{
			uart_base = UART1;
		}
		else
		{
			uart_base = UART2;
		}

	    UART_GetDefaultConfig(&uart_config);
	    uart_config.baudRate_Bps = config->baudRate_Bps;
	    uart_config.enableTx = true;
	    uart_config.enableRx = true;

		if(UART_Init(uart_base, &uart_config, CLOCK_GetBusClkFreq()) ==
				kStatus_Success)
		{
			bTooth_Send = bTooth_UART_Send;
			bTooth_Receive = bTooth_UART_Receive;
			bTooth_SendAT = bTooth_UART_SendAT;
			bTooth_ReceiveAT = bTooth_UART_ReceiveAT;

			/* Clean TX e RX data registers. */
			uart_base->D = cleanChar;
			while (!(uart_base->S1 & UART_S1_TC_MASK)) /* It is transmitting?*/
			{}
			cleanChar = uart_base->D;
			returnVal = true;
		}
	}
#endif
	return returnVal;
}

#ifdef BT_LPSCI
bool bTooth_LPSCI_Send(char *msg, size_t length)
{
	for(size_t i = 0; i < length; ++i)
	{
		UART0->D = (uint8_t)msg[i];
	    while (!(UART0->S1 & UART_S1_TC_MASK)) /* It is transmitting?*/
	    {}
	}

	return true;
}

bool bTooth_LPSCI_Receive(char *msg, size_t length)
{
	for(size_t i = 0; i < length; ++i)
	{
		while (!(UART0->S1 & UART_S1_RDRF_MASK)) /* There is data?*/
		{}
		msg[i] = UART0->D;
	}

	return true;
}

bool bTooth_LPSCI_SendAT(char *msg)
{
	int i = 0;

	while(true)
	{
		UART0->D = (uint8_t)msg[i];
	    while (!(UART0->S1 & UART_S1_TC_MASK)) /* It is transmitting?*/
	    {}

	    if((msg[i] == '\r') && (msg[i+1] == '\n'))
	    {
	    	UART0->D = msg[++i]; /*Send '\n' and finish.*/
		    while (!(UART0->S1 & UART_S1_TC_MASK)) /* It is transmitting?*/
		    {}
	    	return true;
	    }
	    ++i;
	}
}

bool bTooth_LPSCI_ReceiveAT(char *msg)
{
	int i = 0;

	while(true)
	{
		while (!(UART0->S1 & UART_S1_RDRF_MASK)) /* There is data?*/
		{}
		msg[i] = UART0->D;

	    if((msg[i-1] == '\r') && (msg[i] == '\n'))
	    {
	    	return true;
	    }
	    ++i;
	}
}
#endif

#ifdef BT_UART
bool bTooth_UART_Send(char *msg, size_t length)
{
	for(size_t i = 0; i < length; ++i)
	{
		uart_base->D = (uint8_t)msg[i];
	    while (!(uart_base->S1 & UART_S1_TC_MASK)) /* It is transmitting?*/
	    {}
	}

	return true;
}

bool bTooth_UART_Receive(char *msg, size_t length)
{
	for(size_t i = 0; i < length; ++i)
	{
		while (!(uart_base->S1 & UART_S1_RDRF_MASK)) /* There is data?*/
		{}
		msg[i] = uart_base->D;
	}

	return true;
}

bool bTooth_UART_SendAT(char *msg)
{
	int i = 0;

	while(true)
	{
		uart_base->D = (uint8_t)msg[i];
	    while (!(uart_base->S1 & UART_S1_TC_MASK)) /* It is transmitting?*/
	    {}

	    if((msg[i] == '\r') && (msg[i+1] == '\n'))
	    {
	    	uart_base->D = msg[++i]; /*Send '\n' and finish.*/
		    while (!(uart_base->S1 & UART_S1_TC_MASK)) /* It is transmitting?*/
		    {}
	    	return true;
	    }
	    ++i;
	}
}

bool bTooth_UART_ReceiveAT(char *msg)
{
	int i = 0;

	while(true)
	{
		while (!(uart_base->S1 & UART_S1_RDRF_MASK)) /* There is data?*/
		{}
		msg[i] = uart_base->D;

	    if((msg[i-1] == '\r') && (msg[i] == '\n'))
	    {
	    	return true;
	    }
	    ++i;
	}
}
#endif

