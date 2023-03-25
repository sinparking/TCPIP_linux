#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#define LEN_SIZE 4
#define BUF_SIZE 1024

void error(char *message);

int main(int argc, char const *argv[])
{
    int serv, clnt;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_len;

    if (argc != 2)
    {
        printf("Usage: %s <PORT>\n", argv[0]);
        exit(1);
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (-1 == (serv = socket(PF_INET, SOCK_STREAM, 0)))
        error("socket error");
    if (-1 == bind(serv, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
        error("bind error");
    if (-1 == listen(serv, 5))
        error("listen error");
    clnt_addr_len = sizeof(clnt_addr);

    char data[BUF_SIZE] = {0};
    int file_name_len;
    FILE *fp;
    int read_cnt;
    int send_len;
    int recv_len, recv_cnt;
    while (1)
    {
        if (-1 == (clnt = accept(serv, (struct sockaddr *)&clnt_addr, &clnt_addr_len)))
            error("accept error");
        else
            printf("connecting for %s:%d\n", inet_ntoa(clnt_addr.sin_addr), clnt_addr.sin_port);
        // 读取文件名长度
        file_name_len = 0;
        read(clnt, &file_name_len, LEN_SIZE);
        // 读取文件名
        //! 这种争取一次性全部读到缓冲区的方法必须缓冲区够大
        recv_len = 0;
        while (recv_len < file_name_len)
        {
            recv_cnt = read(clnt, &data[recv_len], file_name_len);
            if (recv_cnt == -1)
                error("read error");
            recv_len += recv_cnt;
        }
        data[file_name_len] = 0;
        printf("\"%s\":%d\n", data, file_name_len);
        if (NULL == (fp = fopen(data, "rb")))
        {
            printf("No file found:\"%s\"\n", data);
            send_len = -1;
            write(clnt, (char *)&send_len, LEN_SIZE);
        }
        else
        {
            fseek(fp, 0, SEEK_END);
            // 获取文件长度
            send_len = ftell(fp);
            rewind(fp);
            printf("file found,length:%d,sending....\n", send_len);
            // send_len = 1; //todo 还没有实现读取文件的长度
            write(clnt, (char *)&send_len, LEN_SIZE);
            while (0 != (read_cnt = fread((void *)data, 1, BUF_SIZE, fp)))
            {
                printf("%d\n", read_cnt);
                if (read_cnt < BUF_SIZE)
                {
                    write(clnt, data, read_cnt);
                    break;
                }
                write(clnt, data, BUF_SIZE);
            }
            fclose(fp);
        }
        fputs("closing....\n", stdout);
        close(clnt);
    }

    close(serv);
    return 0;
}

void error(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}