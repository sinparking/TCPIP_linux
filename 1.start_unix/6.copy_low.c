#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void ErrorHandling(char *message);

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <from> <to>", argv[0]);
        exit(0);
    }
    int fd1, fd2;
    char buf[30];
    int count = 0;
    fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1)
        ErrorHandling("open() <from> error!");
    fd2 = open(argv[2], O_CREAT | O_WRONLY);
    if (fd2 == -1)
        ErrorHandling("open() <to> error!");
    while ((count = read(fd1, buf, 20)) > 0)
    {
        write(fd2,buf,count);
    }
    close(fd1);
    close(fd2);
    return 0;
}

void ErrorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}