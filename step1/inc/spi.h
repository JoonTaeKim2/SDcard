
#define SPI_DDR     DDRB
#define SPI_PORT    PORTB
#define SS          PINB0
#define SCK         PINB1
#define MOSI        PINB2
#define MISO        PINB3

void init_spi(void) ;
void spi_xfer(const uint8_t *outbuf, uint8_t *inbuf, uint16_t len) ;
