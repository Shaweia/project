/*************************************************************************
	> File Name: sock5.h
	> Author: 
	> Mail: 
	> Created Time: Wed 22 Aug 2018 08:27:47 AM PDT
 ************************************************************************/

#ifndef _SOCK5_H_
#define _SOCK5_H_

#include"epoll.h"

class Sock5Server:public EpollServer
{
public:
    Sock5Server()
    {}
    virtual void ConnectEventHandle(int connectfd);
    virtual void ReadEventHandle(int connectfd);
    virtual void WriteEventHandle(int connectfd);
};

#endif //_SOCKS_H_
