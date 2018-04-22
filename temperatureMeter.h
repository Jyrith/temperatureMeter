
#ifndef TEMPERATUREMETER_H
#define TEMPERATUREMETER_H


/**
 *	@file temperatureMeter.h
 *
 *	@brief	Waits command from BT and based of the recieved command
 *	sends measured voltage, current, power value back.
 *	
 */


#ifndef F_CPU
#define F_CPU 8000000UL		//!< define CPU frequency in Mhz here if not defined in Makefile
#endif

#define UART_BAUD_RATE	19200	//!< Baud rate for serial port

#define CMD_ADC0 0x20		//!< Command, ADC converter channel 0, LM50
#define CMD_ADC1 0x21		//!< Command, ADC converter channel 1, LM50
#define CMD_ADC2 0x22		//!< Command, ADC converter channel 2
#define CMD_ADC3 0x23		//!< Command, ADC converter channel 3, battery voltage level measurement
#define CMD_ADC_CALIB 0x30	//!< Command, ADC sensors calibration

#define ADC_SIZE 0x04		//!< ADC converter payload data size, 3 bytes + crc

#define CMD_LED_ON 0x40		//!< Command, Set pin PB5, Green LED
#define CMD_LED_OFF 0x41	//!< Command, Clear pin PB5, Green LED

#define LED_SIZE 0x02		//!< LED command payload data size, 1 byte + crc

#define LED_ON 0x00			//!< Reply status of Green LED On
#define LED_OFF 0x01		//!< Reply status of Green LED Off

#define NOT_TEMP 0xFF		//!< Reply fill byte for other that temperature sensor

/**	
 *	@brief	Bit-mask variable
 */

volatile uint8_t globalBitStatus = 0;

/**
 *	@brief	ADC settings
 *		- I/O Control settings for LED
 */
#define ADC_PORT PORTC		//!< Port, PortC
#define ADC_CH_0 0			//!< ADC 0
#define ADC_CH_1 1			//!< ADC 1
#define ADC_CH_2 2			//!< ADC 2
#define ADC_CH_3 3			//!< ADC 3

#define LED_PORT	PORTB	//!< For indication LED
#define LED_DDR		DDRB	//!< For indication LED
#define LED_PIN		PB5		//!< For indication LED

#define SAMPLE_COUNT	16
#define BIT_SHIFT		4

#define MSG_SIZE	6		//!< Size of a message array



#endif //TEMPERATUREMETER_H
