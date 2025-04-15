/** EEPROM:M24C02 operation library for Arduino
 *
 *  @author Tedd OKANO
 *
 *  Released under the MIT license License
 */

#ifndef ARDUINO_M24C02_H
#define ARDUINO_M24C02_H

#include <Arduino.h>
#include <stdint.h>

#include <I2C_device.h>

/** M24C02 class
 *	
 *  @class M24C02
 *
 *	M24C02 class is a sample code for the PCA9846PW-ARD and the PCA9617ADP-ARD operation
 */

class M24C02 : public I2C_device
{
public:
	/** Create a M24C02 instance with specified address
	 *
	 * @param i2c_address I2C-bus address (default: (0xA0>>1))
	 */
	M24C02( uint8_t i2c_address = (0xA0 >> 1) );

	/** Create a M24C02 instance with specified address
	 *
	 * @param wire TwoWire instance
	 * @param i2c_address I2C-bus address (default: (0xA0>>1))
	 */
	M24C02( TwoWire& wire, uint8_t i2c_address = (0xA0 >> 1) );
	virtual ~M24C02();
	
	/** Begin the device operation
	 *
	 *	This method turns-on the device
	 */
	void begin( void );

	/** Wait write complete 
	 *
	 *	@param n loop count (in mili-second)
	 *	@return remain count: 0 means timeout
	 */
	int wait_write_complete( int n );

	/** Write data
	 *
	 * @param byte_adr byte address
	 * @param data 1 byte data for writing
	 * @return size of data written or -10 if failed (timeout)
	 */
	int write( int byte_adr, uint8_t data );	

	/** Write data
	 *
	 * @param byte_adr byte address. The address can be arbitrary. It not needed to be aligned to page size.
	 * @param dp pointer to data array
	 * @param length data size
	 * @return size of data written or -10 if failed (timeout)
	 */
	int write( int byte_adr, const uint8_t *dp, int length );	

	/** Read data
	 *
	 * @param byte_adr byte address
	 * @return 1 byte read data
	 */
	uint8_t read( int byte_adr );	

	/** Read data
	 *
	 * @param byte_adr byte address. The address can be arbitrary. It not needed to be aligned to page size.
	 * @param dp pointer to data array
	 * @param length data size
	 * @return size of data read
	 */
	uint8_t read( int byte_adr );	
	int read( int byte_adr, uint8_t *dp, int length );	
};

#endif //	ARDUINO_M24C02_H
