#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
void error(char *message);

/*
    ! SO_SOCKET中的SO_SNDBUF和SO_RCVBUF，I/O缓冲的大小
*/

int main(int argc, char const *argv[])
{
    int sock;
    int send_buf, recv_buf, state;
    socklen_t len;
    sock = socket(PF_INET, SOCK_STREAM, 0);
    len = sizeof(send_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void *)&send_buf, &len);
    if (state)
        error("getsockopt error");
    len = sizeof(recv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void *)&recv_buf, &len);
    if (state)
        error("getsockopt error");

    printf("Input buffer size :%d \n",recv_buf);
    printf("Output buffer size :%d \n",send_buf);
    return 0;
}

void error(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}