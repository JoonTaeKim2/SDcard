#include <cli.h>

void setup(void)
{
    Init_UART();
    init_command();

}

extern void root_menu(char *);
void loop(void){

    //printf("\x1B[2J\x1B[H");
    root_menu("CLI TEST Online Market");
}

int main(void)
{

    setup();
    PutSt("Hello ");
    PutSt("world \n");

    printf("\nMenu Driven User Interface. %s %s \n",__DATE__, __TIME__);

    while(1)
        loop();

    return 0 ;
}
    
