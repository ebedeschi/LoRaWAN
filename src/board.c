/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: Target board general functions implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Andreas Pella (IMST GmbH), Miguel Luis and Gregory Cristian
*/
#include "board.h"

/*!
 * Potentiometer max and min levels definition
 */
#define POTI_MAX_LEVEL 900
#define POTI_MIN_LEVEL 10

/*!
 * Vref values definition
 */
#define PDDADC_VREF_BANDGAP                             1224 // mV
#define PDDADC_MAX_VALUE                                4096

/*!
 * Battery level ratio (battery dependent)
 */
#define BATTERY_STEP_LEVEL                          0.23


Gpio_t Led1, Led2;



/*!
 * Initializes the unused GPIO to a know status
 */
static void BoardUnusedIoInit( void );

/*!
 * Flag to indicate if the MCU is Initialized
 */
static bool McuInitialized = false;

void BoardInitPeriph( void )
{

    GpioInit( &Led1, RPI_V2_GPIO_P1_11, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );

    GpioWrite( &Led1, 1 );

    GpioInit( &Led2, RPI_V2_GPIO_P1_13, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );

    GpioWrite( &Led2, 1 );

//    __enable_irq( );

}

void BoardInitMcu( void )
{
    if( McuInitialized == false )
    {

        SpiInit( &SX1276.Spi, RADIO_MOSI, RADIO_MISO, RADIO_SCLK, RADIO_NSS );

        SX1276IoInit( );

        TimerHwInit();


        McuInitialized = true;
    }
}

void BoardDeInitMcu( void )
{
    SpiDeInit( &SX1276.Spi );
    SX1276IoDeInit( );
    TimerHwDeInit();
    
    McuInitialized = false;
}

uint32_t BoardGetRandomSeed( void )
{
    return 0;
}

void BoardGetUniqueId( uint8_t *id )
{
    id[7] = 0;
    id[6] = 0;
    id[5] = 0;
    id[4] = 0;
    id[3] = 0;
    id[2] = 0;
    id[1] = 0;
    id[0] = 0;
}

uint8_t BoardMeasurePotiLevel( void ) 
{
    return 0;
}

uint16_t BoardMeasureVdd( void ) 
{
    return ( uint16_t ) 0;
}

uint8_t BoardGetBatteryLevel( void )
{
    return 0;
}

static void BoardUnusedIoInit( void )
{

}

