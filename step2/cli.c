#include <cli.h>

static char command_buf[CMD_BUFSIZE] ;

char *wait_command(void){
    char *cp = command_buf ;
    char ch ; 
    //int len, i ; 

    *cp = EOL ;
    while(1){
        ch = GetCh();
        //printf("Received Ch : %c\n",ch);
        switch(ch){
            case ENTER:
                //PutSt("Here is Enter\n");
                PutSt("\r\n") ;
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

static char **argv_buf ;
void init_command(void){
    int i ; 

    argv_buf = (char**)malloc(sizeof(char*) * MAX_ARGS);
//    argv_buf = calloc(MAX_ARGS, ARG_BUFSIZE);
    for ( i =0 ; i < MAX_ARGS ; i++)
        argv_buf[i] = malloc(ARG_BUFSIZE) ;
}

void parse_command(char *command, int *argc, char **argv){
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

extern cli_cmd_t __cli_start, __cli_end ;

void run_cli(void){

    cli_cmd_t *cli_cmd = &__cli_start ;
    int num_commands = &__cli_end - &__cli_start ;

    int argc ; 
    char **argv ;
    int i ; 

    PutSt("[CLI]$ ");

    parse_command(wait_command(), &argc, (void*)&argv);
    if ( !argc ){
        PutSt("argc equals 0 \r\n");
        return ;
    }

    for ( i = 0 ; i < num_commands; i++, cli_cmd++){
        if(strcmp(cli_cmd->cmd_name, argv[0])==0){
            cli_cmd->cli_func(argc, argv);
            break ;
        }
    }
    if ( num_commands <= i){
        printf("You entered wring command: \"%s\"\n", argv[0]);
    }
}


void root_menu(char *main_menu){
    printf("\n CLI Command Interpreter is running ... !!!\n");
    run_cli();
}
