/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: Thu 09 Aug 2018 10:22:30 PM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<arpa/inet.h>

// ./client  127.0.0.1 8080
int main(int argc,char *argv[])
{
    if(argc != 3)
    {
        printf("usage: ./client [ip]  [port]");
        return 1;
    }
    //创建文件描述符sock
    int sock  = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0)
    {
        printf("create socket error\n");
        return 2;
    }

    struct sockaddr_in client_socket;
    client_socket.sin_family = AF_INET;
    client_socket.sin_port = htons(atoi(argv[2]));
    client_socket.sin_addr.s_addr = inet_addr(argv[1]);
    
    //调用connect() 函数  请求和服务器连接（三次握手）
    int ret = connect(sock,(struct sockaddr*)&client_socket,sizeof(client_socket));
    if(ret < 0)
    {
        printf("connect failed...\n");
        return 3;
    }

    printf("connect success..\n");
    char buf[1024]; 
    memset(buf,'\0',sizeof(buf));

    //循环请求服务器，服务器响应
    while(1)
    {
        printf("client:#");
        fgets(buf,sizeof(buf),stdin);
        socklen_t len = strlen(buf);
        buf[len-1] = '\0';
        write(sock,buf,sizeof(buf));

        printf("pleas wait...\n");

        read(sock,buf,sizeof(buf));
        printf("server # : %s\n",buf);
    }
    
    close(sock);
    return 0;

}

