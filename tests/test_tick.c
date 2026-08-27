#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "timer.h"

static int callbackCalls;

static void timerCallback(void)
{
    callbackCalls++;
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
    register_Timer_callback(timerCallback);

    expect(getTickCounter() == 0, "tick counter starts at zero");
    for (int i = 0; i < 4; i++)
        tick();
    expect(getTickCounter() == 4, "tick counter increments once per tick");
    expect(callbackCalls == 0, "timer callback does not run before five ticks");

    tick();
    expect(getTickCounter() == 5, "fifth tick is counted");
    expect(callbackCalls == 1, "timer callback runs on the fifth tick");

    for (int i = 0; i < 5; i++)
        tick();
    expect(getTickCounter() == 10, "tick counter reaches ten after ten ticks");
    expect(callbackCalls == 2, "timer callback runs once per five ticks");

    puts("PASS: system tick behavior");
    return EXIT_SUCCESS;
}
