
#ifndef TOOLS_H
#define TOOLS_H


/**
 	@file tools.h
	@defgroup jyrith_tools Tools
 	@code #include <tools.h> @endcode

	@brief	helper functions 

		- Bit-mask operation
 
 	@author J.Karlbom
 */


#define BitSet(port,bit) ((port) |= (1<< (bit)))	//!< Sets one bit of defined port to 1
#define BitClear(port,bit) ((port) &= ~(1<< (bit)))	//!< Clear one bit of defined port to 0
#define BitCheck(port,bit) ((port) & (1<< (bit)))	//!< Checks if bit of defined port is set (1)

/**
 *	Bit-mask variables
 */

extern volatile uint8_t globalBitStatus;	
#define READ_AD 0		// Bit defines when ADC result needs to read

#endif //TOOLS_H
