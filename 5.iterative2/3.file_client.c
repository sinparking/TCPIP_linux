#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#define LEN_SIZE 4
#define BUF_SIZE 1024

//! 1.strcat��destination�����п��еĿռ䣬��Ȼ��ı�����������ֵ
//! 2.ѭ����ȡ����д�뻺���ʱ�� ����Ǵ�&data[index]��ʼ����Ҳ����˵������ȫ����������������ô���뱣֤�������ܽ����������ݡ���Ȼÿ�δ�data��ʼд

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
    // ����ĩβ�Ļس�
    data[strlen(data) - 1] = 0;
    file_name_len = strlen(data);
    // �����ļ����ĳ��Ⱥ��ļ���
    write(sock, (char *)&file_name_len, LEN_SIZE);
    write(sock, data, file_name_len);
    // �����ļ�����
    file_len = 0;
    read(sock, &file_len, LEN_SIZE);
    // �����ļ�
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
            // д���ļ�
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