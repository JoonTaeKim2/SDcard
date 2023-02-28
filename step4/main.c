#include <cli.h>
#include <spi.h>
#include <microsd.h>

#define CLR_SCREEN "\x1B[2J\x1B[1;1H"
void setup(void)
{
    init_UART();
    init_command();
    init_spi();

    printf(CLR_SCREEN);
    printf("\nMcrio SD Memory. %s %s \n",__DATE__, __TIME__);

    microsd_init();
}

void loop(void){

    root_menu("CLI TEST Online Market");
}

int main(void)
{

    setup();


    while(1)
        loop();

    return 0 ;
}
    
