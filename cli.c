#include <cli.h>

static char command_buf[CMD_BUFSIZE] ;

static int get_menu_num(void){
    int num ;
    char *cmd = wait_command();

    num = strtol(cmd, NULL, 0); // conv str to long int

    return num;
}

static void display_menu(void)
{
    PutSt("1. Clothing, Shoes & Accessories\n");
    PutSt("2. Business & Industrial\n");
    PutSt("3. Books\n");
    PutSt("4. Toys & Hobbies\n");
    PutSt("5. DVDs & Movies\n");
    PutSt("6. Collectibles\n");
    PutSt("7. Health & Beauty\n");
    PutSt("8. Computers & Tablets\n");
}

void root_menu(char *main_menu){
    int menu_num ;

    printf("\n\t%s\n\n", main_menu);

    display_menu();
    PutSt("\nSelect item: ");
    menu_num = get_menu_num();

    PutSt("\nYour choice is ");
    
    switch (menu_num){
        case 1:
            PutSt("Clothing \n");
            break ;
        case 2:
            PutSt("Business \n");
            break ;
            /*
        case 3:
            puts("Books \n");
            break ;
        case 4:
            puts("Toys \n");
            break ;
        case 5:
            puts("DVD \n");
            break ;
        case 6:
            puts("Collections \n");
            break ;
        case 7:
            puts("Health \n");
            break ;
        case 8:
            puts("Computers \n");
            break ;
            */
        default: 
            puts("Wrong Selection \n");
            break ;
    }
    /*
    if ( menu_num == 1){
        puts("Hee is case 1\n");
    }
    else if ( menu_num == 2){
        puts("Business \n");
    }
    else{
        puts("Wrong Selection \n");
    }
    */
}

void *wait_command(void){
    char *cp = command_buf ;
    char ch ; 
    //int len, i ; 

    *cp = EOL ;
    while(1){
        ch = GetCh();
        switch(ch){
            case ENTER:
                PutCh(ch);
                PutCh('\n') ;
                return command_buf ;

            default:
                if( strlen(command_buf) < (CMD_BUFSIZE-1)){
                    *cp++=ch ;
                    *cp = EOL ;
                    PutCh(ch) ;
                }
                break ;
        }//switch
    }//while
    return command_buf ;
}
/*
static void wait_user_input(void)
{
    char ch ;
    switch (ch=GetCh())
    {
        case '\r':
            puts("Enter");
            break;
 
        case '\b':
            puts("Backspace");
            break;
 
        case CTRL_C:
            puts("Ctrl-C");
            break;
 
        case CTRL_B:
            puts("Ctrl-B");
            break;
 
        case CTRL_F:
            puts("Ctrl-F");
            break;
 
        case ESC:
	    ch = GetCh();
            if (!(ch == ESC2 || ch == ESC3))
                break;
 
            switch (GetCh())
            {
		case 0x50:
		    puts("F1");
 		    break ;

		case 0x51:
		    puts("F2");
 		    break ;
		case 0x52:
		    puts("F3");
 		    break ;
		case 0x53:
		    puts("F4");
 		    break ;

                case 'A':
                    puts("Up");
                    break;
 
                case 'B':
                    puts("Down");
                    break;
 
                case 'C':
                    puts("Right");
                    break;
 
                case 'D':
                    puts("Left");
                    break;
 
		// F5 ~ F8 is in 0x1B 5B 31 + '6''7''8''9'
                case '1':
                    switch (GetCh())
                    {
                        case '6':
                            if (GetCh() == DELIMIT)
                                puts("F5");
                            break;
 
                        case '7':
                            if (GetCh() == DELIMIT)
                                puts("F6");
                            break;
 
                        case '8':
                            if (GetCh() == DELIMIT)
                                puts("F7");
                            break;
 
                        case '9':
                            if (GetCh() == DELIMIT)
                                puts("F8");
                            break;
 
                        default:
                            break;
                    }
                    break;
 
                case '2':
                    switch (GetCh())
                    {
                        case DELIMIT:
                            puts("Insert");
                            break;
 
                        case '0':
                            if (GetCh() == DELIMIT)
                                puts("F9");
                            break;
 
                        case '1':
                            if (GetCh() == DELIMIT)
                                puts("F10");
                            break;
 
                        case '3':
                            if (GetCh() == DELIMIT)
                                puts("F11");
                            break;
 
                        case '4':
                            if (GetCh() == DELIMIT)
                                puts("F12");
                            break;
 
                        default:
                            break;
                    }
                    break;
 
                case '3':
                    if (GetCh() == DELIMIT)
                        puts("Delete");
                    break;
 
                default:
                    break;
            }
 
            break;
 
        default:
            break;
    }
 
}
 
 
void cli_loop(void)
{
    while(1){
    	printf("\n[CLI]$ ");
    	//wait_user_input();
    	wait_command();
    }
}
*/

static char **argv_buf ;
void init_command(void){
    int i ; 

    argv_buf = (char**)malloc(sizeof(char*) * MAX_ARGS);
//    argv_buf = calloc(MAX_ARGS, ARG_BUFSIZE);
    for ( i =0 ; i < MAX_ARGS ; i++)
        argv_buf[i] = malloc(ARG_BUFSIZE) ;
}

void parse_command(const char *command, int *argc, char **argv){
    char tmp_buf[CMD_BUFSIZE] ;
    char *delimit = " \t" ;
    char *token ; 
    int arg_num = 0 ;

    strncpy(tmp_buf, command, CMD_BUFSIZE);
    token = strtok(tmp_buf, delimit); 

    if(token == NULL){
        *argc = 0 ;
        return ; 
    }

    do {
        strncpy(argv_buf[arg_num++], token, ARG_BUFSIZE);
        token = strtok(NULL, delimit) ;
    } while ( token && arg_num < MAX_ARGS );

    *argc = arg_num ;
    *argv = (void *)argv_buf ;
}

void print_args(int argc, char *argv[]){
    int i ; 
    for ( i = 0 ; i < argc ; i++){
        printf("Argv[%u] = %s\n", i, argv[i]) ;
    }
}
