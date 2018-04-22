
#ifndef ADC_H
#define ADC_H

/**
 *	
 	@file adc.h
 	@code #include <adc.h> @endcode

	@brief	AD-Converter functions for ATMega168 (ATMega328).

		- 7 ADC channels.

	@author J.Karlbom
 */


/**
 *	
	@brief Definitions for average value
		- AVERAGE_BITS
		- AVERAGE_SIZE
 */

#define AVERAGE_BITS 2					//!< Constant for oversampled function
#define AVERAGE_SIZE 1<<2*AVERAGE_BITS	//!< Constant for oversampled function


/**
 *	
	@brief	Initilize ADC conversion.
	@return	void
 */
void initADC(void);


/**
 *	
	@brief	Start ADC conversion and read a result.
	 Starts ADC conversion and waits until the conversion is done.
	@param	channel Singled ended ACD channel to read (0 ... 7)
	@return	ADC result 0...1024

	Starts ADC conversion and waits until the conversion is done.

 */
uint16_t readADC(uint8_t channel);


/**
 *	
	@brief	Using oversampling to increase ADC converter resolution (10bit to 12bit).
	@param	channel Singled ended ACD channel to read (0 ... 7)
	@return Oversampled value of ADC result.
	
	Starts ADC conversion and waits until the conversion is done.
 */
uint16_t readADC_12bit(uint8_t channel);


#endif //ADC_H
