/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: MCU RTC timer and low power modes management

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
#include <math.h>
#include "board.h"
#include "timer-board.h"

pthread_attr_t attr2, attr3;
struct sched_param parm2, parm3;
struct sigevent sig2, sig3;
timer_t timerid2, timerid3;
struct itimerspec in2, out2, in3, out3;
int i=0, k=0;
bool irq = false;

/*!
 * Hardware Time base in us
 */
#define HW_TIMER_TIME_BASE                              100 //us

/*!
 * Hardware Timer tick counter
 */
volatile TimerTime_t TimerTickCounter = 1;

/*!
 * Saved value of the Tick counter at the start of the next event
 */
static TimerTime_t TimerTickCounterContext = 0;

/*!
 * Value trigging the IRQ
 */
volatile TimerTime_t TimeoutCntValue = 0;

/*!
 * Increment the Hardware Timer tick counter
 */
void TimerIncrementTickCounter( void );

/*!
 * Counter used for the Delay operations
 */
volatile uint32_t TimerDelayCounter = 0;

/*!
 * Return the value of the counter used for a Delay
 */
uint32_t TimerHwGetDelayValue( void );

/*!
 * Increment the value of TimerDelayCounter
 */
void TimerIncrementDelayCounter( void );

//struct itimerval curr_value;
//struct itimerval new_value;
//struct itimerval old_value;

void TIM2_IRQHandler( union sigval val );
void TIM3_IRQHandler( union sigval val );

void TimerHwInit( void )
{
    TimerDelayCounter = 0;
    TimeoutCntValue = 0;
    
    int ret2, ret3;

    pthread_attr_init( &attr2 );

    parm2.sched_priority = 255;
    pthread_attr_setschedparam(&attr2, &parm2);

    sig2.sigev_notify = SIGEV_THREAD;
    sig2.sigev_notify_function = TIM2_IRQHandler;
    sig2.sigev_value.sival_int =20;
    sig2.sigev_notify_attributes = &attr2;

    //create a new timer.
    ret2 = timer_create(CLOCK_REALTIME, &sig2, &timerid2);
    if (ret2 == 0)
	{
		in2.it_value.tv_sec = 0;
		in2.it_value.tv_nsec = HW_TIMER_TIME_BASE * 1000;
		in2.it_interval.tv_sec = 0;
		in2.it_interval.tv_nsec = HW_TIMER_TIME_BASE * 1000;
		//issue the periodic timer request here.
		ret2 = timer_settime(timerid2, 0, &in2, &out2);
		if(ret2 != 0)
			printf("timer_settime() failed with %d\n", errno);
	}
    else
    	printf("timer_create() failed with %d\n", errno);


    pthread_attr_init( &attr3 );

	parm3.sched_priority = 255;
	pthread_attr_setschedparam(&attr3, &parm3);

	sig3.sigev_notify = SIGEV_THREAD;
	sig3.sigev_notify_function = TIM3_IRQHandler;
	sig3.sigev_value.sival_int =20;
	sig3.sigev_notify_attributes = &attr3;

	//create a new timer.
	ret3 = timer_create(CLOCK_REALTIME, &sig3, &timerid3);
	if (ret3 == 0)
	{
		in3.it_value.tv_sec = 0;
		in3.it_value.tv_nsec = HW_TIMER_TIME_BASE * 1000 * 10;
		in3.it_interval.tv_sec = 0;
		in3.it_interval.tv_nsec = HW_TIMER_TIME_BASE * 1000 * 10;
		//issue the periodic timer request here.
		ret3 = timer_settime(timerid3, 0, &in3, &out3);
		if(ret3 != 0)
			printf("timer_settime() failed with %d\n", errno);
	}
	else
		printf("timer_create() failed with %d\n", errno);


//    NVIC_InitTypeDef NVIC_InitStructure;
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//
//    /* TIM2 clock enable */
//    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );
//
//    /* --------------------------NVIC Configuration -------------------------------*/
//    /* Enable the TIM2 gloabal Interrupt */
//    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//
//    NVIC_Init( &NVIC_InitStructure );



//    /* Time base configuration */
//    TIM_TimeBaseStructure.TIM_Period = 3199;
//    TIM_TimeBaseStructure.TIM_Prescaler = 0;
//    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//    TIM_TimeBaseInit( TIM2, &TIM_TimeBaseStructure );
//
//    TIM_ITConfig( TIM2, TIM_IT_Update, ENABLE );
//
//    /* TIM2 disable counter */
//    TIM_Cmd( TIM2, ENABLE );
//
//        /* TIM3 clock enable */
//    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );
//
//    /* --------------------------NVIC Configuration -------------------------------*/
//    /* Enable the TIM3 gloabal Interrupt */
//    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//
//    NVIC_Init( &NVIC_InitStructure );
//
//    /* Time base configuration */
//    TIM_TimeBaseStructure.TIM_Period = 3199;
//    TIM_TimeBaseStructure.TIM_Prescaler = 10;
//    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//    TIM_TimeBaseInit( TIM3, &TIM_TimeBaseStructure );
//
//    TIM_ITConfig( TIM3, TIM_IT_Update, DISABLE );
//
//    /* TIM3 disable counter */
//    TIM_Cmd( TIM3, DISABLE );
//
//
//    TIM_ITConfig( TIM3, TIM_IT_Update, ENABLE );
//    TIM_Cmd( TIM3, ENABLE );
}

void TimerHwDeInit( void )
{
//    /* Deinitialize the timer */
//    TIM_DeInit( TIM2 );

	timer_delete(timerid2);
	timer_delete(timerid3);

}

uint32_t TimerHwGetMinimumTimeout( void )
{
    return( ceil( 2 * HW_TIMER_TIME_BASE ) );
}

void TimerHwStart( uint32_t val )
{
    TimerTickCounterContext = TimerHwGetTimerValue( );

    if( val <= HW_TIMER_TIME_BASE + 1 )
    {
        TimeoutCntValue = TimerTickCounterContext + 1;
    }
    else
    {
        TimeoutCntValue = TimerTickCounterContext + ( ( val - 1 ) / HW_TIMER_TIME_BASE );
    }
}

void TimerHwStop( void )
{
//    TIM_ITConfig( TIM2, TIM_IT_CC1, DISABLE );
//    TIM_Cmd( TIM2, DISABLE );

	int ret2;

	in2.it_value.tv_sec = 0;
	in2.it_value.tv_nsec = 0;
	in2.it_interval.tv_sec = 0;
	in2.it_interval.tv_nsec = 0;
	//issue the periodic timer request here.
	ret2 = timer_settime(timerid2, 0, &in2, &out2);
	if(ret2 != 0)
		printf("timer_settime() failed with %d\n", errno);
}

void TimerHwReStart( void )
{
//    TIM_ITConfig( TIM2, TIM_IT_CC1, DISABLE );
//    TIM_Cmd( TIM2, DISABLE );

	int ret2;

	in2.it_value.tv_sec = 0;
	in2.it_value.tv_nsec = HW_TIMER_TIME_BASE * 1000;
	in2.it_interval.tv_sec = 0;
	in2.it_interval.tv_nsec = HW_TIMER_TIME_BASE * 1000;
	//issue the periodic timer request here.
	ret2 = timer_settime(timerid2, 0, &in2, &out2);
	if(ret2 != 0)
		printf("timer_settime() failed with %d\n", errno);
}

void TimerHwDelayMs( uint32_t delay )
{
    uint32_t delayValue = 0;
    uint32_t timeout = 0;

    delayValue = delay;
    // Wait delay ms
    timeout = TimerHwGetDelayValue( );
    while( ( TimerHwGetDelayValue( ) - timeout ) < delayValue )
    {
    	((void)0);
    }
}

TimerTime_t TimerHwGetElapsedTime( void )
{
     return( ( ( TimerHwGetTimerValue( ) - TimerTickCounterContext ) + 1 )  * HW_TIMER_TIME_BASE );
}

TimerTime_t TimerHwGetTimerValue( void )
{
    TimerTime_t val = 0;

    __disable_irq( );

    val = TimerTickCounter;

    __enable_irq( );

    return( val );
}

TimerTime_t TimerHwGetTime( void )
{

    return TimerHwGetTimerValue( ) * HW_TIMER_TIME_BASE;
}

uint32_t TimerHwGetDelayValue( void )
{
    uint32_t val = 0;

    __disable_irq( );

    val = TimerDelayCounter;

    __enable_irq( );

    return( val );
}

void TimerIncrementTickCounter( void )
{
    __disable_irq( );

    TimerTickCounter++;

    __enable_irq( );
}

void TimerIncrementDelayCounter( void )
{
    __disable_irq( );

    TimerDelayCounter++;

    __enable_irq( );
}

/*!
 * Timer IRQ handler
 */
void TIM2_IRQHandler( union sigval val )
{
//	printf("\nTIM2_IRQHandler: %d for %d times\n", val.sival_int, ++i);

	if(irq == true)
	{
		TimerIncrementTickCounter( );
		if( TimerTickCounter == TimeoutCntValue )
		{
			TimerIrqHandler( );
//			printf("\TimerIrqHandler");
		}
	}

//    if( TIM_GetITStatus( TIM2, TIM_IT_Update ) != RESET )
//    {
//        TimerIncrementTickCounter( );
//        TIM_ClearITPendingBit( TIM2, TIM_IT_Update );
    
//        if( TimerTickCounter == TimeoutCntValue )
//        {
//            TimerIrqHandler( );
//        }
//    }
}

/*!
 * Timer IRQ handler
 */
void TIM3_IRQHandler( union sigval val )
{
//	printf("\nTIM3_IRQHandler: %d for %d times\n", val.sival_int, ++k);

	if(irq == true)
	{
		TimerIncrementDelayCounter( );
	}

//    if( TIM_GetITStatus( TIM3, TIM_IT_Update ) != RESET )
//    {
//        TimerIncrementDelayCounter( );
//        TIM_ClearITPendingBit( TIM3, TIM_IT_Update );
//    }
}

void TimerHwEnterLowPowerStopMode( void )
{
//#ifndef USE_DEBUGGER
//    __WFI( );
//#endif
}

void __disable_irq( )
{
	irq = false;
}

void __enable_irq( )
{
	irq = true;
}

