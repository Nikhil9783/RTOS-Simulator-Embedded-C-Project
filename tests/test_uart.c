#include <stdio.h>
#include <stdlib.h>
#include "uart.h"

static void expect(int condition, const char *message)
{
    if (!condition)
    {
        fprintf(stderr, "FAIL: %s\n", message);
        exit(EXIT_FAILURE);
    }
}

int main(void)
{
    char received;

    UART_init();
    expect(UART_receive(&received) == -1, "empty receive is rejected");

    expect(UART_send('A') == 0, "UART accepts a character");
    expect(UART_send('B') == 0, "UART accepts a second character");
    UART_isr();
    UART_isr();

    expect(UART_receive(&received) == 0 && received == 'A', "UART preserves first character");
    expect(UART_receive(&received) == 0 && received == 'B', "UART preserves second character");
    expect(UART_receive(&received) == -1, "UART receive is empty after draining");

    for (int i = 0; i < UART_BUFFER_SIZE; i++)
        expect(UART_send('X') == 0, "UART accepts characters until full");
    expect(UART_send('Y') == -1, "UART rejects a character when TX is full");

    puts("PASS: UART buffer behavior");
    return EXIT_SUCCESS;
}
