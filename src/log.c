#include <stdio.h>
#include "log.h"

static void writeCsvField(FILE *file, const char *value)
{
    fputc('"', file);
    for (const char *cursor = value; *cursor != '\0'; cursor++)
    {
        if (*cursor == '"')
            fputc('"', file);
        fputc(*cursor, file);
    }
    fputc('"', file);
}

int exportLogCsv(const char *filename, const char logBuffer[][64], int logCount)
{
    if (filename == NULL || logBuffer == NULL || logCount < 0)
        return -1;

    FILE *file = fopen(filename, "w");
    if (file == NULL)
        return -1;

    fprintf(file, "index,message\n");
    for (int i = 0; i < logCount; i++)
    {
        fprintf(file, "%d,", i);
        writeCsvField(file, logBuffer[i]);
        fputc('\n', file);
    }

    if (fclose(file) != 0)
        return -1;

    return 0;
}
