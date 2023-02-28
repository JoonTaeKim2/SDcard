#define F_CPU 16000000UL  // Note the F_CPU shoule be defined earlier than util/delay.h
#include <avr/io.h>
#include <util/delay.h>
#include <cli.h>
#include <spi.h>
#include <microsd.h>

static char *month[] = 
    {"JAN", "FEB", "MAR", "APR", "MAY", "JUN",
     "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

static void read_cid(int argc, char **argv){
	struct sd_cmd cmd ;

	printf("\n\nCID register:\n");

	cmd.cmd_idx = SD_CMD_SEND_CID ;
	cmd.arg = 0 ;
	cmd.resp_type = SD_RSP_R2 ;

	if ( sd_cmd_request(&cmd, NULL)){
		printf("Read CID Error \n");
		return ;
	}

	printf("Raw Data ==> %08X %08X %08X %08X\n\n", 
							cmd.response[0],
							cmd.response[1],
							cmd.response[2],
							cmd.response[3]);

	printf("Manufacturer ID    : %02X\n", (cmd.response[0] >> 24) & 0xFF);
    printf("OEM/Application ID : %c%c\n",
                (cmd.response[0] >> 16) & 0xFF,
                (cmd.response[0] >> 8) & 0xFF);
    printf("Product Name       : %c%c%c%c%c\n",
                (cmd.response[0] >> 0) & 0xFF,
                (cmd.response[1] >> 24) & 0xFF,
                (cmd.response[1] >> 16) & 0xFF,
                (cmd.response[1] >> 8) & 0xFF,
                (cmd.response[1] >> 0) & 0xFF);
    printf("Product Revision   : %d.%d\n",
                (cmd.response[2] >> 28) & 0xF,
                (cmd.response[2] >> 24) & 0xF);
    printf("Product Serial No. : %06X%02X\n",
                (cmd.response[2] >> 0) & 0xFFFFFF,
                (cmd.response[3] >> 24) & 0xFF);
    printf("Manufacturing Date : %s %d\n",
                month[((cmd.response[3] >> 8) & 0xF) - 1],
                ((cmd.response[3] >> 12) & 0xFF) + 2000);

}

CLI_CMD(cid, "Read CSD register value", read_cid) ;
