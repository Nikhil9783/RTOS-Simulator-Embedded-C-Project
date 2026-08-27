#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "task.h"

static int executionOrder[3];
static int executionCount;

static void recordAndTerminate(void)
{
    executionOrder[executionCount++] = getCurrentTaskId();
    taskList[currentTaskIndex].state = TASK_TERMINATED;
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
    createTask(recordAndTerminate, "HighPriority", 2);
    createTask(recordAndTerminate, "FirstEqualPriority", 1);
    createTask(recordAndTerminate, "SecondEqualPriority", 1);

    runScheduler();
    runScheduler();
    runScheduler();

    expect(executionCount == 3, "scheduler runs each READY task once");
    expect(executionOrder[0] == 0, "scheduler selects the highest priority task first");
    expect(executionOrder[1] == 1, "scheduler selects the first equal-priority task next");
    expect(executionOrder[2] == 2, "scheduler selects the next equal-priority task in round-robin order");

    puts("PASS: scheduler priority and round-robin behavior");
    return EXIT_SUCCESS;
}
