#include <stdio.h>
#include <stdlib.h>
#include "ipc.h"

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
    MessageQueue queue;
    int messages[QUEUE_MAX_SIZE + 1];
    void *received = NULL;

    initQueue(&queue);
    expect(queue.size == 0, "queue starts empty");
    expect(receiveMessage(&queue, &received) == -1, "empty queue rejects receive");

    for (int i = 0; i < QUEUE_MAX_SIZE; i++)
    {
        messages[i] = i;
        expect(sendMessage(&queue, &messages[i]) == 0, "queue accepts messages until full");
    }

    messages[QUEUE_MAX_SIZE] = QUEUE_MAX_SIZE;
    expect(sendMessage(&queue, &messages[QUEUE_MAX_SIZE]) == -1, "full queue rejects another message");

    for (int i = 0; i < QUEUE_MAX_SIZE; i++)
    {
        expect(receiveMessage(&queue, &received) == 0, "queue returns a message");
        expect(*(int *)received == i, "queue preserves FIFO order");
    }

    expect(queue.size == 0, "queue is empty after all messages are received");
    puts("PASS: message queue behavior");
    return EXIT_SUCCESS;
}
