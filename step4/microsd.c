#define F_CPU 16000000UL  // Note the F_CPU shoule be defined earlier than util/delay.h
#include <avr/io.h>
#include <util/delay.h>
#include <cli.h>
#include <spi.h>
#include <microsd.h>

#define SD_CMD(x) (0x40 | (x & 0x3f))

#define SPI_TOKEN_SINGLE        0xfe    /* single block r/w, multiblock read */
#define SPI_TOKEN_MULTI_WRITE   0xfc    /* multiblock write */
#define SPI_TOKEN_STOP_TRAN     0xfd    /* terminate multiblock write */


/* timeout value */
#define CTOUT   8
#define RTOUT   1000 /* 1 sec */
#define WTOUT   1000 /* 1 sec */


extern uint8_t crc7(uint8_t crc, const uint8_t *buf, size_t len);
extern uint8_t crc16(uint16_t crc, const uint8_t *buf, size_t len);

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

    //enable_cs() ;

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

    for ( i = 0 ; i < CTOUT ; i++){
        spi_xfer(NULL, &r1, 1) ;
        if ( !(r1 & 0x80))
            break ; 
    }

    //disable_cs();

    return r1 ;
}

static uint8_t read_data(void *xbuf, uint32_t bcnt, uint32_t bsize){
	uint8_t		*buf = xbuf ;
	uint8_t		r1 = 0 ; 
	uint16_t	crc ;
	int			i ;

	while(bcnt--){
		for ( i = 0 ; i < RTOUT ; i++){
			spi_xfer(NULL, &r1, 1) ;
			if ( r1 != 0xFF)
				break ;
			_delay_ms(1) ;
		}

		if ( r1 == SPI_TOKEN_SINGLE){
			spi_xfer(NULL, buf, bsize) ;
			spi_xfer(NULL, (void*)&crc, 2) ;
			if( crc != swap16(crc16(0, buf, bsize))){
				printf("CRC16 mismatched f\n");
				r1 = R1_SPI_COM_CRC ;
				break ;
			}
			r1 = 0 ;
		}
		else{
			r1 = R1_SPI_ERROR ;
			break; 
		}
		buf += bsize ;
	}
	return r1 ;

}

static int sd_set_idle(void){
	struct sd_cmd cmd ;
	int err ;

	_delay_ms(1);

	cmd.cmd_idx = SD_CMD_GO_IDLE_STATE ;
	cmd.arg = 0 ;
	cmd.resp_type = SD_RSP_NONE ;

	err = sd_cmd_request(&cmd, NULL);

	if (err)
		return err ;
	_delay_ms(2);
	return 0 ;
}

static int send_if_cond(void){
	struct sd_cmd cmd ;
	int err ;

	cmd.cmd_idx = SD_CMD_SEND_IF_COND ;
	cmd.arg = 0x1AA ;
	cmd.resp_type = SD_RSP_R7 ;

	err = sd_cmd_request(&cmd, NULL);

	if (err)
		return err ;

	printf("SD Memory Card Version: ");
	if ( (cmd.response[0] & 0x1FF) != 0x1AA){
		printf("1.0\n");
		return UNUSABLE_ERR ;
	}
	printf("2.0\n");
	return 0 ;
}

static int send_op_cond(void){
	struct sd_cmd cmd ;
	int err ;
	int 	timeout=1000 ;

	while(1){
		cmd.cmd_idx = SD_CMD_APP_CMD;
		cmd.arg = 0;
		cmd.resp_type = SD_RSP_R1 ;

		err = sd_cmd_request(&cmd, NULL);
		if (err)
			return err ;

		cmd.cmd_idx = SD_CMD_APP_SEND_OP_COND;
		cmd.arg = OCR_HCS;
		cmd.resp_type = SD_RSP_R3 ;

		err = sd_cmd_request(&cmd, NULL);
		if (err)
			return err ;

		if ( cmd.response[0] & OCR_BUSY)
			break ;

		if ( timeout-- <= 0)
			return UNUSABLE_ERR ;

		_delay_ms(1);
	}
	return 0 ;
}

static int sd_read_ocr(void){
	struct sd_cmd cmd ;
	int err ;

	cmd.cmd_idx = SD_CMD_SPI_READ_OCR;
	cmd.arg = 0;
	cmd.resp_type = SD_RSP_R3 ;

	err = sd_cmd_request(&cmd, NULL);
	if (err)
		return err ;

	printf("SD Memory Card Capacity : ");
	if(cmd.response[0] & OCR_HCS)
		printf("SDHC or SDXC\n");
	else
		printf("Standard SD\n");

	return 0 ;
}

static int sdcard_init_flow(void){
	int err = 0 ;

	if ( sd_set_idle()){
		printf("CMD0 Fail \n");
		err++ ;
	}

	if ( !err && send_if_cond())
		err++ ;

	if ( !err && send_op_cond()){
		printf("ACMD41 Fail\n");
		err++ ;
	}
	
	if ( !err && sd_read_ocr())
		err++ ;

	return err ;
}

void microsd_init(){
    disable_cs();
    _delay_ms(100);

    set_spi_mode();

    //send_sd_cmd(0,0);
    //send_sd_cmd(8, 0x1AA) ;
	//sd_set_idle();
	//send_if_cond();
	//send_op_cond();
	//sd_read_ocr();
	sdcard_init_flow();
    printf("MicroSD initialized ...!!\n");
}

int sd_cmd_request(struct sd_cmd *cmd, struct sd_data *data){
	uint8_t r1 ;
	int i ; 
	int ret = 0 ;

	enable_cs();

	r1 = send_sd_cmd(cmd->cmd_idx, cmd->arg);
	printf("r1 data : %x\n", r1);

	if (r1 == 0xFF){
		printf("Here is NO_CARD_ERR\n");
		ret = NO_CARD_ERR ;
		goto done ;
	}

	else if ( r1 & R1_SPI_COM_CRC){
		printf("Here is COMM_ERR\n");
		ret = COMM_ERR ;
		goto done ;
	}
	else if ( r1 & ~R1_SPI_IDLE ){
		printf("Here is TIMEOUT\n");
		ret = TIMEOUT ;
	 	goto done ;
	}
	else if ( cmd->resp_type == SD_RSP_R2){
		printf("Here is Read_data\n");
		r1 = read_data(cmd->response, 1, 16) ;
		for ( i =0 ; i < 4 ; i ++)
			cmd->response[i] = swap32(cmd->response[i]) ;
	}

	else if ( !data){
		switch (cmd-> cmd_idx){
			case SD_CMD_APP_SEND_OP_COND:
			case SD_CMD_SEND_OP_COND:
				cmd->response[0] = (r1 & R1_SPI_IDLE) ? 0 : OCR_BUSY ;
				break ;

			case SD_CMD_SPI_READ_OCR:
			case SD_CMD_SEND_IF_COND:
				spi_xfer(NULL, (void *)cmd->response, 4) ;
				cmd->response[0] = swap32(cmd->response[0]) ;
				break ;

			case SD_CMD_SEND_STATUS:
				spi_xfer(NULL, (void *)cmd->response, 1) ;
				cmd->response[0] = (cmd->response[0] & 0xFF) ? SD_STATUS_ERROR : SD_STATUS_RDY_FOR_DATA ;
				break ;

			default:
				break ;
		}
	}
	else {
        if (data->flags == SD_DATA_READ)
        {
            // TODO:
		}
        else if (data->flags == SD_DATA_WRITE)
        {
            // TODO:
        }

        if (r1 & R1_SPI_COM_CRC)
        {
            ret = COMM_ERR;
        }
        else if (r1)
        {
            ret = TIMEOUT;
        }
	}

done:
	disable_cs();

	return ret ;
}
