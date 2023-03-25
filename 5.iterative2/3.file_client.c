#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#define LEN_SIZE 4
#define BUF_SIZE 1024

//! 1.strcat的destination必须有空闲的空间，不然会改变其他变量的值
//! 2.循环读取数据写入缓冲的时候 如果是从&data[index]开始读，也就是说把数据全部读到缓冲区，那么必须保证缓冲区能接受所有数据。不然每次从data开始写

void error(char *message);

int main(int argc, char const *argv[])
{
    int sock;
    struct sockaddr_in sock_addr;
    socklen_t clnt_addr_len;

    if (argc != 3)
    {
        printf("Usage: %s <IP> <PORT>\n", argv[0]);
        exit(1);
    }
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(argv[1]);
    sock_addr.sin_port = htons(atoi(argv[2]));
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (-1 == connect(sock, (struct sockaddr *)&sock_addr, sizeof(sock_addr)))
        error("connect error");
    else
        fputs("connecting....\n", stdout);

    char data[BUF_SIZE] = {0};
    int file_name_len, file_len;
    int file;
    int recv_len, recv_cnt;
    fputs("enter file name:", stdout);
    fgets(data, BUF_SIZE, stdin);
    // 消除末尾的回车
    data[strlen(data) - 1] = 0;
    file_name_len = strlen(data);
    // 发送文件名的长度和文件名
    write(sock, (char *)&file_name_len, LEN_SIZE);
    write(sock, data, file_name_len);
    // 接收文件长度
    file_len = 0;
    read(sock, &file_len, LEN_SIZE);
    // 接收文件
    if (file_len == -1)
    {
        fputs("no file found in server\n", stdout);
    }
    else
    {
        char recv_file_name[100] = "recv_";
        strcat(recv_file_name, data);
        printf("file found,length:%d recieving....\n", file_len);
        file = open(recv_file_name, O_CREAT | O_WRONLY | O_TRUNC);
        recv_len = 0;
        while (recv_len < file_len)
        {
            recv_cnt = read(sock, data, BUF_SIZE);
            printf("%d\n", recv_cnt);
            if (recv_cnt == -1)
                error("read error");
            // 写入文件
            write(file, data, recv_cnt);
            recv_len += recv_cnt;
        }
        close(file);
    }

    fputs("closing...\n", stdout);
    close(sock);
    return 0;
}

void error(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}