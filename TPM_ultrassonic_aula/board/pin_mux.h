/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

#define SOPT4_TPM1CH0SRC_TPM1_CH0 0x00u /*!<@brief TPM1 channel 0 input capture source select: TPM1_CH0 signal */
#define SOPT5_UART0RXSRC_UART_RX 0x00u  /*!<@brief UART0 receive data source select: UART0_RX pin */
#define SOPT5_UART0TXSRC_UART_TX 0x00u  /*!<@brief UART0 transmit data source select: UART0_TX pin */

/*! @name PORTA2 (number 28), J1[4]/D1/UART0_TX
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_DEBUG_UART_TX_PORT PORTA               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_DEBUG_UART_TX_PIN 2U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_DEBUG_UART_TX_PIN_MASK (1U << 2U)      /*!<@brief PORT pin mask */
                                                              /* @} */

/*! @name PORTA1 (number 27), J1[2]/D0/UART0_RX
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_DEBUG_UART_RX_PORT PORTA               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_DEBUG_UART_RX_PIN 1U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_DEBUG_UART_RX_PIN_MASK (1U << 1U)      /*!<@brief PORT pin mask */
                                                              /* @} */

/*! @name PORTB9 (number 48), J9[3]
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_TRIGGER_FGPIO FGPIOB             /*!<@brief FGPIO peripheral base pointer */
#define BOARD_INITPINS_TRIGGER_GPIO GPIOB               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_TRIGGER_GPIO_PIN_MASK (1U << 9U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_TRIGGER_PORT PORTB               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_TRIGGER_PIN 9U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_TRIGGER_PIN_MASK (1U << 9U)      /*!<@brief PORT pin mask */
                                                        /* @} */

/*! @name PORTA12 (number 32), J1[8]/D3
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_ECHO_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_ECHO_PIN 12U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_ECHO_PIN_MASK (1U << 12U)      /*!<@brief PORT pin mask */
                                                      /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/