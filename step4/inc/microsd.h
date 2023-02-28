#define SD_DATA_READ 	1
#define SD_DATA_WRITE 	2

enum sd_err {
	NO_ERR,
	NO_CARD_ERR,
	UNUSABLE_ERR,
	COMM_ERR,
	TIMEOUT,
	SWITCH_ERR,
};

enum sd_resp_type {
	SD_RSP_NONE,
	SD_RSP_R1,
	SD_RSP_R1b,
	SD_RSP_R2,
	SD_RSP_R3,
	SD_RSP_R4,
	SD_RSP_R5,
	SD_RSP_R6,
	SD_RSP_R7,
};

/*SD in SPI mode reports R1 Status always */
#define R1_SPI_IDLE				(1 << 0)
#define R1_SPI_ERASE_RESET		(1 << 1)
#define R1_SPI_ILLEGAL_COMMAND	(1 << 2)
#define R1_SPI_COM_CRC			(1 << 3)
#define R1_SPI_ERASE_SEQ		(1 << 4)
#define R1_SPI_ADDRESS			(1 << 5)
#define R1_SPI_PARAMETER		(1 << 6)
#define R1_SPI_ERROR			(1 << 7)

#define SD_CMD_GO_IDLE_STATE        0
#define SD_CMD_SEND_OP_COND         1
#define SD_CMD_ALL_SEND_CID         2
#define SD_CMD_SET_RELATIVE_ADDR    3
#define SD_CMD_SET_DSR              4
#define SD_CMD_SWITCH               6
#define SD_CMD_SELECT_CARD          7
#define SD_CMD_SEND_IF_COND         8
#define SD_CMD_SEND_CSD             9
#define SD_CMD_SEND_CID             10
#define SD_CMD_STOP_TRANSMISSION    12
#define SD_CMD_SEND_STATUS          13
#define SD_CMD_SET_BLOCKLEN         16
#define SD_CMD_READ_SINGLE_BLOCK    17
#define SD_CMD_READ_MULTIPLE_BLOCK  18
#define SD_CMD_SET_BLOCK_COUNT      23
#define SD_CMD_WRITE_SINGLE_BLOCK   24
#define SD_CMD_WRITE_MULTIPLE_BLOCK 25
#define SD_CMD_ERASE_GROUP_START    35
#define SD_CMD_ERASE_GROUP_END      36
#define SD_CMD_ERASE                38
#define SD_CMD_APP_CMD              55
#define SD_CMD_SPI_READ_OCR         58
#define SD_CMD_SPI_CRC_ON_OFF       59
#define SD_CMD_RES_MAN              62

#define SD_CMD_APP_SET_BUS_WIDTH    6
#define SD_CMD_ERASE_WR_BLK_START   32
#define SD_CMD_ERASE_WR_BLK_END     33
#define SD_CMD_APP_SEND_OP_COND     41
#define SD_CMD_APP_SEND_SCR         51

#define OCR_BUSY                    0x80000000
#define OCR_HCS                     0x40000000
#define OCR_VOLTAGE_MASK            0x007FFF80
#define OCR_ACCESS_MODE             0x60000000

#define SD_STATUS_MASK              (~0x0206BF7F)
#define SD_STATUS_SWITCH_ERROR      (1 << 7)
#define SD_STATUS_RDY_FOR_DATA      (1 << 8)
#define SD_STATUS_CURR_STATE        (0xf << 9)
#define SD_STATUS_ERROR             (1 << 19)

struct sd_cmd{
	uint8_t		cmd_idx ;
	uint32_t	resp_type ;
	uint32_t	arg;
	uint32_t	response[4] ;
};


struct sd_data{
	union {
		char 		*dst ;
		const char *src ;
	};
	uint32_t 	flags ;
	uint32_t	blocks; 
	uint32_t	block_size ;
};
void microsd_init(void) ;
int sd_cmd_request(struct sd_cmd *cmd, struct sd_data *data);
