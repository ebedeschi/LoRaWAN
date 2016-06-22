/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: Generic GPIO driver implementation

Comment: Relies on the specific board GPIO implementation as well as on
         IO expander driver implementation if one is available on the target
         board.

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
#include "board.h"
#include "bcm2835.h"

/*!
 * Flag to indicate if the Gpio is Initialized
 */
static bool GpioInitialized = false;

void GpioInit( Gpio_t *obj, PinNames pin, PinModes mode,  PinConfigs config, PinTypes type, uint32_t value )
{
	if (GpioInitialized == false)
		if (!bcm2835_init())
		  return ;
	GpioInitialized = true;

    obj->pin = pin;

	switch(mode)
	{
		case PIN_INPUT:
			bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);
		break;
		case PIN_OUTPUT:
			bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
		break;
		case PIN_ALTERNATE_FCT:
			// no
		break;
		case PIN_ANALOGIC:
			// no
		break;
	}
	switch(config)
	{
		case PIN_PUSH_PULL:
			switch(type)
			{
				case PIN_NO_PULL:
					bcm2835_gpio_set_pud(pin, BCM2835_GPIO_PUD_OFF);
				break;
				case PIN_PULL_UP:
					bcm2835_gpio_set_pud(pin, BCM2835_GPIO_PUD_UP);
				break;
				case PIN_PULL_DOWN:
					bcm2835_gpio_set_pud(pin, BCM2835_GPIO_PUD_DOWN);
				break;
			}
		break;
		case PIN_OPEN_DRAIN:
			// no
		break;
	}

	if( value == 0 )
		bcm2835_gpio_write(pin, LOW);
	else
		bcm2835_gpio_write(pin, HIGH);

}

void GpioSetInterrupt( Gpio_t *obj, IrqModes irqMode, IrqPriorities irqPriority, GpioIrqHandler *irqHandler )
{
	//TODO
//    // Set RPI pin P1-15 to be an input
//    bcm2835_gpio_fsel(obj->pin, BCM2835_GPIO_FSEL_INPT);
//    //  with a pullup
//    bcm2835_gpio_set_pud(obj->pin, BCM2835_GPIO_PUD_DOWN);
//    // And a low detect enable
//    bcm2835_gpio_hen(obj->pin);
}

void GpioRemoveInterrupt( Gpio_t *obj )
{
	//TODO
}

void GpioWrite( Gpio_t *obj, uint32_t value )
{
	if( value == 0 )
		bcm2835_gpio_write(obj->pin, LOW);
	else
		bcm2835_gpio_write(obj->pin, HIGH);
}

uint32_t GpioRead( Gpio_t *obj )
{
	return bcm2835_gpio_lev(obj->pin);
}
