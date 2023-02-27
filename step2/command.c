#include <stdio.h>
#include <command.h>

extern cli_cmd_t __cli_start, __cli_end ;

static void do_help(int argc, char **argv){
    cli_cmd_t *cli_cmd = &__cli_start ;
    int num_commands = &__cli_end - &__cli_start ;

    for ( int i = 0 ; i < num_commands ; i++, cli_cmd++){
        printf("%-10s : %s \n", cli_cmd->cmd_name, cli_cmd->cmd_help);
    }
}
/*
static void do_book(int argc, char **argv){
    printf("Here is book \n");
}
*/
static void do_toys(int argc, char **argv){
    printf("Here is toys \n");
}
static void do_medias(int argc, char **argv){
    printf("Here is medias \n");
}

extern void do_books(int argc, char **argv) ;

//struct cli_cmd __cli_cmd_help __attribute__ ((section (".cli.help"))) = {
//    "help", "Display CLI command usage", do_help} ;

CLI_CMD(help, "Display CLI command usage", do_help);
CLI_CMD(toys, "Toys & Hobbies", do_toys);
CLI_CMD(medias, "DVDs & Movies", do_medias);
//CLI_CMD(book, "Various Books", do_books);
//struct cli_cmd __cli_cmd_book  = {
//    "book", "Book Description", do_book} ;
