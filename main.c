#include <cli.h>

//extern void PutSt(char *ch) ;

void setup(void)
{
    Init_UART();
    init_command();

}

extern void root_menu(char *);
void loop(void){

    root_menu("CLI TEST Online Market");
    puts("end of root_menu\n");
/*
    char *str ;
    int argc; 
    char *argv ;

    printf("\n[CLI]$ ");
    //printf("=> %s \r\n",wait_command());
    str = wait_command() ;
    parse_command(str, &argc, &argv) ;
    print_args(argc,(char **)argv) ;
*/

}

int main(void)
{

    setup();
    PutSt("Hello ");
    PutSt("world \n");

    printf("\nMenu Driven User Interface. %s %s \n",__DATE__, __TIME__);

    while(1)
        loop();

 
/*
    while(1){
        ch = GetCh();
    	if (ch != '\r')
    	{
            uart_buf[index++] = ch;
    	}
        else
     	{
            printf("==> ");
            for (i=0; i<index; i++)
            {
             	printf("%02X ", uart_buf[i]);
            }
            printf("\n");
 
            index = 0;
    	}
    }
*/

    return 0 ;
}
    
