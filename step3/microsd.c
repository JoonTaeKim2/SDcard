#define F_CPU 16000000UL  // Note the F_CPU shoule be defined earlier than util/delay.h
#include <avr/io.h>
#include <util/delay.h>
#include <cli.h>
#include <spi.h>

#define SD_CMD(x) (0x40 | (x & 0x3f))

extern uint8_t crc7(uint8_t crc, const uint8_t *buf, size_t len);

static inline void enable_cs(void){
    SPI_PORT &= ~(1 << SS) ;
}

static inline void disable_cs(void){
    SPI_PORT |= (1 << SS) ;
}

static inline uint16_t swap16(uint16_t val){
    return (((val & 0xFF00) >> 8) | ((val & 0x00FF) << 8)) ;
}

static inline uint32_t swap32(uint32_t val)
{
    return (((val & 0xFF000000) >> 24) |
            ((val & 0x00FF0000) >> 8)  |
            ((val & 0x0000FF00) << 8)  |
            ((val & 0x000000FF) << 24));
}

static void set_spi_mode(void){
    spi_xfer(NULL, NULL, 10) ;
}

static uint8_t send_sd_cmd(uint8_t cmd, uint32_t arg){
    uint8_t spi_buf[8];
    uint8_t r1 = 0;
    int i ;

    enable_cs() ;

    spi_buf[0] = 0xFF ;
    spi_buf[1] = SD_CMD(cmd) ;
    spi_buf[2] = (arg >> 24) & 0xFF ;
    spi_buf[3] = (arg >> 16) & 0xFF ;
    spi_buf[4] = (arg >> 8) & 0xFF ;
    spi_buf[5] = arg & 0xFF ;
    spi_buf[6] = ((crc7(0, &spi_buf[1], 5) << 1) | 0x01 ) & 0xFF ;
    spi_buf[7] = 0xFF ;

    spi_xfer(spi_buf, NULL, 8);

/*
    printf("CMD: ");
    for ( i = 0 ; i < 8 ; i++)
        printf("%02X ", spi_buf[i]);
    printf("\n");

    spi_xfer(NULL, spi_buf, 8);
    printf("Res: ");
    for ( i = 0 ; i < 8 ; i++)
        printf("%02X ", spi_buf[i]);
    printf("\n\n");
	*/

    for ( i = 0 ; i < 8 ; i++){
        spi_xfer(NULL, &r1, 1) ;
        if ( !(r1 & 0x80))
            break ; 
    }

    disable_cs();

    return r1 ;
}


static void sd_set_idle(void){
	uint8_t r1 ;

	r1 = send_sd_cmd(0,0) ;
	printf("Set Idle(CMD0): %s\n", r1==0x01 ? "PASS":"FAIL");

}

static void send_if_cond(void){
	uint8_t	r1 ;
	uint32_t data;

	r1 = send_sd_cmd(8, 0x1AA);
	printf("SEND_IF_COND: ");
	if ( r1 == 0x01){
		enable_cs();
		spi_xfer(NULL, (void *)&data, 4);
		disable_cs();
		printf("%08x\n",(unsigned int)swap32(data));
		return ;
	}
	printf("FAIL\n");
}

static void send_op_cond(void){
	uint8_t r1 ;
	int 	timeout=1000 ;

	printf("SEND_OP_COND: ");
	while(1){
		r1 = send_sd_cmd(55, 0);
		if ( r1 != 0x01){
			printf("FAIL \n");
			return ;
		}

		r1 = send_sd_cmd(41, 0x40000000);
		if ( r1 == 0x00)
		{
			printf("PASS \n");
			return ;
		}

		if ( timeout-- <= 0){
			printf("Time out \n");
			return ;
		}
		_delay_ms(1);
	}
}

static void sd_read_ocr(void){
	uint8_t r1 ;
	uint32_t data ;

	r1 = send_sd_cmd(58, 0);
	printf("READ OCR: ");
	if( !(r1 & 0xFE)){
		enable_cs();
		spi_xfer(NULL, (void *)&data, 4);
		disable_cs();
		printf("%08X\n", swap32(data));
		return ;
	}
	printf("FAIL \n");
}

void microsd_init(){
    disable_cs();
    _delay_ms(100);

    set_spi_mode();

    //send_sd_cmd(0,0);
    //send_sd_cmd(8, 0x1AA) ;
	sd_set_idle();
	send_if_cond();
	send_op_cond();
	sd_read_ocr();
    printf("MicroSD initialized ...!!\n");
}
