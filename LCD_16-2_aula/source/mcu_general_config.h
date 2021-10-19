/**
 * @file	mcu_general_config.h
 * @author  Matheus Leitzke Pinto <matheus.pinto@ifsc.edu.br>
 * @version 1.0
 * @date    2021
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * The general microcontroller configuration and definitions for use
 * in the whole system.
 *
 */

#ifndef MCU_GENERAL_CONFIG_H_
#define MCU_GENERAL_CONFIG_H_

/* TODO: insert other definitions and declarations here. */
#include <stdint.h>
#include "MKL25Z4.h"
#include "fsl_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @addtogroup mcu_general_config
 * @{
 */


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!< If FreeRTOS is present, uncomment this macro.*/
//#define __FREERTOS_H

/*!< MCU specific port register type for write or read logic values in pins.*/
typedef GPIO_Type* portPinsRegister_t;
/*!< Mask type with the MCU ports width.*/
typedef uint32_t portPinMask_t;


/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * @brief Set the port pins.
 *
 * @param portPinsRegister - port register.
 * @param portPinMask      - pin mask, bits equal 1 set the correspondingly pin.
 *
 */
#define MCU_PortSet(portPinsRegister, portPinMask) portPinsRegister->PSOR = portPinMask;


/**
 * @brief Clear the port pins.
 *
 * @param portPinsRegister - port register.
 * @param portPinMask      - pin mask, bits equal 1 clear the correspondingly pin.
 *
 */
#define MCU_PortClear(portPinsRegister, portPinMask) portPinsRegister->PCOR = portPinMask;

#ifdef __cplusplus
}  /* extern "C" */
#endif

/*! @}*/

#endif /* MCU_GENERAL_CONFIG_H_ */
