/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: Bleeper board SPI driver implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
#include "board.h"
#include "spi-board.h"
#include "bcm2835.h"
#include <semaphore.h>

sem_t mutex;

void SpiInit( Spi_t *obj, PinNames mosi, PinNames miso, PinNames sclk, PinNames nss )
{

	GpioInit( &obj->Mosi, mosi, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_DOWN, 0 );
	GpioInit( &obj->Miso, miso, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_DOWN, 0 );
	GpioInit( &obj->Sclk, sclk, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_DOWN, 0 );

	//GpioInit( &obj->Nss, nss, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_UP, 1 );

   if (!bcm2835_init())
	{
	   puts("bcm2835_init failed. Are you running as root??\n");
	  return ;
	}

	if (!bcm2835_spi_begin())
	{
		puts("bcm2835_spi_begin failedg. Are you running as root??\n");
	  return ;
	}
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536); // The default
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default

	obj->Spi = 1;

	/* create, initialize semaphore */
	if( sem_init(&mutex,1,1) < 0)
	{
		printf("Err semaphore initilization\n");
	}
}

void SpiDeInit( Spi_t *obj )
{
	bcm2835_spi_end();
}

void SpiFormat( Spi_t *obj, int8_t bits, int8_t cpol, int8_t cpha, int8_t slave )
{
	//TODO
}

void SpiFrequency( Spi_t *obj, uint32_t hz )
{
	//TODO
}

uint16_t SpiInOut( Spi_t *obj, uint16_t outData )
{
	uint8_t ret = 0;

    if( ( obj == NULL ) || ( obj->Spi ) == NULL )
    {
        while( 1 );
    }
    
    sem_wait(&mutex);
    ret = bcm2835_spi_transfer(outData);
    sem_post(&mutex);

    return ret;

//    while( SPI_I2S_GetFlagStatus( obj->Spi, SPI_I2S_FLAG_TXE ) == RESET );
//    SPI_I2S_SendData( obj->Spi, outData );
//    while( SPI_I2S_GetFlagStatus( obj->Spi, SPI_I2S_FLAG_RXNE ) == RESET );
//    return SPI_I2S_ReceiveData( obj->Spi );
}

