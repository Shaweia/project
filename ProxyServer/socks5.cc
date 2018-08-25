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

    //添加connetfd 到epoll，监听事件
    SetNonBlocking(connectfd);
    OpEvent(connectfd,EPOLLIN,EPOLL_CTL_ADD);

    Connect* con = new Connect;
    con->_state = AUTH;
    con->_clientChannel._fd = connectfd;
    _fdConnectMap[connectfd] = con;
    con->_ref++;
}

// 0 表示数据没到，继续等待
// 1 成功
// -1 失败
int Sock5Server::AuthHandle(int fd)
{
    size_t len = 256;
    char buf[len];

    //窥探缓冲区是否有足够的数据
    int rlen = recv(fd,buf,len,MSG_PEEK);
    if(rlen <= 0)
    {
        return -1;
    }
    else if(rlen < 3)
    {
        return 0;
    }
    else{
    int n = recv(fd,buf,len,0);

    if(buf[0] != 0x05)
    {
        ErrorLog("not socks5");
        return -1;
    }
/*
    buf[0] = 0x05;
    buf[1] = 0x0;
    if(send(fd,buf,2,0) != 2)
    {
        ErrorLog("reply sock5");
        return -1;
    }
*/
    return 1;
    }
}

//失败 -1
//数据没有到返回 -2
//连接成功 返回serverfd
int Sock5Server::EstablishmentHandle(int fd)
{
    size_t len = 256;
    char buf[len];

    //窥探缓冲区是否有足够的数据。返回0，表示数据还没有到
    int rlen = recv(fd,buf,len,MSG_PEEK);
    if(rlen <= 0)
    {
        return -1;
    }
    else if(rlen < 10)
    {
        return -2;
    }
    else{
        char ip[4];
        char port[2];
        //ipv4  直接给出IP地址的
        recv(fd,buf,4,0);

        if(buf[3] == 0x01) 
        {
            recv(fd,ip,4,0);
            recv(fd,port,2,0);

            TraceLog("ipv4:%s,%s",ip,port);
        }
        else if(buf[3] == 0x03)  //domain Name
        {
            char len = 0;
            recv(fd,&len,1,0);

            recv(fd,buf,len,0);
            buf[len] = '\0';
            TraceLog("domianname:%s ",buf);

            recv(fd,port,2,0);

            //通过域名取‘IP’
            struct hostent* hptr = gethostbyname(buf);
            if(hptr == NULL)
            {
                ErrorLog("gethostbyname():%s",buf);
                return -1;
            }
            struct in_addr addr;
            memcpy(ip,hptr->h_addr,hptr->h_length);
        }
        else if(buf[3] == 0x04) //ipv6
        {
            ErrorLog("ipv6 not suport");
            return -1;
        }
        else{
            ErrorLog("未知协议");
            return -1;
        }
        struct sockaddr_in addr;
        memset(&addr,0,sizeof(struct sockaddr_in));
        addr.sin_family = AF_INET;
        memcpy(&addr.sin_addr.s_addr,ip,4);
        addr.sin_port = *((uint16_t*)port);
        int serverfd = socket(AF_INET,SOCK_STREAM,0);
        if(serverfd < 0)
        {
            ErrorLog("server socket");
            return -1;
        }
        if(connect(serverfd,(struct sockaddr*)&addr,sizeof(addr)) < 0)
        {
            ErrorLog("connect error");
            close(serverfd);
            return -1;
        }
        return serverfd;
    }
}


void Sock5Server::ReadEventHandle(int connectfd)
{
    //TraceLog("connect event:%d",connectfd);
    map<int,Connect*>::iterator it = _fdConnectMap.find(connectfd);
    if(it != _fdConnectMap.end())
    {
        Connect* con = it->second;
        if(con->_state == AUTH)
        {
            char reply[2];
            reply[0] = 0x05;
            int ret = AuthHandle(connectfd);
            if(ret == 0)
            {
                return;
            }
            else if(ret == 1)
            {
                reply[1] = 0x00;
                con->_state = ESTABLISHMENT;
            }
            else if(ret == -1)
            {
                reply[1] = 0xFF;
                RemoveConnect(connectfd);
            }

            if(send(connectfd,reply,2,0) != 2)
            {
                ErrorLog("auth reply");
            }
        }
        else if(con->_state == ESTABLISHMENT)
        {
            char reply[10] = {0};
            reply[0] = 0x05;

            int serverfd = EstablishmentHandle(connectfd);
            if(serverfd == -1)
            {
                reply[1] = 0x01;
                RemoveConnect(connectfd);
            }
            else if(serverfd == -2)
            {
                return;
            }
            else{
                reply[1] = 0x00;
                reply[3] = 0x01;
            }

            if(send(connectfd,reply,10,0) != 10)
            {
                ErrorLog("Establishment reply");
            }
            if(serverfd >= 0)
            {
                SetNonBlocking(serverfd);
                OpEvent(serverfd,EPOLLIN,EPOLL_CTL_ADD);
                con->_serverChannel._fd = serverfd;
                _fdConnectMap[serverfd] = con;
                con->_ref++;
                con->_state = FORWARDING;
            }
        }
        else if(con->_state == FORWARDING)
        {
            Channel* clientChannel = &con->_clientChannel;
            Channel* serverChannel = &con->_serverChannel;

            if(connectfd == serverChannel->_fd)
            {
                swap(clientChannel,serverChannel);
            }
            //client->server
            Forwarding(clientChannel,serverChannel);
        }
        else{
            assert(false);
        }
    }
    else{
        assert(false);
    }
}

int main()
{
    Sock5Server server(8001);
    server.Start();
}
