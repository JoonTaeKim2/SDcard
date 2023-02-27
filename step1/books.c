#include <cli.h>

static void do_art(int argc, char **argv);
static void do_comics(int argc, char **argv);
static void do_history(int argc, char **argv);
static void do_sf(int argc, char **argv);

struct cli_cmd book_cmd[]={
    {"art", "Arts & Photography", do_art},
    {"comic", "Comics & Graphic Novels", do_comics},
    {"history", "History", do_history},
    {"sf", "Science Fiction", do_sf},
};

static int books_items = array_num(book_cmd) ;

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

static void book_help(){
    int i ;

    printf("You can select one of below item \n");
    for ( i = 0; i < books_items; i++){
        printf("%-10s : %s\n", book_cmd[i].cmd_name, book_cmd[i].cmd_help);
    }
}

void do_books(int argc, char **argv)
{
    int     i;

    if (argc == 1)
    {
        book_help();
        return;
    }

    for (i=0; i<books_items; i++)
    {
        if (strcmp(book_cmd[i].cmd_name, argv[1]) == 0)
        {
            book_cmd[i].cli_func(argc, argv);
            break;
        }
    }
}

CLI_CMD(book, "Here is Book Selection corner", do_books);
