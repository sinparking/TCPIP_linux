#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test01()
{
    printf("char%d\n", sizeof(char));
    printf("short int%d\n", sizeof(short int));
    printf("int%d\n", sizeof(int));
    printf("long int%d\n", sizeof(long int));
    printf("float%d\n", sizeof(float));
    printf("double%d\n", sizeof(double));
    printf("long long%d\n", sizeof(long long));
    printf("long double%d\n", sizeof(long double));
}

void test02()
{
    char buf[200];
    int i = 5, j;
    buf[0] = (char)i; //! 这里是把int强转成char，因此只占1字节
    j = buf[0];
    printf("%d\n", j);
}

void test03()
{
    char buf[200];
    int index = 0;
    for (int i = 0; i < 3; ++i)
    {
        scanf("%d", (int *)&buf[index]); //! 这里是scanf把int写入char *的地址中，因此一个int要占4字节
        index += 4;
    }
    for (int i = 0; i < 3; i++)
        printf("%d\n", ((int *)buf)[i]);
}

void test04(void *message)
{
    int *p = (int *)message;
    printf("%d\n", *p);
}

void test05()
{
    FILE *fp;
    fp = fopen("aa", "rb");
    if (fp == NULL)
        ;
    else
        fclose(fp);
}

int main(int argc, char const *argv[])
{
    // test02();
    // test03();
    int i = 10;
    // test04((char *)&i);
    test05();
    return 0;
}
