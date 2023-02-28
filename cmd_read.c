#define F_CPU 16000000UL  // Note the F_CPU shoule be defined earlier than util/delay.h
#include <avr/io.h>
#include <util/delay.h>
#include <cli.h>
#include <spi.h>
#include <microsd.h>

#define SD_MEM_BUF_SZ	512

static void dump_mem(void *buf, uint32_t len){
	int i, j ;
	char ch ;
	uint8_t *mem_buf = buf ;

	for(i = 0 ; i < len ; i+=16){
		printf("\n%04X: ", i) ;

		for ( j = 0; j < 16 ; j++){
			printf("%02X", mem_buf[i+j]);
			if (( j & 3) == 3)
				PutCh(' ');
		}
		PutSt("| ");
		for ( j = 0 ; j < 16 ; j++){
			ch = mem_buf[i+j] ;
			if ( ( ch < 0x20 ) || (ch >= 0x7F))
				ch = '.' ;
			printf("%c", ch);
		}
	}
	puts("\n\n");
}

static void read_mem_single(uint32_t num_blk){
	struct sd_cmd cmd ;
	struct sd_data data ;
	uint8_t buf[SD_MEM_BUF_SZ] ;

	printf("Read Single Block [0x%X]\n", num_blk);

	cmd.cmd_idx = SD_CMD_READ_SINGLE_BLOCK ;
	/*
	if( high_capacity)
		cmd.arg = num_blk ;
	else
		cmd.arg = num_blk * SD_MEM_BUF_SZ ;
	*/
	cmd.arg = num_blk ;
	cmd.resp_type = SD_RSP_R1 ;

	data.dst = (void *)buf ;
	data.blocks = 1 ;
	data.block_size = SD_MEM_BUF_SZ ;
	data.flags = SD_DATA_READ ;

	if ( sd_cmd_request(&cmd, &data)){
		printf("Read block error \n");
		return ;
	}
	dump_mem(buf, SD_MEM_BUF_SZ);
}

static void read_mem(int argc, char **argv){
	uint32_t num_blk = 0 ;
	uint32_t blk_count = 1 ;

	if ( argc > 1)
		num_blk = strtoul(argv[1], NULL, 0);
	if (argc > 2)
		blk_count = strtoul(argv[2], NULL, 0);

	if ( blk_count == 0)
		blk_count = 1 ;

	if ( blk_count > 1){
		if ( blk_count > 4)
			blk_count = 4 ;
		// read_memory_multi function 
	}
	else
		read_mem_single(num_blk) ;
}

CLI_CMD(read, "Read SD Memory Data", read_mem) ;
