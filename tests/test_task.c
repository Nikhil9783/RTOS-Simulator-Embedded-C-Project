#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"

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
    createTask(NULL, "FirstTask", 3);
    expect(taskCount == 1, "task count increases after creation");
    expect(taskList[0].taskId == 0, "task IDs start at zero");
    expect(strcmp(taskList[0].taskName, "FirstTask") == 0, "task name is stored");
    expect(taskList[0].priority == 3, "task priority is stored");
    expect(taskList[0].state == TASK_READY, "new task starts READY");
    expect(taskList[0].sleepTicks == 0, "new task starts with no sleep ticks");

    for (int i = 1; i < MAX_TASKS; i++)
        createTask(NULL, "ExtraTask", i);

    expect(taskCount == MAX_TASKS, "task creation stops at the task limit");
    createTask(NULL, "RejectedTask", 1);
    expect(taskCount == MAX_TASKS, "task beyond the limit is rejected");

    expect(strcmp(getTaskStateName(TASK_READY), "READY") == 0, "READY state has a name");
    expect(strcmp(getTaskStateName(TASK_RUNNING), "RUNNING") == 0, "RUNNING state has a name");
    expect(strcmp(getTaskStateName(TASK_BLOCKED), "BLOCKED") == 0, "BLOCKED state has a name");
    expect(strcmp(getTaskStateName(TASK_SLEEPING), "SLEEPING") == 0, "SLEEPING state has a name");
    expect(strcmp(getTaskStateName(TASK_TERMINATED), "TERMINATED") == 0, "TERMINATED state has a name");

    puts("PASS: task creation and state helpers");
    return EXIT_SUCCESS;
}
