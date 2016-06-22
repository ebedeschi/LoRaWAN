/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: Target board general functions implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "timer.h"
#include "delay.h"
#include "gpio.h"
#include "spi.h"
#include "timer-board.h"
#include "radio/radio.h"
#include "radio/sx1276/sx1276.h"

#define USE_BAND_868

extern SX1276_t SX1276;

#if defined( USE_USB_CDC )
#include "usb-cdc-board.h"
#endif

/*!
 * Define indicating if an external IO expander is to be used
 */
#define BOARD_IOE_EXT

/*!
 * Generic definition
 */
#ifndef SUCCESS
#define SUCCESS                                     1
#endif

#ifndef FAIL
#define FAIL                                        0  
#endif


/*!
 * Board MCU pins definitions
 */

#define RADIO_RESET                                 RPI_V2_GPIO_P1_12

#define RADIO_MOSI                                  RPI_V2_GPIO_P1_19
#define RADIO_MISO                                  RPI_V2_GPIO_P1_21
#define RADIO_SCLK                                  RPI_V2_GPIO_P1_23
#define RADIO_NSS                                   RPI_V2_GPIO_P1_24

#define RADIO_ANT_SWITCH_LF							31
#define RADIO_ANT_SWITCH_HF							31

#define RADIO_DIO_0                                 22
#define RADIO_DIO_1                                 31
#define RADIO_DIO_2                                 31
#define RADIO_DIO_3                                 31
#define RADIO_DIO_4                                 31
#define RADIO_DIO_5                                 31

#define RADIO_ANT_SWITCH_RX                         31
#define RADIO_ANT_SWITCH_TX                         31

#define OSC_LSE_IN                                  31
#define OSC_LSE_OUT                                 31

#define OSC_HSE_IN                                  31
#define OSC_HSE_OUT                                 31

#define USB_DM                                      31
#define USB_DP                                      31

#define JTAG_TMS                                    31
#define JTAG_TCK                                    31
#define JTAG_TDI                                    31
#define JTAG_TDO                                    31
#define JTAG_NRST                                   31

#define I2C_SCL                                     31
#define I2C_SDA                                     31

#define CON_EXT_1                                   31
#define CON_EXT_3                                   31
#define CON_EXT_7                                   31
#define CON_EXT_8                                   31
#define CON_EXT_9                                   31
#define BAT_LEVEL                                   31

#define BOOT_1                                      31
    
#define GPS_PPS                                     31
#define UART_TX                                     31
#define UART_RX                                     31

#define PIN_PB6                                     31
#define WKUP1                                       31


/*!
 * LED GPIO pins objects
 */
extern Gpio_t IrqMpl3115;
extern Gpio_t IrqMag3110;
extern Gpio_t GpsPowerEn;
extern Gpio_t NcIoe3;
extern Gpio_t NcIoe4;
extern Gpio_t NcIoe5;
extern Gpio_t NcIoe6;
extern Gpio_t NcIoe7;
extern Gpio_t NIrqSX9500;
extern Gpio_t Irq1Mma8451;
extern Gpio_t Irq2Mma8451;
extern Gpio_t TxEnSX9500;
extern Gpio_t Led1;
extern Gpio_t Led2;
extern Gpio_t Led3;


/*!
 * Debug GPIO pins objects
 */
#if defined( USE_DEBUG_PINS )
extern Gpio_t DbgPin1;
extern Gpio_t DbgPin2;
extern Gpio_t DbgPin3;
extern Gpio_t DbgPin4;
#endif

/*!
 * MCU objects
 */
#if defined( USE_USB_CDC )
extern Uart_t UartUsb;
#endif

extern Gpio_t GpsPps;
extern Gpio_t GpsRx;
extern Gpio_t GpsTx;

/*!
 * \brief Initializes the target board peripherals.
 */
void BoardInitMcu( void );

/*!
 * \brief Initializes the boards peripherals.
 */
void BoardInitPeriph( void );

/*!
 * \brief De-initializes the target board peripherals to decrease power
 *        consumption.
 */
void BoardDeInitMcu( void );

/*!
 * \brief Get the current battery level
 *
 * \retval value  battery level ( 0: very low, 254: fully charged )
 */
uint8_t BoardGetBatteryLevel( void );

/*!
 * Returns a pseudo random seed generated using the MCU Unique ID
 *
 * \retval seed Generated pseudo random seed
 */
uint32_t BoardGetRandomSeed( void );

/*!
 * \brief Gets the board 64 bits unique ID 
 *
 * \param [IN] id Pointer to an array that will contain the Unique ID
 */
void BoardGetUniqueId( uint8_t *id );

#endif // __BOARD_H__
