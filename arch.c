#include <avr/io.h>
#include <stdio.h>

extern void PutCh(char ch);
extern char GetCh(void);
extern void UART_Init(void);

static int uart_tx(char ch, FILE *stream);

static FILE mystdio = FDEV_SETUP_STREAM(uart_tx, NULL, _FDEV_SETUP_RW);

static int uart_tx(char ch, FILE *stream)
{
    if (ch == '\n')
        uart_tx('\r', stream);

    PutCh(ch);
    return 0 ;
}

void Init_UART(void)
{
    UART_Init();
    stdout = stdin = &mystdio ;
}

/*
void arch_init(void)
{
    cli();
    uart2_init();
}*/
