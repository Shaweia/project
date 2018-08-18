/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: Sat 18 Aug 2018 02:57:33 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<string.h>

#define MAX_FD sizeof(fd_set)*8

//初始化数组为-1，因为没有那个文件描述符是-1的
void Array_init(int fd_array[],int max_num)
{
    int i = 0;
    for(i < 0; i < max_num; i++)
    {
        fd_array[i] = -1;
    }
}

//往数组中添加文件描述符
int Array_add(int fd_array[],int max_num,int fd)
{
    int i = 0;
    for(i = 0; i < max_num;i++)
    {
        if(fd_array[i] == -1)
        {
            fd_array[i] = fd;
            return 0;
        }
    }
    return -1;
}

void Array_del(int fd_array[],int max_num,int index)
{
    if(index < max_num && index > 0)
    {
        fd_array[index] = -1;
    }
}

//把数组中准备就绪的文件描述符添加到结构体中
//返回最大文件描述符
int set_rfds(int fd_array[],int max_num,fd_set *rfds)
{
    int i = 0;
    int max_fd = -1;
    printf("select:");
    for(i = 0; i < max_num; i++)
    {
        if(fd_array[i] > -1)
        {
            printf("%d",fd_array[i]);
            FD_SET(fd_array[i],rfds);

            if(max_fd < fd_array[i])
            {
                max_fd = fd_array[i];
            }
        }
    }
    printf("\n");
    return max_fd;
}


void service(int fd_array[],int max_num,fd_set *rfds)
{
    int i = 0;
    for(i = 0; i < max_num; i++)
    {
        if(fd_array[i] > -1 && FD_ISSET(fd_array[i],rfds))
        {
            int fd = fd_array[i];
            if(i == 0)
            {
                struct sockaddr_in client;
                socklen_t len = sizeof(client);
                int new_sock = accept(fd,(struct sockaddr*)&client,&len);
                if(new_sock < 0)
                {
                    perror("accept");
                    continue;
                }

                printf("get a new client,[%s:%d]\n",
                     inet_ntoa(client.sin_addr),ntohs(client.sin_port));
                if(Array_add(fd_array,max_num,new_sock) < 0)
                {
                    printf("server is busy\n");
                    close(new_sock);
                }
            }else{
            char buf[10240] = {0};
            ssize_t s = read(fd,buf,sizeof(buf)-1);
            if(s > 0)
            {
                buf[s] = 0;
                printf("client:%s\n",buf);
                write(fd,buf,strlen(buf));
            }else if(s == 0)
            {
                close(fd);
                printf("client quit!\n");
                Array_del(fd_array,max_num,i);
            }else{
                perror("read");
                close(fd);
                Array_del(fd_array,max_num,i);
            }
            }
        } 
    }
}

int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        printf("usage: ./server [ip] [port]\n");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    int listen_fd = socket(AF_INET,SOCK_STREAM,0);
    if(listen_fd < 0)
    {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    int ret = bind(listen_fd,(struct sockaddr*)&addr,sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        return 1;
    }

    ret = listen(listen_fd,5);
    if(ret < 0)
    {
        perror("listen");
        return 1;
    }
    
    
    int fd_array[MAX_FD];
    Array_init(fd_array,MAX_FD);
    Array_add(fd_array,MAX_FD,listen_fd);

    fd_set rfds;
    int max_fd = 0;
    while(1)
    {
        struct timeval timeout = {5,0};
        FD_ZERO(&rfds);
        max_fd = set_rfds(fd_array,MAX_FD,&rfds);
        int tmp = select(max_fd+1,&rfds,NULL,NULL,/*&timeout*/NULL);
        switch(tmp)
        {
            case 0:
                printf("select timeout...!\n");
                break;
            case -1:
                perror("select");
                break;
            default:
                service(fd_array,MAX_FD,&rfds);
                break;
        }
    }


}
