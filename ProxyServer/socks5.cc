/*************************************************************************
	> File Name: socks5.c
	> Author: 
	> Mail: 
	> Created Time: Wed 22 Aug 2018 09:46:49 AM PDT
 ************************************************************************/

#include"socks5.h"

void Sock5Server::ConnectEventHandle(int connectfd)
{
    TraceLog("new connect event:%d",connectfd);


}

void Sock5Server::ReadEventHandle(int connectfd)
{
    TraceLog("connect event:%d",connectfd);
}
void Sock5Server::WriteEventHandle(int connectfd)
{
    TraceLog("");
}



int main()
{
    Sock5Server server;
    server.Start();
}
