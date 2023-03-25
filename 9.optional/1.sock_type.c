#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
void error(char *message);

/*
    ! int getsockopt(int sock, int level, int optname, void *optval, socklen_t * optlen); �ɹ�����0��ʧ�ܷ���-1
    ! �鿴sock��level���optnameѡ�������浽optval��optlen��optval���ݻ����С�����ý����󱣴��ѡ����Ϣ���ֽ���
*/

int main(int argc, char const *argv[])
{
    int tcp_sock, udp_sock;
    int sock_type;
    socklen_t optlen;
    int state;

    optlen = sizeof(sock_type);
    tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
    udp_sock = socket(PF_INET, SOCK_DGRAM, 0);
    printf("SOCK_STREAM:%d \n", SOCK_STREAM);
    printf("SOCK_DGRAM:%d \n", SOCK_DGRAM);

    state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void *)&sock_type, &optlen);
    if (state)
        error("getsockopt error");
    printf("Socket type one:%d \n", sock_type);

    state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void *)&sock_type, &optlen);
    if (state)
        error("getsockopt error");

    printf("Socket type two:%d \n", sock_type);

    return 0;
}

void error(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}