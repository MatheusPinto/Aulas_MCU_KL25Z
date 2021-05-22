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

#define SOPT5_UART0RXSRC_UART_RX 0x00u /*!<@brief UART0 receive data source select: UART0_RX pin */
#define SOPT5_UART0TXSRC_UART_TX 0x00u /*!<@brief UART0 transmit data source select: UART0_TX pin */
#define SOPT5_UART1RXSRC_UART_RX 0x00u /*!<@brief UART1 receive data source select: UART1_RX pin */
#define SOPT5_UART1TXSRC_UART_TX 0x00u /*!<@brief UART1 transmit data source select: UART1_TX pin */

/*! @name PORTA2 (number 28), J1[4]/D1/UART0_TX
  @{ */

/* Symbols to be used with PORT driver */
#define DEBUG_UART_TX_PORT PORTA               /*!<@brief PORT peripheral base pointer */
#define DEBUG_UART_TX_PIN 2U                   /*!<@brief PORT pin number */
#define DEBUG_UART_TX_PIN_MASK (1U << 2U)      /*!<@brief PORT pin mask */
                                               /* @} */

/*! @name PORTA1 (number 27), J1[2]/D0/UART0_RX
  @{ */

/* Symbols to be used with PORT driver */
#define DEBUG_UART_RX_PORT PORTA               /*!<@brief PORT peripheral base pointer */
#define DEBUG_UART_RX_PIN 1U                   /*!<@brief PORT pin number */
#define DEBUG_UART_RX_PIN_MASK (1U << 1U)      /*!<@brief PORT pin mask */
                                               /* @} */

/*! @name PORTC9 (number 66), J1[16]
  @{ */

/* Symbols to be used with GPIO driver */
#define IN1_MOTOR_FGPIO FGPIOC             /*!<@brief FGPIO peripheral base pointer */
#define IN1_MOTOR_GPIO GPIOC               /*!<@brief GPIO peripheral base pointer */
#define IN1_MOTOR_GPIO_PIN_MASK (1U << 9U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define IN1_MOTOR_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define IN1_MOTOR_PIN 9U                   /*!<@brief PORT pin number */
#define IN1_MOTOR_PIN_MASK (1U << 9U)      /*!<@brief PORT pin mask */
                                           /* @} */

/*! @name PORTA13 (number 33), J2[2]/D8
  @{ */

/* Symbols to be used with GPIO driver */
#define IN2_MOTOR_FGPIO FGPIOA              /*!<@brief FGPIO peripheral base pointer */
#define IN2_MOTOR_GPIO GPIOA                /*!<@brief GPIO peripheral base pointer */
#define IN2_MOTOR_GPIO_PIN_MASK (1U << 13U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define IN2_MOTOR_PORT PORTA                /*!<@brief PORT peripheral base pointer */
#define IN2_MOTOR_PIN 13U                   /*!<@brief PORT pin number */
#define IN2_MOTOR_PIN_MASK (1U << 13U)      /*!<@brief PORT pin mask */
                                            /* @} */

/*! @name PORTA4 (number 30), J1[10]/D4
  @{ */

/* Symbols to be used with GPIO driver */
#define IN3_MOTOR_FGPIO FGPIOA             /*!<@brief FGPIO peripheral base pointer */
#define IN3_MOTOR_GPIO GPIOA               /*!<@brief GPIO peripheral base pointer */
#define IN3_MOTOR_GPIO_PIN_MASK (1U << 4U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define IN3_MOTOR_PORT PORTA               /*!<@brief PORT peripheral base pointer */
#define IN3_MOTOR_PIN 4U                   /*!<@brief PORT pin number */
#define IN3_MOTOR_PIN_MASK (1U << 4U)      /*!<@brief PORT pin mask */
                                           /* @} */

/*! @name PORTD5 (number 78), J2[4]/D9
  @{ */

/* Symbols to be used with GPIO driver */
#define IN4_MOTOR_FGPIO FGPIOD             /*!<@brief FGPIO peripheral base pointer */
#define IN4_MOTOR_GPIO GPIOD               /*!<@brief GPIO peripheral base pointer */
#define IN4_MOTOR_GPIO_PIN_MASK (1U << 5U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define IN4_MOTOR_PORT PORTD               /*!<@brief PORT peripheral base pointer */
#define IN4_MOTOR_PIN 5U                   /*!<@brief PORT pin number */
#define IN4_MOTOR_PIN_MASK (1U << 5U)      /*!<@brief PORT pin mask */
                                           /* @} */

/*! @name PORTA5 (number 31), J1[12]/D5
  @{ */

/* Symbols to be used with PORT driver */
#define PWM1_MOTOR_PORT PORTA               /*!<@brief PORT peripheral base pointer */
#define PWM1_MOTOR_PIN 5U                   /*!<@brief PORT pin number */
#define PWM1_MOTOR_PIN_MASK (1U << 5U)      /*!<@brief PORT pin mask */
                                            /* @} */

/*! @name PORTC8 (number 65), J1[14]
  @{ */

/* Symbols to be used with PORT driver */
#define PWM2_MOTOR_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define PWM2_MOTOR_PIN 8U                   /*!<@brief PORT pin number */
#define PWM2_MOTOR_PIN_MASK (1U << 8U)      /*!<@brief PORT pin mask */
                                            /* @} */

/*! @name PORTC4 (number 61), J1[7]
  @{ */

/* Symbols to be used with PORT driver */
#define BT_TX_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BT_TX_PIN 4U                   /*!<@brief PORT pin number */
#define BT_TX_PIN_MASK (1U << 4U)      /*!<@brief PORT pin mask */
                                       /* @} */

/*! @name PORTC3 (number 58), J1[5]
  @{ */

/* Symbols to be used with PORT driver */
#define BT_RX_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BT_RX_PIN 3U                   /*!<@brief PORT pin number */
#define BT_RX_PIN_MASK (1U << 3U)      /*!<@brief PORT pin mask */
                                       /* @} */

/*! @name PORTC5 (number 62), J1[9]
  @{ */

/* Symbols to be used with GPIO driver */
#define BT_EN_FGPIO FGPIOC             /*!<@brief FGPIO peripheral base pointer */
#define BT_EN_GPIO GPIOC               /*!<@brief GPIO peripheral base pointer */
#define BT_EN_GPIO_PIN_MASK (1U << 5U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BT_EN_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BT_EN_PIN 5U                   /*!<@brief PORT pin number */
#define BT_EN_PIN_MASK (1U << 5U)      /*!<@brief PORT pin mask */
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
