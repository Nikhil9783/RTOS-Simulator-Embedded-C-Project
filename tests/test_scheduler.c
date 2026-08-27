#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "task.h"

static void sleepingTask(void)
{
    taskSleep(2);
}

static void idleTestTask(void)
{
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
    createTask(sleepingTask, "SleepingTask", 2);
    createTask(idleTestTask, "TestTask1", 1);
    createTask(idleTestTask, "TestTask2", 1);
    createTask(idleTestTask, "ShellTask", 1);

    runScheduler();
    expect(currentTaskIndex == 0, "scheduler tracks the selected task");
    expect(taskList[0].state == TASK_SLEEPING, "taskSleep marks the task sleeping");
    expect(taskList[0].sleepTicks == 2, "taskSleep stores the requested duration");

    tick();
    expect(taskList[0].state == TASK_SLEEPING, "task remains asleep before duration expires");
    expect(taskList[0].sleepTicks == 1, "tick decrements the sleep duration");

    tick();
    expect(taskList[0].state == TASK_READY, "task wakes after its sleep duration");
    expect(taskList[0].sleepTicks == 0, "sleep duration reaches zero on wake-up");

    puts("PASS: scheduler task sleep and wake-up");
    return EXIT_SUCCESS;
}
