#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

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
    const char filename[] = "test_log_output.csv";
    char logBuffer[2][64] = {"simple message", "message, with \"quotes\""};
    char output[256] = {0};
    FILE *file;
    size_t bytesRead;

    expect(exportLogCsv(filename, logBuffer, 2) == 0, "CSV export succeeds");

    file = fopen(filename, "r");
    expect(file != NULL, "CSV file is created");
    bytesRead = fread(output, 1, sizeof(output) - 1, file);
    fclose(file);
    remove(filename);

    output[bytesRead] = '\0';
    expect(strcmp(output, "index,message\n0,\"simple message\"\n1,\"message, with \"\"quotes\"\"\"\n") == 0,
           "CSV output escapes fields correctly");
    expect(exportLogCsv("missing-directory/test.csv", logBuffer, 2) == -1,
           "CSV export reports file errors");
    expect(exportLogCsv(filename, logBuffer, 0) == 0, "empty CSV export succeeds");
    remove(filename);

    puts("PASS: log CSV export");
    return EXIT_SUCCESS;
}
