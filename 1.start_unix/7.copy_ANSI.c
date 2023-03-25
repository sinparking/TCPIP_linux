#include <stdio.h>
#include <stdlib.h>

void ErrorHandling(char *message);

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <from> <to>", argv[0]);
        exit(0);
    }
    FILE *fd1, *fd2;
    char buf[30];
    int count;


    fd1 = fopen(argv[1], "r");
    if (fd1 == NULL)
        ErrorHandling("fopen() <from> error!");
        
    fd2 = fopen(argv[2], "aw");
    if (fd2 == NULL)
        ErrorHandling("fopen() <to> error!");
    
    while ((count = fread(buf, 1, 30, fd1)) > 0)
    {
        printf("%d",count);
        fwrite(buf, 1, count, fd2);
    }

    fclose(fd1);
    fclose(fd2);
    return 0;
}

void ErrorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}