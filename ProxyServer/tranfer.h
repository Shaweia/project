/*************************************************************************
	> File Name: tranfer.h
	> Author: 
	> Mail: 
	> Created Time: Tue 28 Aug 2018 12:39:43 AM PDT
 ************************************************************************/

#ifndef _TRANFER_H__
#define _TRANFER_H__

#include "epoll.h"

class TranferServer:public EpollServer
{
public:
    TranferServer(int selfport,const char* socks5ip,int socks5port)
        :EpollServer(selfport)
    {
        memset(&_socks5addr,0,sizeof(struct sockaddr_in));
        _socks5addr.sin_family = AF_INET;
        _socks5addr.sin_port = htons(socks5port);
        _socks5addr.sin_addr.s_addr = inet_addr(socks5ip);
    }

    //实现多态
    virtual void ConnectEventHandle(int connectfd);
    virtual void ReadEventHandle(int connectfd);

private:
    struct sockaddr_in _socks5addr;

};




#endif //_TRANFER_H__
