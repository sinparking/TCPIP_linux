#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/*
    1.int socket(int domain, int type, int protocal);
    2.int bind(int sockfd, struct sockaddr * myaddr, socklen_t addrlen);
    3.int listen(int sockfd, int backlog);
    4.int accept(int sockfd, struct sockaddr *addr, socklen_t addrlen);
*/

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[] = "hello world";

    if (argc != 2)
    {
        std::cout << "Usage:" << argv[0] << " <port>" << std::endl;
        exit(1);
    }
    //! socket �����׽���
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //* IP
    serv_addr.sin_port = htons(atoi(argv[1]));  //* port
    //! bind ����IP�Ͷ˿ں�
    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    //! listen ���׽���ת��Ϊ���Խ�������״̬
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    clnt_addr_size = sizeof(clnt_addr);
    //! accept������������������û��������������µ��øú��������᷵�أ�ֱ������������Ϊֹ
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1)
        error_handling("accept() error");
    //! write �������ݴ���
    write(clnt_sock, message, sizeof(message));
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
