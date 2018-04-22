
## BT Temperature meter
Waits command from BT and sends response back.
Read temperature values using internal ADC of atmega168/atmega328.

### BOM
- Arduino Pro Mini Board
- Bluetooth serial module
- LM50 temperature sensor


#### LM50 temperature sensor, 500mV output at 0°C, 10mV/°C
 - Sensors can calibrate at 0°C in water-ice mixture
 - Calibrated values are saved to internal EEPROM and read in start-up
 - External 1,2V voltage reference is needed for ADC (LM385 1.2).

 - Bluetooth serial module is connected to UART1 of atmega168/atmega328.
 - Arduino Mini Pro 3.3V 8MHz versio is needed when operated from single Li-Ion cell. 

### Note:
At least some of clones of Arduino Mini Pro uses a AMS1117 regulator which dropout voltage is 1.1V so these will have problems when supplied from single Li-Ion cell. Original design uses MIC5205 which has a much lower dropout voltage (165mV at 150mA). It can be easily check just by measure voltage drop of the regulator or from markings of the regulator.

#### Complier & Editor Info
- avr-gcc (WinAVR 20100110) 4.3.3
- tabwidth 4


### Author
- Jyrith C