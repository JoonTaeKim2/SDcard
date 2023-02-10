#include <cli.h>

#define array_num(x) (sizeof(x) / sizeof(x[0]))

struct menu_table{
    char *menu_item ;
    void (*menu_func)(void);
};

static void menu_header(char *title);
static void menu_body(struct menu_table *body, int num_items);
static int menu_tail(void);


static void do_cloth(void);
static void do_business(void);
static void do_books(void);
static void do_toys(void);
static void do_dvds(void);
static void do_collectibles(void);
static void do_health(void);
static void do_computers(void);

struct menu_table top_menu[] = {
    {"Clothing", do_cloth}, 
    {"Business", do_business}, 
    {"Books", do_books}, 
    {"Toys", do_toys}, 
    {"DVDs", do_dvds}, 
    {"Collectibles", do_collectibles}, 
    {"Health", do_health}, 
    {"Computers", do_computers} 
} ;


static void do_art(void);
static void do_comics(void);
static void do_history(void);
static void do_sf(void);

struct menu_table book_menu[] ={
    {"Arts & Photography", do_art},
    {"comics & Graphic Novels", do_comics},
    {"History", do_history},
    {"Science Fiction", do_sf},
};


static void do_art(void)
{
    puts("Arts & Photography\n");
}
static void do_comics(void)
{
    puts("Comics & Graphic Novels\n");
}
static void do_history(void)
{
    puts("History\n");
}
static void do_sf(void)
{
    puts("Science Fiction\n");
}


static void do_cloth(void)
{
    puts("Clothing, Shoes & Accessories\n");
}

static void do_business(void)
{
    puts("Business & Industrial\n");
}

static void do_books(void)
{
    //puts("Books\n");
    int menu_num, num_items ;

    do{
        num_items = array_num(book_menu);
        menu_header("Books");
        menu_body(book_menu, num_items);
        menu_num = menu_tail();

        if ( menu_num > 0 && menu_num <= num_items)
            book_menu[menu_num-1].menu_func();
    } while (menu_num);
}

static void do_toys(void)
{
    puts("Toys & Hobbies\n");
}

static void do_dvds(void)
{
    puts("DVDs & Movies\n");
}

static void do_collectibles(void)
{
    puts("Collectibles\n");
}

static void do_health(void)
{
    puts("Health & Beauty\n");
}

static void do_computers(void)
{
    puts("Computers & Tablets\n");
}
static char command_buf[CMD_BUFSIZE] ;

static int title_len ;

static void menu_header(char *title){
    title_len = strlen(title)+ 8;
    int i ; 

    PutSt("\r\n\r\n");
    for ( i = 0 ; i < title_len; i++)
        PutCh('=');

    printf("\n    %s\n",title);

    for (i = 0; i < title_len; i++)
        PutCh('=');

    PutSt("\r\n\r\n");
}

static void menu_body(struct menu_table *body, int num_items){
    int i ; 
    
    for (i = 0; i < num_items; i++, body++)
        printf("%2d. %s\n",i+1, body->menu_item);
}

static int menu_tail(void){
    int i ; 

    for ( i = 0 ; i < title_len; i++)
        PutCh('-');
    printf("\n 0. Exit\n");
    for ( i = 0 ; i < title_len; i++)
        PutCh('-');

    printf("\n   Select Number: ");

    return strtol(wait_command(), NULL, 0);
}
void root_menu(char *main_menu){
    int menu_num, num_items ;

    num_items = array_num(top_menu);
    menu_header(main_menu);
    menu_body(top_menu, num_items);
    menu_num = menu_tail();
    if ( menu_num > 0 && menu_num <= num_items)
        top_menu[menu_num-1].menu_func();
    else
        PutSt("Unknown Item\r\n");
}

void *wait_command(void){
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
    /*
    scanf("%s",command_buf) ;
    return command_buf ;
    */
}

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
