#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
void error(char *message);

void error(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}