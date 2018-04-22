
#include <avr/io.h>

#include "adc.h"
#include "tools.h"


void initADC(void)
{
	// Voltage reference is AREF pin,
	// Internal Vref turned off, Default state on ADMUX
	ADMUX = 0x00;
	
	// ADC prescaler 125 kHz, 8MHz/64 -> 125kHz, Enable ADC.
	ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADEN);

	// Disable Digital Input buffers from ch0 to ch3
	DIDR0 = (1 << ADC0D) | (1 << ADC1D) | (1 << ADC2D) | (1 << ADC3D);
}


uint16_t readADC(uint8_t channel)
{

	uint8_t temp = ADMUX;	// Read current ADMUX settings
	temp &= ~0x0F;			// clear bits MUX0...MUX3

	if (channel < 8 )
	{
		// ADC0...ADC7
		temp |= channel;	// set a new channnel
		ADMUX = temp;		// Write a new value to ADMUX
	}
	else
		return 0;

	// Start ADC conversion
	ADCSRA |= (1 << ADSC);

	// Wait until conversion is done.
	// ADSC will read as one as long as a conversion is in progress
	while (ADCSRA & (1 << ADSC))
		;
	
	// Read value as 16 bit (avr complier)
	int16_t adcValue = ADC;
	return adcValue;
}


uint16_t readADC_12bit(uint8_t channel)
{
	int16_t average = 0;
	for (int8_t i=0;i<AVERAGE_SIZE;i++)
		average += readADC(channel);
	average = average>>AVERAGE_BITS;	// divide by 4 to get 12 bit oversampled value
	return average;
}

