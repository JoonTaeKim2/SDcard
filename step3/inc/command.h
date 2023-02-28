struct cli_cmd{
    char *cmd_name ;
    char *cmd_help ;
    void (*cli_func)(int, char **);
};

typedef struct cli_cmd cli_cmd_t ;

#define CLI_CMD(name,help,cmd) \
        cli_cmd_t __cli_cmd_##name __attribute__ ((section (".cli."#name)))= \
        {#name, help, cmd}
