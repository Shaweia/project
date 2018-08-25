/*************************************************************************
	> File Name: epoll.h
	> Author: 
	> Mail: 
	> Created Time: Wed 22 Aug 2018 08:28:33 AM PDT
 ************************************************************************/

#ifndef _EPOLL_H_
#define _EPOLL_H_

#include"common.h"

class IgnoreSigPipe
{
public:
    IgnoreSigPipe()
    {
        ::signal(SIGPIPE,SIG_IGN);
    }
};
static IgnoreSigPipe initPIPE_IGN;

class EpollServer{
public:
    EpollServer(int port)
        :_port(port)
        ,_listenfd(-1)
        ,_eventfd(-1)
    {}

    virtual  ~EpollServer()
    {
        if(_listenfd != -1)
            close(_listenfd);
    }


    enum Socks5State
    {
        AUTH,             //身份认证
        ESTABLISHMENT,    //建立连接
        FORWARDING,       //转发
    };

    struct Channel{
        int _fd;         //文件描述符
        string _buf;   //写缓冲

        Channel()
            :_fd(-1)
        {}
    };

    struct Connect
    {
        Socks5State _state;    //连接的状态
        Channel _clientChannel;
        Channel _serverChannel;
        int _ref;

        Connect()
            :_state(AUTH)
            ,_ref(0)
        {}
    };
    
    void Start();
    void EventLoop();
    void OpEvent(int fd,int events,int op);   //操作时间，添加或者删除
    void SetNonBlocking(int fd);               //设置文件描述符为非阻塞
    void SendInLoop(int fd,const char* buf,int len);
    void Forwarding(Channel* clientChannel,Channel* serverChannel);
    void RemoveConnect(int fd);

    //多态实现虚函数
    virtual void ConnectEventHandle(int connectfd) = 0;
    virtual void ReadEventHandle(int connectfd) = 0;
    virtual void WriteEventHandle(int connectfd);


protected:
    int _port;      //服务端口号
    int _listenfd;  //监听套接字
    int _eventfd;   //事件描述符
    map<int,Connect*> _fdConnectMap;  //fd映射连接的map容器
};

#endif//_EPOLL_H_
