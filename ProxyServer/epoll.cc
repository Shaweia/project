/*************************************************************************	   > File Name: epoll.c > Author: 
	> Mail: 
	> Created Time: Wed 22 Aug 2018 08:45:48 AM PDT
 ************************************************************************/

#include"epoll.h"

void SetNoBlocking(int fd)
{
    int flags,s;
    flags = fcntl(fd,F_GETFL,0);
    if(flags == -1)
    {
        ErrorLog("SetNoBlocking:F_GETFL");
    }
    flags != O_NONBLOCK;
    s = fcntl(fd,F_SETFL,flags);
    if(s == -1)
    {
        ErrorLog("SetNoBlocking:F_SETFL");
    }
}


void EpollServer::Start()
{
    _listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(_listenfd < 0)
    {
        ErrorLog("create socket");
        return;
    }

    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(_listenfd,(struct sockaddr*)&addr,sizeof(addr)) < 0)
    {
        ErrorLog("bind sock");
        return;
    }

    if(listen(_listenfd,100000) < 0)
    {
        ErrorLog("listen");
        return;
    }

    TraceLog("epoll server listen on %d",_port);

    _eventfd = epoll_create(100000);
    if(_eventfd == -1)
    {
        ErrorLog("epoll_create");
        return;
    }

    //添加listenfd到epoll,监听连接事件
    SetNoBlocking(_listenfd);
    OpEvent(_listenfd,EPOLLIN,EPOLL_CTL_ADD);

    //进入事件
    EventLoop();
}

void EpollServer:: OpEvent(int fd,int events,int how)
{
    struct epoll_event event;
    event.events = events;
    event.data.fd = fd;
    if(epoll_ctl(_eventfd,how,fd,&event) == -1)
    {
        ErrorLog("epoll_ctl");
    }
}

void EpollServer:: EventLoop()
{
    struct epoll_event events[100000];
    while(1)
    {
        int n = epoll_wait(_eventfd,events,100000,0);
        for(int i = 0; i < n; i++)
        {
            if(events[i].data.fd == _listenfd)
            {
                struct sockaddr clientaddr;
                socklen_t len = 0;
                int connectfd = accept(_listenfd,&clientaddr,&len);
                if(connectfd < 0)
                {
                    ErrorLog("accept");
                }

                ConnectEventHandle(connectfd);
            }
            else if(events[i].events & EPOLLIN)
            {
                ReadEventHandle(events[i].data.fd);
            }
            else if(events[i].events & EPOLLOUT)
            {
                WriteEventHandle(events[i].data.fd);
            }
            else
            {
                ErrorLog("event:%d",events[i].data.fd);
            }
        }        
    }
}


