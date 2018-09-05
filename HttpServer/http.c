/*************************************************************************
	> File Name: http.c
	> Author: 
	> Mail: 
	> Created Time: Sun 12 Aug 2018 12:05:31 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h> 
#include<sys/socket.h> 
#include<netinet/in.h> 
#include<arpa/inet.h>
#include<signal.h>
#include<pthread.h>
#include<sys/sendfile.h>


#define MAX 1024
#define HOME_PAGE "index.html"

static int startup(int port)
{
    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0)
    {
        perror("socket");
        exit(2);
    }

    int opt = 1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(port);

    if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0)
    {
        perror("bind");
        exit(3);
    }

    if(listen(sock,5) < 0)
    {
        perror("listen");
        exit(4);
    }

    return sock;
}

//获取首行  方法 + url + 版本号
//recv 相当于copy
//由于每个浏览器 换行的表达不一样   如： \n  \r  \r\n  都换成 \n
int get_line(int sock,char line[],int size)
{
    int c = 'a';
    int i = 0;
    ssize_t s = 0;
    while(i<size-1 && c != '\n')
    {
        s = recv(sock,&c,1,0);
        if(s > 0)
        {
            if(c == '\r')
            {
                //\r->\n  or  \r\n->\n
                recv(sock,&c,1,MSG_PEEK);
                if(c != '\n')
                {
                    c = '\n';
                }
                else
                {
                    recv(sock,&c,1,0);
                }
            }    
            //c == \n
            line[i++] = c;
        }
        else{
            break;
        }
    }
    line[i] = '\0';
  //  printf("%s\n",line);
    return i;
}


void clear_header(int sock)
{
    char line[MAX];
    do{
        get_line(sock,line,sizeof(line));
    }while(strcmp(line,"\n") != 0);
}

void show_404(int sock)
{
    char line[MAX];
    sprintf(line,"HTTP/1.0 404 Not Found\r\n");
    send(sock,line,strlen(line),0);
    sprintf(line,"Content_Type: text/html\r\n");
    send(sock,line,strlen(line),0);
    sprintf(line,"\r\n");
    send(sock,line,strlen(line),0);

    const char* err = "wwwroot/404.html";
    int fd = open(err,O_RDONLY);
    struct stat st;
    stat(err,&st);
    sendfile(sock,fd,NULL,st.st_size);
    close(fd);
}
void echo_error(int sock,int code)
{
    clear_header(sock);
    switch(code)
    {
        case 403:
            break;
        case 404:
            show_404(sock);
            break;
        case 501:
            break;
        case 503:
            break;
        default:
            break;
    }
}

void echo_www(int sock,char *path,int size,int *err)
{
    clear_header(sock);

    int fd = open(path,O_RDONLY);
    if(fd < 0)
    {
        *err = 404;
        return;
    }

    char line[MAX];
    sprintf(line,"HTTP/1.0 200 OK\r\n");
    send(sock,line,strlen(line),0);
    sprintf(line,"Content_Type: text/html\r\n");
    send(sock,line,strlen(line),0);

   // sprintf(line,"Content_Type: imag/jpg\r\n");
   // send(sock,line,strlen(line),0);
    sprintf(line,"\r\n");
    send(sock,line,strlen(line),0);
    sendfile(sock,fd,NULL,size);
    close(fd);

}

int exe_cgi(int sock,char path[],char method[],char *query_string)
{
    char line[MAX];
    int content_length = -1;
    char method_env[MAX/32];
    char query_string_env[MAX];
    char content_length_env[MAX/16];

    if(strcasecmp(method,"GET") == 0)
    {
        clear_header(sock);
    }else{
        do{
            get_line(sock,line,sizeof(line));
            if(strncmp(line,"Content_Length:",16) == 0)
            {
                content_length = atoi(line+16);
            }
        }while(strcmp(line,"\n") != 0);
        if(content_length == -1)
        {
            return 404;
        }
    }

    sprintf(line,"HTTP/1.0 200 OK\r\n");
    send(sock,line,strlen(line),0);
    sprintf(line,"Content_Type: text/html\r\n");
    send(sock,line,strlen(line),0);
    sprintf(line,"\r\n");
    send(sock,line,strlen(line),0);

    int input[2];
    int output[2];

    pipe(input);
    pipe(output);

    pid_t id = fork();
    if(id < 0)
    {
        return 404;
    }
    else if(id == 0)//child
    {
        //输出重定向到标准输入输出
        close(input[1]);
        close(output[0]);
        dup2(input[0],0);
        dup2(output[1],1);

        sprintf(method_env,"METHOD=%s",method);
        putenv(method_env);
        if(strcasecmp(method,"GET")==0)
        {
            sprintf(query_string_env,"QUERY_STRING=%s",query_string);
            putenv(query_string_env);
        }
        else{
            sprintf(content_length_env,"CONTENT_LENGTH=%d",\
                    content_length);
            putenv(content_length_env);
        }
        printf("method:%s");
        //method,GET[query_string], POST[content_leng]
        execl(path,path,NULL);
        exit(1);
    }
    else
    {//father
        close(input[0]);
        close(output[1]);

        char c;
        if(strcasecmp(method,"POST")==0)
        {
            int i = 0;
            for(;i<content_length;i++)
            {
                read(sock,&c,1);
                write(input[1],&c,1);
            }
        }
        
        while(read(output[0],&c,1) >0)
        {
            send(sock,&c,1,0);    
        }
        
        waitpid(id,NULL,0);    
        close(input[1]);
        close(output[0]);
    }
    return 200;
}

static void *handler_request(void *arg)
{
    int sock = (int)arg;
    char line[MAX];
    char method[MAX/32];
    char url[MAX];
    char path[MAX];
    int errCode = 200;
    int cgi = 0;
    char *query_string = NULL;

/*#ifdef Debug
    do{
        get_line(sock,line,sizeof(line));
        printf("%s",line);
    }while(strcmp(line,"\n")!=0);

#else*/

    if(get_line(sock,line,sizeof(line))<0)
    {
        errCode = 404;
        goto end;
    }
    
    //get method
    //line[] = GET /index.php?a=100&&b=200 HTTP/1.1
    int i = 0;
    int j = 0;
    while(i < sizeof(method)-1 && j < sizeof(line) && !isspace(line[j]))
    {
        method[i] = line[j];
        i++;
        j++;
    }
    method[i] = '\0';

    //处理方法大小写  GEt post Post get
    if(strcasecmp(method,"GET") == 0)
    {
    }
    else if(strcasecmp(method,"POST") == 0)
    {
        cgi = 1;
    }
    else
    {
        errCode = 404;
        goto end;
    }
    
    while(j < sizeof(line) && isspace(line[j]))
    {
        j++;
    }
    i = 0;
    while(i < sizeof(url)-1 && j<sizeof(line) && !isspace(line[j]))
    {
        url[i] = line[j];
        j++;
        i++;
    }
    url[i] = '\0';

    //url
    //如果为GET请求，query_string 为空，则静态页面
    //如果不为空，则cgi = 1，动态页面
    if(strcasecmp(method,"GET") == 0)
    {
        query_string = url;
        while(*query_string)
        {
            if(*query_string == '?')
            {
                *query_string = '\0';
                query_string++;
                cgi = 1;
                break;
            }
            query_string++;
        }
    }

    //url -> wwwroot/a/b/c.html | url -> wwwroot/
    sprintf(path,"wwwroot%s",url);
    if(path[strlen(path)-1] == '/')
    {
        strcat(path,HOME_PAGE);
        // 构建路径 ：wwwroot/index.html
    }
    
    //stat结构体是获取文件的属性
    //提供文件名字，获取文件对应属性
    //成功返回0 失败返回-1
    struct stat st;
    if(stat(path,&st) < 0)
    {
        errCode = 404;
        goto end;
    }
    else
    {
        if(S_ISDIR(st.st_mode))
        {
            strcat(path,HOME_PAGE);
        }
        else
        {
            if((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || \
            (st.st_mode & S_IXOTH))
            {
                cgi = 1;
            }
        }

        if(cgi)
        {
            printf("method: %s,path: %s\n",method,path);
            errCode = exe_cgi(sock,path,method,query_string);
        }
        else
        {
            printf("method: %s,path: %s\n",method,path);

            echo_www(sock,path,st.st_size,&errCode);
        }
    }



//#endif

end:
    if(errCode != 200)
    {
        echo_error(sock,errCode);
    }
    close(sock);
    
}

int main(int argc,char *argv[])
{
    if(argc != 2)
    {
        printf("usage: ./server [port]\n");
        return 1;
    }

    int listen_sock = startup(atoi(argv[1]));

    signal(SIGPIPE,SIG_IGN);

    for(;;)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int new_sock = accept(listen_sock,(struct sockaddr *)&peer,&len);
        if(new_sock<0)
        {
            perror("accept error!\n");
            continue;
        }

        pthread_t tid;
        pthread_create(&tid,NULL,handler_request,(void *)new_sock);
        pthread_detach(tid);
    }
}
