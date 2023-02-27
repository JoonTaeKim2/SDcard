#include <avr/io.h>
#include <cli.h>
#include <spi.h>

/*
#define SPI_DDR     DDRB
#define SPI_PORT    PORTB
#define SS          PINB0
#define SCK         PINB1
#define MOSI        PINB2
#define MISO        PINB3
*/

void init_spi(void)
{
    SPI_DDR &= ~(1<<MISO);                     // MISO => input
    SPI_DDR |= (1<<SS) | (1<<MOSI) | (1<<SCK); // MOSI, SCK, SS=> output
    
    // SPI Enable, MSB first, Master Mode, IDLE SCK=Low, SCK speed = 16M / 16
    SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) ;
    SPCR |= (1<<SPR0) ;
    printf("SPI initialized\n");
}

/*
static void spi_write(uint8_t data){
    SPDR = data ;
    while (!(SPSR & (1<<SPIF))) ; 
}

static uint8_t spi_read(void){
    SPDR = 0x00 ;

    while(!(SPSR & (1<<SPIF))) ;

    return SPDR ;
}*/

static uint8_t spi_write_read(uint8_t data){

    SPDR = data ;
    while (!(SPSR & (1<<SPIF))) ; 
    return SPDR ;
    
}

void spi_xfer(const uint8_t *outbuf, uint8_t *inbuf, uint16_t len)
{
    uint8_t val ;

    while(len--){
        val = spi_write_read(outbuf ? *outbuf++ : 0xFF);

        if ( inbuf)
            *inbuf++ = val ;
    }
}

