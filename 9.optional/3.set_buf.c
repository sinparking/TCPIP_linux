#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
void error(char *message);

/*
    ! SO_SOCKET中的SO_SNDBUF和SO_RCVBUF，I/O缓冲的大小
    ! int setsockopt(int sock, int level, int optname, void *optval, socklen_t * optlen); 成功返回0，失败返回-1
    ! 将sock中level层的optname选项的值设置为optval（更改信息的缓冲区地址），optlen向第四个参数optval传递可选项信息的字节数
    ? 缓冲区的大小并没有严格按照set的值进行，通过setsockopt向系统传递我们的要求。
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
    recv_buf = 0; //* 实际并非按照传入的值设置
    setsockopt(sock,SOL_SOCKET,SO_RCVBUF,(void*)&recv_buf,len);
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