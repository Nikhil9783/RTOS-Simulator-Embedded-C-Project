#include <stdio.h>
#include <stdlib.h>
#include "semaphore.h"

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
    semaphore lock;

    createTask(NULL, "FirstTask", 1);
    createTask(NULL, "SecondTask", 1);

    semaphoreInit(&lock, 1);
    expect(lock.count == 1, "semaphore stores its initial count");

    semaphoreWait(&lock, &taskList[0]);
    expect(lock.count == 0, "successful wait consumes the semaphore");
    expect(taskList[0].state == TASK_READY, "successful wait does not block the task");

    semaphoreWait(&lock, &taskList[1]);
    expect(taskList[1].state == TASK_BLOCKED, "wait blocks when the semaphore is unavailable");

    semaphoreSignal(&lock);
    expect(lock.count == 1, "signal releases the semaphore");
    expect(taskList[1].state == TASK_READY, "signal wakes a blocked task");

    semaphoreWait(&lock, &taskList[1]);
    expect(lock.count == 0, "woken task can acquire the semaphore");

    puts("PASS: semaphore behavior");
    return EXIT_SUCCESS;
}
