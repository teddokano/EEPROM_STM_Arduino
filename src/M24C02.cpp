#include <M24C02.h>

M24C02::M24C02( uint8_t i2c_address ) : I2C_device( i2c_address )
{
}

M24C02::M24C02( TwoWire& wire, uint8_t i2c_address ) : I2C_device( wire, i2c_address )
{
}

M24C02::~M24C02()
{
}

void M24C02::begin( void )
{
}

int M24C02::write( int byte_adr, uint8_t data )
{
	if ( !wait_write_complete( 10 ) )
		return -10;
	
	int r = reg_w( byte_adr, data );
	
	if ( !wait_write_complete( 10 ) )
		return -10;
	
	return r;
}

int M24C02::write( int byte_adr, const uint8_t *dp, int length )
{
#define	PAGE_WRITE_SIZE	16
	int	w_size;
	int	written	= 0;
	
	while ( length ) {
		w_size	= ( PAGE_WRITE_SIZE < length ) ? PAGE_WRITE_SIZE : length;

		if ( !wait_write_complete( 10 ) )
			return -10;
		
		w_size	= reg_w( byte_adr, dp, w_size ) - 1;

		if ( w_size < 0 )
			return w_size;
		
		length		-= w_size;
		written		+= w_size;
		byte_adr	+= w_size;
		dp			+= w_size;
	}

	if ( !wait_write_complete( 10 ) )
		return -10;
	
	return written;
}

int M24C02::wait_write_complete( int n )
{
	while ( !ping() && n-- )
		delay( 1 );
	
	if ( !n )
		Serial.println("time out in M24C02::wait_write_complete()");

	return n;
}

uint8_t M24C02::read( int byte_adr )
{
	return reg_r( byte_adr );
}

int M24C02::read( int byte_adr, uint8_t *dp, int length )
{
#define	PAGE_READ_SIZE	32
	int	r_size;
	int	read_done	= 0;

	while ( length ) {
		r_size	= ( PAGE_READ_SIZE < length ) ? PAGE_READ_SIZE : length;

		r_size	= reg_r( byte_adr, dp, r_size );

		length		-= r_size;
		read_done	+= r_size;
		byte_adr	+= r_size;
		dp			+= r_size;
	}
	
	return read_done;
}

