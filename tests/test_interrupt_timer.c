#include <stdio.h>
#include <stdlib.h>
#include "interrupt.h"
#include "timer.h"

static int interruptCalls;
static int timerCalls;

static void interruptHandler(void)
{
    interruptCalls++;
}

static void timerCallback(void)
{
    timerCalls++;
}

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
    init_timer();
    registerInterrupt(2, interruptHandler);
    triggerInterrupt(2);
    expect(interruptCalls == 1, "registered interrupt handler runs");

    triggerInterrupt(-1);
    triggerInterrupt(MAX_INTERRUPTS);
    expect(interruptCalls == 1, "invalid interrupts do not run the handler");

    register_Timer_callback(timerCallback);
    timer_isr();
    expect(timerCalls == 1, "timer callback runs");

    init_timer();
    timer_isr();
    expect(timerCalls == 1, "timer initialization clears the callback");

    puts("PASS: interrupt and timer behavior");
    return EXIT_SUCCESS;
}
