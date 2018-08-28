
#include "tranfer.h"


void TranferServer::ConnectEventHandle(int connectfd)
{
    int serverfd = socket(AF_INET,SOCK_STREAM,0);
    if(serverfd == -1)
    {
        ErrorLog("socket");
        return;
    }

    if(connect(serverfd,(struct sockaddr*)&_socks5addr,sizeof(_socks5addr)) < 0)
    {
        ErrorLog("connect socks5");
        return;
    }

    //设置成非阻塞，并且添加到epoll
    SetNonBlocking(connectfd);
    OpEvent(connectfd,EPOLLIN,EPOLL_CTL_ADD);

    SetNonBlocking(serverfd);
    OpEvent(serverfd,EPOLLIN,EPOLL_CTL_ADD);

    //设置到map中保存连接
    Connect* con = new Connect;
    con->_state = FORWARDING;

    con->_clientChannel._fd = connectfd;
    con->_ref++;
    _fdConnectMap[connectfd] = con;

    con->_serverChannel._fd = serverfd;
    con->_ref++;
    _fdConnectMap[serverfd] = con;
    TraceLog("zhuanfa!!!");
}

void TranferServer::ReadEventHandle(int connectfd)
{
    map<int,Connect*>::iterator it = _fdConnectMap.find(connectfd);
    if(it != _fdConnectMap.end())
    {
        Connect* con = it->second;
        Channel* clientChannel = &con->_clientChannel;
        Channel* serverChannel = &con->_serverChannel;
        
        //加密处理
        bool sendencry = true,recvdecrypt = false;
        if(connectfd == con->_serverChannel._fd)
        {
            swap(clientChannel,serverChannel);
            swap(sendencry,recvdecrypt);
        }

        Forwarding(clientChannel,serverChannel,sendencry,recvdecrypt);
    }
    else{
        assert(false);
    }
}


int main()
{
    TranferServer server(8000,"192.168.78.143",8001);
    server.Start();
}
