/*************************************************************************
	> File Name: epoll.h
	> Author: 
	> Mail: 
	> Created Time: Wed 22 Aug 2018 08:28:33 AM PDT
 ************************************************************************/

#ifndef _EPOLL_H_
#define _EPOLL_H_

#include"common.h"

class EpollServer{
public:
    EpollServer(int port = 8000)
        :_port(port)
        ,_listenfd(-1)
        ,_eventfd(-1)
    {}

    ~EpollServer()
    {
        if(_listenfd)
            close(_listenfd);
    }

    void Start();
    void EventLoop();
    void OpEvent(int fd,int events,int how);

    virtual void ConnectEventHandle(int connectfd) = 0;
    virtual void ReadEventHandle(int connectfd) = 0;
    virtual void WriteEventHandle(int connectfd) = 0;


private:
    int _port;      //服务端口号
    int _listenfd;  //监听套接字
    int _eventfd;   //事件描述符
};

#endif//_EPOLL_H_
