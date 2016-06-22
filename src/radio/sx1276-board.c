/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: SX1276 driver specific target board functions implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "../board.h"
#include "../gpio.h"
#include "../spi-board.h"
#include "radio.h"
#include "sx1276/sx1276.h"
#include "sx1276-board.h"

/*!
 * Hardware Time base in us
 */
#define IRQ_TIMER                              500000 //us

/*!
 * Flag used to set the RF switch control pins in low power mode when the radio is not active.
 */
static bool RadioIsActive = false;

/*!
 * Radio driver structure initialization
 */
const struct Radio_s Radio =
{
    SX1276Init,
    SX1276GetStatus,
    SX1276SetModem,
    SX1276SetChannel,
    SX1276IsChannelFree,
    SX1276Random,
    SX1276SetRxConfig,
    SX1276SetTxConfig,
    SX1276CheckRfFrequency,
    SX1276GetTimeOnAir,
    SX1276Send,
    SX1276SetSleep,
    SX1276SetStby, 
    SX1276SetRx,
    SX1276StartCad,
    SX1276ReadRssi,
    SX1276Write,
    SX1276Read,
    SX1276WriteBuffer,
    SX1276ReadBuffer,
    SX1276SetMaxPayloadLength
};

void checkStatusRegister()
{
	char* b_irq_flag;
	uint8_t irq_flag;
	irq_flag = Radio.Read( REG_LR_IRQFLAGS );

	uint8_t dio_mapping_1 = SX1276Read( REG_DIOMAPPING1 );
	uint8_t dio_mapping_2 = SX1276Read( REG_DIOMAPPING2 );

	switch( SX1276.Settings.Modem )
	{
		case MODEM_FSK:
			{

			}
			break;
		case MODEM_LORA:
			{
				// DIO0
				// 00 --> RXDONE
				if( ( dio_mapping_1 & RFLR_DIOMAPPING1_DIO0_MASK ) == RFLR_DIOMAPPING1_DIO0_00 )
				{
					// RXDONE
					if( ( irq_flag & RFLR_IRQFLAGS_RXDONE_MASK) == RFLR_IRQFLAGS_RXDONE )
						SX1276OnDio0Irq();
				}
				// 01 --> TXDONE
				else if( ( dio_mapping_1 & RFLR_DIOMAPPING1_DIO0_MASK ) == RFLR_DIOMAPPING1_DIO0_01 )
				{
					// TXDONE
					if( ( irq_flag & RFLR_IRQFLAGS_TXDONE_MASK) == RFLR_IRQFLAGS_TXDONE )
						SX1276OnDio0Irq();
				}
				// 10 --> CADDONE
				else if( ( dio_mapping_1 & RFLR_DIOMAPPING1_DIO0_MASK ) == RFLR_DIOMAPPING1_DIO0_10 )
				{
					// CADDONE
					if( ( irq_flag & RFLR_IRQFLAGS_CADDONE_MASK) == RFLR_IRQFLAGS_CADDONE )
						SX1276OnDio0Irq();
				}

				// DIO1
				// 00 --> RXTIMEOUT
				if( ( dio_mapping_1 & RFLR_DIOMAPPING1_DIO1_MASK ) == RFLR_DIOMAPPING1_DIO1_00 )
				{
					// RXTIMEOUT
					if( ( irq_flag & RFLR_IRQFLAGS_RXTIMEOUT_MASK) == RFLR_IRQFLAGS_RXTIMEOUT )
						SX1276OnDio1Irq();
				}
				// 01 --> FHSSCHANGEDCHANNEL
				else if( ( dio_mapping_1 & RFLR_DIOMAPPING1_DIO1_MASK ) == RFLR_DIOMAPPING1_DIO1_01 )
				{
					// FHSSCHANGEDCHANNEL
					if( ( irq_flag & RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL_MASK) == RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL )
						SX1276OnDio1Irq();
				}
				// 10 --> CADDETECTED
				else if( ( dio_mapping_1 & RFLR_DIOMAPPING1_DIO1_MASK ) == RFLR_DIOMAPPING1_DIO1_10 )
				{
					// CADDETECTED
					if( ( irq_flag & RFLR_IRQFLAGS_CADDETECTED_MASK) == RFLR_IRQFLAGS_CADDETECTED )
						SX1276OnDio1Irq();
				}

				// DIO2
				// 00 --> FHSSCHANGEDCHANNEL
				if( ( dio_mapping_1 & RFLR_DIOMAPPING1_DIO2_MASK ) == RFLR_DIOMAPPING1_DIO2_00 )
				{
					// RXTIMEOUT
					if( ( irq_flag & RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL_MASK) == RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL )
						SX1276OnDio2Irq();
				}
				// 01 --> FHSSCHANGEDCHANNEL
				else if( ( dio_mapping_1 & RFLR_DIOMAPPING1_DIO2_MASK ) == RFLR_DIOMAPPING1_DIO2_01 )
				{
					// FHSSCHANGEDCHANNEL
					if( ( irq_flag & RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL_MASK) == RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL )
						SX1276OnDio2Irq();
				}
				// 10 --> FHSSCHANGEDCHANNEL
				else if( ( dio_mapping_1 & RFLR_DIOMAPPING1_DIO2_MASK ) == RFLR_DIOMAPPING1_DIO2_10 )
				{
					// FHSSCHANGEDCHANNEL
					if( ( irq_flag & RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL_MASK) == RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL )
						SX1276OnDio2Irq();
				}

				// DIO3
				// 00 --> CADDONE
				if( ( dio_mapping_1 & RFLR_DIOMAPPING1_DIO3_MASK ) == RFLR_DIOMAPPING1_DIO3_00 )
				{
					// CADDONE
					if( ( irq_flag & RFLR_IRQFLAGS_CADDONE_MASK) == RFLR_IRQFLAGS_CADDONE )
						SX1276OnDio3Irq();
				}
				// 01 --> VALIDHEADER
				else if( ( dio_mapping_1 & RFLR_DIOMAPPING1_DIO3_MASK ) == RFLR_DIOMAPPING1_DIO3_01 )
				{
					// VALIDHEADER
					if( ( irq_flag & RFLR_IRQFLAGS_VALIDHEADER_MASK) == RFLR_IRQFLAGS_VALIDHEADER )
						SX1276OnDio3Irq();
				}
				// 10 --> PAYLOADCRCERROR
				else if( ( dio_mapping_1 & RFLR_DIOMAPPING1_DIO3_MASK ) == RFLR_DIOMAPPING1_DIO3_10 )
				{
					// PAYLOADCRCERROR
					if( ( irq_flag & RFLR_IRQFLAGS_PAYLOADCRCERROR_MASK) == RFLR_IRQFLAGS_PAYLOADCRCERROR )
						SX1276OnDio3Irq();
				}

				// DIO4
				// 00 --> CADDETECTED
				if( ( dio_mapping_2 & RFLR_DIOMAPPING2_DIO4_MASK ) == RFLR_DIOMAPPING2_DIO4_00 )
				{
					// CADDETECTED
					if( ( irq_flag & RFLR_IRQFLAGS_CADDETECTED_MASK) == RFLR_IRQFLAGS_CADDETECTED )
						SX1276OnDio4Irq();
				}
				// 01 --> PLLLOCK
				// 10 --> PLLLOCK

				// DIO5
				// 00 --> MODEREADY
				// 01 --> CLKOUT
				// 10 --> CLKOUT

			}
			break;
	}

}

/*!
 * Antenna switch GPIO pins objects
 */
Gpio_t AntSwitchLf;
Gpio_t AntSwitchHf;

void SX1276IoInit( void )
{
    GpioInit( &SX1276.Spi.Nss, RADIO_NSS, PIN_OUTPUT, PIN_PUSH_PULL, PIN_PULL_UP, 1 );

    GpioInit( &SX1276.DIO0, RADIO_DIO_0, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );
//    GpioInit( &SX1276.DIO1, RADIO_DIO_1, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );
//    GpioInit( &SX1276.DIO2, RADIO_DIO_2, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );
//    GpioInit( &SX1276.DIO3, RADIO_DIO_3, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );
//    GpioInit( &SX1276.DIO4, RADIO_DIO_4, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );
//    GpioInit( &SX1276.DIO5, RADIO_DIO_5, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );
}

pthread_attr_t irq_attr;
struct sched_param irq_parm;
struct sigevent irq_sig;
timer_t irq_timerid;
struct itimerspec irq_in, irq_out;

void SX1276IoIrqInit( DioIrqHandler **irqHandlers )
{
//    int ret;
//
//    pthread_attr_init( &irq_attr );
//
//    irq_parm.sched_priority = 255;
//    pthread_attr_setschedparam(&irq_attr, &irq_parm);
//
//    irq_sig.sigev_notify = SIGEV_THREAD;
//    irq_sig.sigev_notify_function = checkStatusRegister;
//    irq_sig.sigev_value.sival_int =20;
//    irq_sig.sigev_notify_attributes = &irq_attr;
//
//    //create a new timer.
//    ret = timer_create(CLOCK_REALTIME, &irq_sig, &irq_timerid);
//    if (ret == 0)
//	{
//    	irq_in.it_value.tv_sec = 0;
//    	irq_in.it_value.tv_nsec = IRQ_TIMER * 1000;
//    	irq_in.it_interval.tv_sec = 0;
//    	irq_in.it_interval.tv_nsec = IRQ_TIMER * 1000;
//		//issue the periodic timer request here.
//		ret = timer_settime(irq_timerid, 0, &irq_in, &irq_out);
//		if(ret != 0)
//			printf("timer_settime() failed with %d\n", errno);
//	}


//    GpioSetInterrupt( &SX1276.DIO0, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, irqHandlers[0] );
//    GpioSetInterrupt( &SX1276.DIO1, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, irqHandlers[1] );
//    GpioSetInterrupt( &SX1276.DIO2, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, irqHandlers[2] );
//    GpioSetInterrupt( &SX1276.DIO3, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, irqHandlers[3] );
//    GpioSetInterrupt( &SX1276.DIO4, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, irqHandlers[4] );
//    GpioSetInterrupt( &SX1276.DIO5, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, irqHandlers[5] );
}

void SX1276IoIrqDeInit(  )
{
	timer_delete(irq_timerid);
}

void SX1276IoDeInit( void )
{
//    GpioInit( &SX1276.Spi.Nss, RADIO_NSS, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 1 );

//    GpioInit( &SX1276.DIO0, RADIO_DIO_0, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
//    GpioInit( &SX1276.DIO1, RADIO_DIO_1, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
//    GpioInit( &SX1276.DIO2, RADIO_DIO_2, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
//    GpioInit( &SX1276.DIO3, RADIO_DIO_3, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
//    GpioInit( &SX1276.DIO4, RADIO_DIO_4, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
//    GpioInit( &SX1276.DIO5, RADIO_DIO_5, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
}

uint8_t SX1276GetPaSelect( uint32_t channel )
{return RF_PACONFIG_PASELECT_PABOOST;
    if( channel < RF_MID_BAND_THRESH )
    {
        return RF_PACONFIG_PASELECT_PABOOST;
    }
    else
    {
        return RF_PACONFIG_PASELECT_RFO;
    }
}

void SX1276SetAntSwLowPower( bool status )
{
    if( RadioIsActive != status )
    {
        RadioIsActive = status;
    
        if( status == false )
        {
            SX1276AntSwInit( );
        }
        else
        {
            SX1276AntSwDeInit( );
        }
    }
}

void SX1276AntSwInit( void )
{
//    GpioInit( &AntSwitchLf, RADIO_ANT_SWITCH_LF, PIN_OUTPUT, PIN_PUSH_PULL, PIN_PULL_UP, 1 );
//    GpioInit( &AntSwitchHf, RADIO_ANT_SWITCH_HF, PIN_OUTPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );
}

void SX1276AntSwDeInit( void )
{
//    GpioInit( &AntSwitchLf, RADIO_ANT_SWITCH_LF, PIN_OUTPUT, PIN_OPEN_DRAIN, PIN_NO_PULL, 0 );
//    GpioInit( &AntSwitchHf, RADIO_ANT_SWITCH_HF, PIN_OUTPUT, PIN_OPEN_DRAIN, PIN_NO_PULL, 0 );
}

void SX1276SetAntSw( uint8_t rxTx )
{
    if( SX1276.RxTx == rxTx )
    {
        return;
    }

    SX1276.RxTx = rxTx;

    if( rxTx != 0 ) // 1: TX, 0: RX
    {
//        GpioWrite( &AntSwitchLf, 0 );
//        GpioWrite( &AntSwitchHf, 1 );
    }
    else
    {
//        GpioWrite( &AntSwitchLf, 1 );
//        GpioWrite( &AntSwitchHf, 0 );
    }
}

bool SX1276CheckRfFrequency( uint32_t frequency )
{
    // Implement check. Currently all frequencies are supported
    return true;
}
