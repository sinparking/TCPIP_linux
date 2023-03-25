#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024
void error(char *message);

// client
int main(int argc, char const *argv[])
{
    int sock;
    struct sockaddr_in addr;
    char message[BUF_SIZE];
    int str_len;

    if (argc != 3)
    {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (-1 == sock)
        error("sock error");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    if (-1 == connect(sock, (struct sockaddr *)&addr, sizeof(addr)))
        error("connect error");
    else
        puts("connected.....");


    close(sock);
    return 0;
}

void error(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

//#############################################################################################################
int main(int argc, char const *argv[])
{
    struct sockaddr_in c_addr;
    struct sockaddr_in s_addr;
    socklen_t c_addr_len;
    int str_len;

    int clnt_sock;
    int serv_sock;

    char message[BUF_SIZE];

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (-1 == serv_sock)
        error("sock error");

    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_addr.sin_port = htons(atoi(argv[1]));
    if (-1 == bind(serv_sock, (struct sockaddr *)&s_addr, sizeof(s_addr)))
        error("bind error");
    if (-1 == listen(serv_sock, 5))
        error("listen error");

    /*
        �����addrlen��ָ���ֵ���Ǳ����ʼ���ģ�����Ҫ��ʼ��Ϊһ�����ڵ���ʵ�ʻ�ȡsocket�����ݳ��ȵ�ֵ����accept������ִ�к󣬻Ὣʵ��ֵ����addrlen��ָ���ֵ
    */
    c_addr_len = sizeof(c_addr);
    
    //! ��������������οͻ��ˣ�֮�󽫹ر�
    for (size_t i = 0; i < 5; i++)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&c_addr, &c_addr_len);
        if (-1 == clnt_sock)
            error("accept error");
        else
            printf("connected client %d \n", i + 1);


        close(clnt_sock);
    }

    close(serv_sock);
    return 0;
}