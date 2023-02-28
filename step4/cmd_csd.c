#define F_CPU 16000000UL  // Note the F_CPU shoule be defined earlier than util/delay.h
#include <avr/io.h>
#include <util/delay.h>
#include <cli.h>
#include <spi.h>
#include <microsd.h>

static void csd_ver1(struct sd_cmd   *cmd)
{
    uint32_t rd_blk_len, block_len;
    uint32_t c_size_mul, mult;
    uint32_t block_nr, c_size;

    printf("CSD Version 1.0\n");

    printf("TAAC            : %02X\n", (cmd->response[0] >> 16) & 0xFF);
    printf("NSAC            : %02X\n", (cmd->response[0] >> 8) & 0xFF);
    printf("TRAN_SPEED      : %02X\n", (cmd->response[0] >> 0) & 0xFF);
    printf("CCC             : %03X\n", (cmd->response[1] >> 20) & 0xFFF);
    rd_blk_len = (cmd->response[1] >> 16) & 0xF;
    block_len = 1 << rd_blk_len;
    printf("RD_BL_LEN       : %X(%u)\n", rd_blk_len, block_len);
    printf("RD_BL_PART      : %X\n", (cmd->response[1] >> 15) & 0x1);
    printf("WR_BLK_MISALIGN : %X\n", (cmd->response[1] >> 14) & 0x1);
    printf("RD_BLK_MISALIGN : %X\n", (cmd->response[1] >> 13) & 0x1);
    printf("DSR_IMP         : %X\n", (cmd->response[1] >> 12) & 0x1);
    c_size = ((cmd->response[1] & 0x3FF) << 2) | ((cmd->response[2] >> 30) & 0x3);
    printf("CSIZE           : %03X\n", c_size);
    printf("VDD_R_CUR_MIN   : %X\n", (cmd->response[2] >> 27) & 0x7);
    printf("VDD_R_CUR_MAX   : %X\n", (cmd->response[2] >> 24) & 0x7);
    printf("VDD_W_CUR_MIN   : %X\n", (cmd->response[2] >> 21) & 0x7);
    printf("VDD_W_CUR_MAX   : %X\n", (cmd->response[2] >> 18) & 0x7);
    c_size_mul = (cmd->response[2] >> 15) & 0x7;
    mult = 1 << (c_size_mul + 2);
    printf("C_SIZE_MULT     : %X(%u)\n", c_size_mul, mult);
    printf("ERASE_BLK_EN    : %X\n", (cmd->response[2] >> 14) & 0x1);
    printf("SECTOR_SIZE     : %02X\n", (cmd->response[2] >> 7) & 0x7F);
    printf("WP_GRP_SIZE     : %02X\n", (cmd->response[2] >> 0) & 0x7F);
    printf("WP_GRP_EN       : %X\n", (cmd->response[3] >> 31) & 0x1);
    printf("R2W_FACTOR      : %X\n", (cmd->response[3] >> 26) & 0x7);
    printf("WR_BL_LEN       : %X\n", (cmd->response[3] >> 22) & 0xF);
    printf("WR_BL_PART      : %X\n", (cmd->response[3] >> 21) & 0x1);
    printf("FILE_FMT_GRP    : %X\n", (cmd->response[3] >> 15) & 0x1);
    printf("COPY            : %X\n", (cmd->response[3] >> 14) & 0x1);
    printf("PERM_WR_PROT    : %X\n", (cmd->response[3] >> 13) & 0x1);
    printf("TMP_WR_PROT     : %X\n", (cmd->response[3] >> 12) & 0x1);
    printf("FILE_FORMAT     : %X\n", (cmd->response[3] >> 10) & 0x3);

    block_nr = (c_size + 1) * mult * block_len;
    printf("\nMemory Capacity : %u\n\n", block_nr);
}

static void csd_ver2(struct sd_cmd   *cmd)
{
    uint32_t c_size;
    uint64_t mem_cap;

    printf("CSD Version 2.0\n");

    printf("TAAC            : %02X\n", (cmd->response[0] >> 16) & 0xFF);
    printf("NSAC            : %02X\n", (cmd->response[0] >> 8) & 0xFF);
    printf("TRAN_SPEED      : %02X\n", (cmd->response[0] >> 0) & 0xFF);
    printf("CCC             : %03X\n", (cmd->response[1] >> 20) & 0xFFF);
    printf("RD_BL_LEN       : %X\n", (cmd->response[1] >> 16) & 0xF);
    printf("RD_BL_PART      : %X\n", (cmd->response[1] >> 15) & 0x1);
    printf("WR_BLK_MISALIGN : %X\n", (cmd->response[1] >> 14) & 0x1);
    printf("RD_BLK_MISALIGN : %X\n", (cmd->response[1] >> 13) & 0x1);
    printf("DSR_IMP         : %X\n", (cmd->response[1] >> 12) & 0x1);
    c_size = ((cmd->response[1] & 0x3F) << 16) | ((cmd->response[2] >> 16) & 0xFFFF);
    printf("CSIZE           : %06X\n", c_size);
    printf("ERASE_BLK_EN    : %X\n", (cmd->response[2] >> 14) & 0x1);
    printf("SECTOR_SIZE     : %02X\n", (cmd->response[2] >> 7) & 0x7F);
    printf("WP_GRP_SIZE     : %02X\n", (cmd->response[2] >> 0) & 0x7F);
    printf("WP_GRP_EN       : %X\n", (cmd->response[3] >> 31) & 0x1);
    printf("R2W_FACTOR      : %X\n", (cmd->response[3] >> 26) & 0x7);
    printf("WR_BL_LEN       : %X\n", (cmd->response[3] >> 22) & 0xF);
    printf("WR_BL_PART      : %X\n", (cmd->response[3] >> 21) & 0x1);
    printf("FILE_FMT_GRP    : %X\n", (cmd->response[3] >> 15) & 0x1);
    printf("COPY            : %X\n", (cmd->response[3] >> 14) & 0x1);
    printf("PERM_WR_PROT    : %X\n", (cmd->response[3] >> 13) & 0x1);
    printf("TMP_WR_PROT     : %X\n", (cmd->response[3] >> 12) & 0x1);
    printf("FILE_FORMAT     : %X\n", (cmd->response[3] >> 10) & 0x3);

    mem_cap = (c_size + 1) * 0x80000ull;
    printf("\nMemory Capacity : %llu\n\n", mem_cap);
}

static void read_csd(int argc, char **argv)
{
	struct sd_cmd cmd ;
	uint32_t csd_structure ;

	printf("\n\nCID register:\n");

	cmd.cmd_idx = SD_CMD_SEND_CSD ;
	cmd.arg = 0 ;
	cmd.resp_type = SD_RSP_R2 ;

	if ( sd_cmd_request(&cmd, NULL)){
		printf("Read CSD error\n");
		return ;
	}

	printf("Raw Data ==> %08X %08X %08X %08X\n\n", 
						cmd.response[0], 
						cmd.response[1], 
						cmd.response[2], 
						cmd.response[3] );

	csd_structure = (cmd.response[0] >> 30) & 0x03 ;
	if ( csd_structure == 0)
		csd_ver1(&cmd);
	else if ( csd_structure == 1)
		csd_ver2(&cmd) ;
	else
		printf("CSD version Error \n");
}

CLI_CMD(csd, "Read CSD register value", read_csd) ;
