#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <command.h>

#define CTRL_B      2
#define CTRL_C      3
#define CTRL_F      6
 
#define EOL         0x00 
#define ENTER       '\r'
#define ESC         0x1B
#define ESC2        0x4F
#define ESC3        0x5B

#define DELIMIT	    0x7E

#define MAX_ARGS 10
#define CMD_BUFSIZE 100
#define ARG_BUFSIZE 10

extern void init_UART() ;
extern char GetCh(void) ;
extern void PutCh(char) ;
extern void PutSt(char *);

void cli_loop(void) ;
void init_command(void);
//void parse_command(const char *command, int *argc, char **argv);
//void print_args(int argc, char *argv[]) ;

//char *wait_command(void);
#define array_num(x) (sizeof(x) / sizeof(x[0]))

void root_menu(char *);
