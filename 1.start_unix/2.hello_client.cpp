#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
using std::cerr;
using std::cout;
using std::endl;
using std::string;

/*
    int connect(int sockfd, struct sockaddr * serv_addr, socklen_t addrlen);
*/

auto error_handling(char *message)->void;

int main(int argc, char const *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len;
    if (argc != 3)
    {
        cout << "Usage:" << argv[0] << " <IP> <port>" << endl;
        exit(1);
    }
    //! socket创建套接字之后，调用bind listen即为服务器套接字，调用 connect即为客户端套接字
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); //* ip
    serv_addr.sin_port = htons(atoi(argv[2]));      //* port
    
    //! connect发出连接请求
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");

    str_len = read(sock, message, sizeof(message) - 1);
    if (str_len == -1)
        error_handling("read() error!");

    cout << "Message from server:" << message << endl;
    close(sock);
    return 0;
}

auto error_handling(char *message)->void
{
    cerr<<message<<endl;
    exit(1);
}