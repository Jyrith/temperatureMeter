
/**
 *	@file temperatureMeter.c
 *	@defgroup jyrith_main Main code
 *
 *	@brief	Waits command from BT and sends response back.
 *
 *		Read temperature values using internal ADC of atmega168/atmega328.
 *
 *		LM50 temperature sensor, 500mV output at 0°C, 10mV/°C
 *		- Sensors are calibrated at 0°C in water-ice mixture
 *		- Calibrated values are saved to internal EEPROM and read in start-up
 *		- External 1,234V voltage reference is needed for ADC (LM385 1.2).
 *		
 *		- Bluetooth serial module is connected to UART1 of atmega168/atmega328.
 *		- Arduino Mini Pro 3.3V 8MHz versio is needed when operated from single 
 *			Li-Ion cell. 
 *			Note:
 *			At least some of clones of Arduino Mini Pro uses a AMS1117 regulator which
 *			dropout voltage is 1.1V so these will have problems when suplied from
 *			single Li-Ion cell. Original design uses MIC5205 which has a much lower
 *			dropout voltage (165mV at 150mA).
 *		
 *		- avr-gcc (WinAVR 20100110) 4.3.3
 *		- tabwidth 4
 *	
 *
 *	@author J.Karlbom
 *	@version   1.0
 */

/**@{*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "adc.h"
#include "uart.h"
#include "tools.h"
#include "temperatureMeter.h"


/**
 *	@brief	Handle command send from serial port.
	@param	command Received ccommand 
	@return	none
 */
static void handleCommand(uint8_t command);

/**
 *	@brief	Send a reply via serial port.
		- Command
		- Payload size
		- ADC results
	@param	command
	@param	result
	@return	none
 */
static void sendACDReply(uint8_t command, uint16_t result);

/**
 *	
	@brief	Send a reply to a command via serial port.
		- Command
		- Payload size
		- On/Off info
	@param	command Echo command back
	@return	none
 */
static void sendLEDReply(uint8_t command);


/**
 *	
	@brief	Calibrate temperature sensors at 0°C degree.

		- Calibration is done by putting sensors at 0°C water-ice mixture
		- And after sensors are stabilized values are read
		- And saved to an internal EEPROM
		- Only lower part of ADC result is saved to EEPROM (uint8_t)
	@return	none
 */
static void calibrate();

/**
 *	@brief	CRC-8 calculation 
	@param	bytes[] Message array
	@param	size  Size of the message array
	@return	CRC value
 */
static uint8_t crc8Simple(uint8_t bytes[], uint8_t size);

/**
 *	@brief	Sends a message to serial port
	@param	msg[] Message array
	@param	size  Size of the message array
	@return	none
 */
static void sendMsg(uint8_t msg[], uint8_t size);



uint8_t msgData[MSG_SIZE];	//!< Array for message to send

// internal eeprom, EEMEM address starting from location 0
uint8_t EEMEM ee_CalibrationSensor_1;	//!< EEMEM address for 1. sensor
uint8_t EEMEM ee_CalibrationSensor_2;	//!< EEMEM address for 2. sensor

uint8_t calibSensor_1;		//!< Holds a 1. calibration value, read from eeprom
uint8_t calibSensor_2;		//!< Holds a 2. calibration value, read from eeprom


/**
 *	Main
 */
int main (void)
	{    

    /*	Initialize ADC 
	*/	
	initADC();

    /*	Initialize UART library, 
		pass baudrate and AVR cpu clock 
	*/
    uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) ); 

	// Set LED pin as output.
	LED_DDR |= (1<<LED_PIN);


	uint16_t commandFromSerial;

	calibSensor_1 = eeprom_read_byte (&ee_CalibrationSensor_1);
	calibSensor_2 = eeprom_read_byte (&ee_CalibrationSensor_2);
    
    /*	Enable interrupts
	*/
    sei();

	
	// Main loop
	for(;;)
		{
		commandFromSerial = uart_getc();
		// Check if command is available from UART
		if (! (commandFromSerial & UART_NO_DATA ))	
			{
			handleCommand(commandFromSerial);
			}
		}
	}


static void handleCommand(uint8_t command)
	{
	uint16_t adcValue = 0;
	switch (command) 
		{
		case CMD_ADC0:
		adcValue = readADC(ADC_CH_0);
		sendACDReply(CMD_ADC0, adcValue);
		break;

		case CMD_ADC1:
		adcValue = readADC(ADC_CH_1);
		sendACDReply(CMD_ADC1, adcValue);
		break;

		case CMD_ADC2:
		adcValue = readADC(ADC_CH_2);
		sendACDReply(CMD_ADC2, adcValue);
		break;

		case CMD_ADC3:
		adcValue = readADC(ADC_CH_3);
		sendACDReply(CMD_ADC3, adcValue);
		break;

		case CMD_ADC_CALIB:
		calibrate();
		break;

		case CMD_LED_ON:
		BitSet(LED_PORT,LED_PIN);
		sendLEDReply(CMD_LED_ON);
		break;

		case CMD_LED_OFF:
		BitClear(LED_PORT,LED_PIN);
		sendLEDReply(CMD_LED_OFF);
		break;
		}
	}


static void sendACDReply(uint8_t command, uint16_t result)
	{
	msgData[0] = command;
	msgData[1] = ADC_SIZE;
	if(command == ADC_CH_0)
		msgData[2] = calibSensor_1;
	else if(command == ADC_CH_1)
		msgData[2] = calibSensor_2;
	else 	// Other than temperature sensor
		msgData[2] = NOT_TEMP;
	msgData[3] =  (uint8_t) ((0xFF00 & result) >> 8);
	msgData[4] =  (uint8_t) 0x00FF & result;
	msgData[5] = crc8Simple(msgData, ADC_SIZE);
	sendMsg (msgData, ADC_SIZE + 1);
	}

static void sendLEDReply(uint8_t command)
	{
	msgData[0] = command;
	msgData[1] = LED_SIZE;
	if(command == CMD_LED_ON)
		msgData[2] = LED_ON;
	else
		msgData[2] = LED_OFF;
	msgData[3] = crc8Simple(msgData, LED_SIZE);
	sendMsg (msgData, LED_SIZE + 1);
	}

static void calibrate()
	{
	uint8_t adcValue = 0;
	adcValue = 0xFF & readADC(ADC_CH_0);
	eeprom_update_byte(&ee_CalibrationSensor_1, adcValue);
	adcValue = 0xFF & readADC(ADC_CH_1);
	eeprom_update_byte(&ee_CalibrationSensor_2, adcValue);
	}


static uint8_t crc8Simple(uint8_t bytes[], uint8_t size)
	{
	const uint8_t polynomial = 0x07;	// CRC-8-CCITT
	uint8_t crc = 0;
	uint8_t i,j;

	for (j = 0; j < size; j++)
		{
		crc ^= bytes[j];
		for (i = 0; i < 8; i++)
			{
			if ((crc & 0x80) != 0)
				{
				crc = (uint8_t)((crc << 1) ^ polynomial);
				}
			else
				{
				crc <<= 1;
				}
			}
		}
	return crc;
	}


static void sendMsg(uint8_t msg[], uint8_t size)
	{
	uint8_t index;
	for (index = 0; index <= size; index++)
		uart_putc(msg[index]);
	}

/**@}*/
