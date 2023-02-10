#include <cli.h>

#define array_num(x) (sizeof(x) / sizeof(x[0]))

struct menu_table{
    char *menu_item ;
    void (*menu_func)(void);
};


struct cli_table {
    char *cmd_name; 
    char *cmd_help;
    void (*cli_func)(int, char**);
};

static void do_cloth(int argc, char **argv){
    PutSt("Here is do_cloth function \r\n");
}
static void do_business(int argc, char **argv){
    PutSt("Here is do_business function \r\n");
}
static void do_books(int argc, char **argv);

static void do_toys(int argc, char **argv){
    PutSt("Here is do_toys function \r\n");
}
static void do_dvds(int argc, char **argv){
    PutSt("Here is do_dvds function \r\n");
}
static void do_collectibles(int argc, char **argv){
    PutSt("Here is do_collectibles function \r\n");
}
static void do_health(int argc, char **argv){
    PutSt("Here is do_health function \r\n");
}
static void do_computers(int argc, char **argv){
    PutSt("Here is do_computers function \r\n");
}
static void do_help(int argc, char **argv);
static void do_books(int argc, char **argv);

struct cli_table cli_command[] = {
    {"Clothing", "Clothing, Shoes & Accesories", do_cloth}, 
    {"Business", "Business & Industrial", do_business}, 
    {"Books", "Books", do_books}, 
    {"Toys", "Toys & Hobbies", do_toys}, 
    {"media", "DVDs & Movies", do_dvds}, 
    {"collect", "Collectibles", do_collectibles}, 
    {"Health", "Health & Beauty", do_health}, 
    {"Computers", "Computers & Tablets", do_computers},
    {"help", "Display CLI command usage", do_help}
} ;


static int num_commands = array_num(cli_command);

static int cmd_sorted = 0 ;
static int cli_order[array_num(cli_command)];

static void do_help(int argc, char **argv){
    int i ; 
    for ( i = 0; i < num_commands; i++){
        printf("%-10s : %s\n", cli_command[cli_order[i]].cmd_name, 
                               cli_command[cli_order[i]].cmd_help);
    }
}

static void sort_table(void)
{
    int     i, j, tmp;

    for (i=0; i<num_commands; i++)
        cli_order[i] = i;

    for (i=num_commands-1; i; i--)
        for (j=0; j<i; j++)
        {
            if (strcmp(cli_command[cli_order[j]].cmd_name,
                       cli_command[cli_order[j+1]].cmd_name) > 0)
            {
                tmp = cli_order[j];
                cli_order[j] = cli_order[j+1];
                cli_order[j+1] = tmp;
            }
        }
}

static void do_art(int argc, char **argv){
    PutSt("Here is do_art function \r\n");
}
static void do_comics(int argc, char **argv){
    PutSt("Here is do_comics function \r\n");
}
static void do_history(int argc, char **argv){
    PutSt("Here is do_history function \r\n");
}
static void do_sf(int argc, char **argv){
    PutSt("Here is do_sf function \r\n");
}
struct cli_table book_cmd[] = {
    {"art", "Arts & Photography", do_art},
    {"comic", "Comics & Graphic Novels", do_comics},
    {"history", "History", do_history},
    {"sf", "Science Fiction", do_sf},
};
static int book_items = array_num(book_cmd);


static void book_help(){
    int i ; 

    printf("You can select one of below item \n");
    for ( i = 0; i < book_items; i++){
        printf("%-10s : %s\n", book_cmd[i].cmd_name, book_cmd[i].cmd_help);
    }
}

static void do_books(int argc, char **argv){
    int i ; 

    if ( argc == 1){
        book_help();
        return ;
    }

    for ( i = 0 ; i < book_items; i++){
        if(strcmp(book_cmd[i].cmd_name, argv[1])==0){
            book_cmd[i].cli_func(argc, argv);
            break ;
        }
    }
}

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

void run_cli(void){
    int argc ; 
    char **argv ;
    int i ; 

    if(!cmd_sorted){
        sort_table();
        cmd_sorted =1 ;
    }

    PutSt("[CLI]$ ");

    parse_command(wait_command(), &argc, (void*)&argv);
    if ( !argc ){
        PutSt("argc equals 0 \r\n");
        return ;
    }

    for ( i = 0 ; i < num_commands; i++){
        if(strcmp(cli_command[i].cmd_name, argv[0])==0){
            cli_command[i].cli_func(argc, argv);
            break ;
        }
    }
    if ( num_commands <= i){
        printf("You entered wring command: \"%s\"\n", argv[0]);
    }
}


void root_menu(char *main_menu){
    run_cli();
}

void print_args(int argc, char *argv[]){
    int i ; 
    for ( i = 0 ; i < argc ; i++){
        printf("Argv[%u] = %s\n", i, argv[i]) ;
    }
}
