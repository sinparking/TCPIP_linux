#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

void error(char *msg);

/*
    UDP: SOCK_DGRAM
    UDP���������� echo_server.c socket->bind->sendto/recvfrom
    TCP����������               socket->bind->listen->accept->read/write
    ! 1 ssize_t recvfrom(int sock, void *buff, size_t nbytes, int flags, struct sockaddr * from, socklen_t *addrlen);
    ! 2 ssize_t sendto(int sock, void *buff, size_t nbytes, int flags, struct sockaddr *to, socklen_t addrlen); //��һsendto ��ʱ��ᶯ̬�󶨵�ַ�Ͷ˿�
*/

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(0);
    }
    //! ����Ҫclnt��ֻ��Ҫclnt_addr
    //! ����serv��serv_addr�Ǳ�����ַ��clnt_addr�Ƿ��Ͷ����ĵ�ַ
    int serv;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_len;
    char message[BUF_SIZE];
    int message_len;

    if ((serv = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
        error("socket error");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if (bind(serv, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error("bind error");
    while (1)
    {
        //* ͨ��serv���ղ���ͨ��serv����
        clnt_addr_len = sizeof(clnt_addr);
        message_len = recvfrom(serv, message, BUF_SIZE, 0, (struct sockaddr *)&clnt_addr, &clnt_addr_len);
        sendto(serv, message, message_len, 0, (struct sockaddr *)&clnt_addr, clnt_addr_len);
    }

    close(serv);
    return 0;
}

void error(char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}