#include "server.h"

#define NI_MAXHOST 1025
#define NI_NUMERICHOST 1

int printMachineInfo1(void){
   struct ifaddrs *ifaddr, *ifa;
   int family, s;
   char host[NI_MAXHOST];

   if (getifaddrs(&ifaddr) == -1) {
       perror("getifaddrs");
      return -1;;
   }

   /* Walk through linked list, maintaining head pointer so we
      can free list later */

   for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
       if (ifa->ifa_addr == NULL)
           continue;

       family = ifa->ifa_addr->sa_family;

       /* Display interface name and family (including symbolic
          form of the latter for the common families) */

       printf("%s  address family: %d%s\n",
               ifa->ifa_name, family,
               (family == AF_PACKET) ? " (AF_PACKET)" :
               (family == AF_INET) ?   " (AF_INET)" :
               (family == AF_INET6) ?  " (AF_INET6)" : "");

       /* For an AF_INET* interface address, display the address */

       if (family == AF_INET || family == AF_INET6) {
           s = getnameinfo(ifa->ifa_addr,
                   (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                         sizeof(struct sockaddr_in6),
                   host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
           if (s != 0) {
               printf("getnameinfo() failed: %s\n", gai_strerror(s));
               return -1;;
           }
           printf("\taddress: <%s>\n", host);
       }
   }
   printf("  device listen port: %d\n",DEVICE_LISTEN_PORT);
   printf("  client listen port: %d\n\n",CLIENT_LISTEN_PORT);
   freeifaddrs(ifaddr);
   return 0;

}


int printMachineInfo(void)
{
    struct ifaddrs * ifAddrStruct=NULL;
    void * tmpAddrPtr=NULL;
    getifaddrs(&ifAddrStruct);
    while (ifAddrStruct!=NULL) {
	printf("this is IP4 judge\n");
        if (ifAddrStruct->ifa_addr->sa_family==AF_INET) { // check it is IP4
            // is a valid IP4 Address
	    printf("this is in IP4\n");
            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            printf("  %s IP Address: %s\n", ifAddrStruct->ifa_name, addressBuffer); 
        } 
	printf("this is IP6 judge\n");
        if (ifAddrStruct->ifa_addr->sa_family==AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            printf("  %s IP Address: %s\n", ifAddrStruct->ifa_name, addressBuffer); 
        } 
	printf("this is try to get next\n");
        ifAddrStruct=ifAddrStruct->ifa_next;
    }
    printf("  device listen port: %d\n",DEVICE_LISTEN_PORT);
    printf("  client listen port: %d\n\n",CLIENT_LISTEN_PORT);
    return 0;
}


int createSocketAndBind(int _port)
{
    int serverFd;
    struct sockaddr_in serverAddr;
    int errorCode=0;
    //创建套接字描述符
    if((errorCode=serverFd=socket(AF_INET,SOCK_STREAM,0))<0){
        printf(" create server failed! port: %d errno: %d\n",_port,errno);
        return -1;
    }
    //设置端口可复用，否则上次程序结束后再重启会导致绑定不成功
    int flag=1;
    if((errorCode=setsockopt(serverFd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag)))<0){
        printf("create server failed! port: %d errno: %d\n",_port,errno);
        return -2;
    }
    if(setsockopt(serverFd,IPPROTO_TCP,TCP_NODELAY,&flag,sizeof(flag))<0){
        printf("create server failed! port:%d errno:%d\n",_port,errno);
        return -3;
    }

    //绑定套接字
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_addr.s_addr=INADDR_ANY;
    serverAddr.sin_port=htons(_port);
    if((errorCode=bind(serverFd,(struct sockaddr *)&serverAddr,sizeof(serverAddr)))<0){
        printf("create server failed! port: %d errno: %d\n",_port,errno);
        return -4;
    }
    return serverFd;
}

int makeSocketNonBlocking(int _socketFd)
{
    int flags=fcntl(_socketFd,F_GETFL,0);
    if(flags==-1){
        printf("make socket non blocking failed! errno: %d\n",errno);
        return -1;
    }
    if(fcntl(_socketFd,F_SETFL,flags|O_NONBLOCK)==-1){
         printf("make socket non blocking failed! errno: %d\n",errno); 
        return -1;
    }
    return 0;
}

//可重入函数:添加epoll事件
int epollAddEvent(int _epollFd,int _fd,int _state)
{
    struct epoll_event event;
    event.events=_state;
    event.data.fd=_fd;
    if(epoll_ctl(_epollFd,EPOLL_CTL_ADD,_fd,&event)<0){
        printf("epoll add events failed! epollFd:%d fd:%d state:%d errno: %d\n",_epollFd,_fd,_state,errno);
        return -1;
    }    
    return 0;
}

//可重入函数:删除事件
int epollDeleteEvent(int _epollFd,int _fd,int _state)
{
    struct epoll_event event;
    event.events=_state;
    event.data.fd=_fd;
    if(epoll_ctl(_epollFd,EPOLL_CTL_DEL,_fd,&event)<0){
        printf("epoll delete events failed! epollFd:%d fd:%d state:%d errno:%d\n",_epollFd,_fd,_state,errno);
        return -1;
    }    
    return 0;
}

//可重入函数:修改事件状态
int epollModifyEvent(int _epollFd,int _fd,int _state)
{
    struct epoll_event event;
    event.events=_state;
    event.data.fd=_fd;
    if(epoll_ctl(_epollFd,EPOLL_CTL_MOD,_fd,&event)<0){
        printf("epoll modify events failed! epollFd:%d fd:%d state:%d errno:%d\n",_epollFd,_fd,_state,errno);
        return -1;
    }    
    return 0;
}

//可重入函数:处理错误
int epollHandleError(int _epollFd,int _fd,int _state)
{
    printf("epoll_%d's error: fd:%d state:%d\n",_epollFd,_fd,_state);
    epollDeleteEvent(_epollFd,_fd,_state);
    close(_fd);
    return 0;
}


//可重入函数:处理连接请求
int epollHandleAccept(int _epollFd,int _listenFd)
{
    int clientFd;
    struct sockaddr_in clientAddr;
    int c=sizeof(struct sockaddr_in);
    clientFd=accept(_listenFd,(struct sockaddr *)&clientAddr,(socklen_t *)&c);
    if(clientFd==-1){
        printf("epoll handle accept failed! epollFd: %d listenFd: %d errno: %d\n",_epollFd,_listenFd,errno);
        return -1;
    }
    else{
        char str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET,&(clientAddr.sin_addr),str,INET_ADDRSTRLEN);
        printf("epoll %d-listenFd %d accept new connect: fd %d %s:%d\n",_epollFd,_listenFd,clientFd,str,clientAddr.sin_port);
        //设置新接入的socket为非阻塞
        makeSocketNonBlocking(clientFd);
        //为新接入的socket注册事件
        epollAddEvent(_epollFd,clientFd,EPOLLIN|EPOLLET);
        return clientFd;
    }
}

//可重入函数:处理读请求
int epollHandleRead(int _epollFd,int _fd,char *_buffer,int _bufferSize)
{
    int nRead;
    memset(_buffer,0,_bufferSize);
    nRead=recv(_fd,_buffer,_bufferSize,0);
    if(nRead==-1){
        printf("epoll handle read error! epollFd: %d fd: %d errno: %d\n",_epollFd,_fd,errno);
        close(_fd);     //记住这里一定要关闭_fd 
        epollDeleteEvent(_epollFd,_fd,EPOLLIN);
        return -1;
    }
    else if(nRead==0){
        printf("epoll handle read error: client closed! epollFd: %d fd: %d errno: %d\n",_epollFd,_fd,errno);
        close(_fd);     //记住这里一定要关闭_fd
        epollDeleteEvent(_epollFd,_fd,EPOLLIN);
        return -2;
    }
    else{
//        printf("epoll handle read: readed %d: ",nRead);
//        for(int i=0;i<nRead;++i){
//            printf("%c",_buffer[i]);
//        }
//      printf("\n");
        //修改描述符对应的事件，由读改为写
//        epollModifyEvent(_epollFd,_fd,EPOLLOUT);
        return nRead;
    }
}

//可重入函数:处理写请求
int epollHandleWrite(int _epollFd,int _fd,char *_buffer,int _bufferSize)
{
    printf("this is epoll handle write\n");
    int nWrite;
    nWrite=write(_fd,_buffer,_bufferSize);
    if(nWrite==-1){
        printf("epoll handle write error! epollFd: %d fd: %d errno: %d\n",_epollFd,_fd,errno);
        close(_fd);     //记住这里一定要关闭_fd
        epollDeleteEvent(_epollFd,_fd,EPOLLOUT);
        return -1;
    }
    else{
        //修改描述符对应的事件，由写改为读
        epollModifyEvent(_epollFd,_fd,EPOLLIN);
        return 0;
    }
}

int startUpServer(void)
{
    //两个进程:前台进程监听客户端设备，后台进程监听采集设备
    pid_t processId=fork();
    if(processId==0){
        //插入延时确保前台本地服务进程创建成功
        usleep(10000);
        //建立前台和后台进程间传送数据的本地socket连接
        int localClientFd;
        struct sockaddr_in localServerAddr;
        localClientFd=socket(AF_INET,SOCK_STREAM,0);
        if(makeSocketNonBlocking(localClientFd)<0){
            printf("create local client socket failed! errno: %d\n",errno);
            exit(-1);
        }
        //设置端口可复用，否则上次程序结束后再重启会导致绑定不成功
        int flag=1;
        if(setsockopt(localClientFd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag))<0){
            printf("create local client socket failed! errno: %d\n",errno);
            exit(-2);
        }
        if(setsockopt(localClientFd,IPPROTO_TCP,TCP_NODELAY,&flag,sizeof(flag))<0){
            printf("create local client socket failed!errno:%d\n",errno);
            exit(-3);
        }

        memset(&localServerAddr,0,sizeof(localServerAddr));
        localServerAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
        localServerAddr.sin_family=AF_INET;
        localServerAddr.sin_port=htons(LOCAL_LISTEN_PORT);
        //连接本地服务器
        int errorCode=0;
        while((errorCode=connect(localClientFd,(struct sockaddr *)&localServerAddr,sizeof(localServerAddr)))<0){
            printf("create local client failed! errno: %d\n",errno);
            usleep(10000);

           // exit(-2);
        }
        printf("create local client success!\n");
        //监听传感设备
        listenSensors(localClientFd);
        exit(0);    
    }
    else{
        //建立前台和后台进程间传送数据的本地socket连接
        int localServerFd=createSocketAndBind(LOCAL_LISTEN_PORT); 

        //监听套接字
        int errorCode=0;
        if((errorCode=listen(localServerFd,1))<0){
            printf("create local server failed! errno: %d\n",errno);
            exit(-2);
        }
        printf("create local server success! waiting for client....\n");
        //等待后台进程的连接
        int localClientFd;
        struct sockaddr_in localClientAddr;
        int c=sizeof(struct sockaddr_in);
        if((localClientFd=accept(localServerFd,(struct sockaddr *)&localClientAddr,(socklen_t *)&c))<0){
            printf("create local server failed! errno: %d\n",errno);
            exit(-3);
        }
        makeSocketNonBlocking(localClientFd);
        usleep(100000);
        //监听客户端
        listenClients(localClientFd);
        exit(0);
    }    
}


int listenSensors(int _localClientFd)
{
    //建立监听传感器设备的套接字
    int sensorsListenFd=createSocketAndBind(DEVICE_LISTEN_PORT);
    //设置套接字为非阻塞
    int errorCode=0;
    if((errorCode=makeSocketNonBlocking(sensorsListenFd))<0){
        printf("create sensors listen scoket failed! errno: %d\n",errno);
        return -1;
    }
    //监听套接字，SOMAXCONN为系统默认的最大连接个数
    if((errorCode=listen(sensorsListenFd,SOMAXCONN))<0){
        printf("create sensors listen scoket failed! errno: %d\n",errno);
        return -2;
    }

    //以下为使用IO多路复用模型函数epool来监控设备的接入
    struct epoll_event events[EPOLL_EVENTS_SIZE];
    int epollFd;
    if((epollFd=epoll_create(EPOLL_FD_SIZE))<0){
        printf("create epoll failed! errno: %d\n",errno);
        return -3;
    }
    //添加监听描述符事件
    epollAddEvent(epollFd,sensorsListenFd,EPOLLIN|EPOLLET);

    char buffer[SENSORS_DATA_MAX_SIZE];
    memset(buffer,'A',SENSORS_DATA_MAX_SIZE);
    
    std::map<int,int> connectedSensorMap;
    
    //循环等待
    while(1){
        int eventsNum=epoll_wait(epollFd,events,EPOLL_EVENTS_SIZE,-1);
        for(int i=0;i<eventsNum;++i){
            //根据事件描述符类型和事件类型进行处理
            int fdTmp=events[i].data.fd;
            //处理错误和挂起
            if(events[i].events&(EPOLLERR|EPOLLHUP|EPOLLRDHUP)){
                epollHandleError(epollFd,fdTmp,events[i].events);
            }
            //处理有新连接接入
            else if((fdTmp==sensorsListenFd)&&(events[i].events&EPOLLIN)){
                int connectedSensor=epollHandleAccept(epollFd,sensorsListenFd);
                if(connectedSensor>0){
                    connectedSensorMap.insert(std::pair<int,int>(connectedSensor,connectedSensor));
                }
            }
            else if(events[i].events&EPOLLIN){
                  int nRecv=epollHandleRead(epollFd,fdTmp,buffer,SENSORS_DATA_MAX_SIZE);
                  if(nRecv==SENSORS_DATA_FRAME_SIZE){
                      send(_localClientFd,buffer,SENSORS_DATA_FRAME_SIZE,0);   
                  }
                  else if(nRecv<0){
                      connectedSensorMap.erase(fdTmp);
                  }
                  
                  printf("valid sensor fd: ");
                  for(std::map<int,int>::iterator it=connectedSensorMap.begin();it!=connectedSensorMap.end();++it){
                      printf("%d ",it->second);   
                  }
                  printf("\n");
            }
            else if(events[i].events&EPOLLOUT){
//                epollHandleWrite(epollFd,fdTmp,buffer,SENSORS_DATA_MAX_SIZE);
            }
        }
    }
    free(events);
    close(sensorsListenFd);
    return 0;
}

int listenClients(int _localServerFd)
{
    int clientListenFd=createSocketAndBind(CLIENT_LISTEN_PORT);
    //设置套接字为非阻塞
    int errorCode=0;
    if((errorCode=makeSocketNonBlocking(clientListenFd))<0){
        printf("create client listen scoket failed! errno: %d\n",errno);
        return -1;
    }
    //监听套接字，SOMAXCONN为系统默认的最大连接个数
    if((errorCode=listen(clientListenFd,SOMAXCONN))<0){
        printf("create client listen scoket failed! errno: %d\n",errno);
        return -2;
    }

    //以下为使用IO多路复用模型函数epool来监控接入
    struct epoll_event events[EPOLL_EVENTS_SIZE];
    int epollFd;
    if((epollFd=epoll_create(EPOLL_FD_SIZE))<0){
        printf("create epoll failed! errno: %d\n",errno);
        return -3;
    }
    //添加监听描述符事件
    epollAddEvent(epollFd,clientListenFd,EPOLLIN|EPOLLET);

    epollAddEvent(epollFd,_localServerFd,EPOLLIN|EPOLLET);

    char buffer[SENSORS_DATA_MAX_SIZE];
    memset(buffer,'0',SENSORS_DATA_MAX_SIZE);

    //this is used for record valid client fd
    std::map<int,int> connectedClientMap;

    //循环等待
    while(1){
        int eventsNum=epoll_wait(epollFd,events,EPOLL_EVENTS_SIZE,-1);
        for(int i=0;i<eventsNum;++i){
            //根据事件描述符类型和事件类型进行处理
            int fdTmp=events[i].data.fd;
            uint32_t eventsTmp=events[i].events;
//            printf("localServerFd:%d  clientListenFd:%d fdTmp:%d  events: %d EPOLLERR:%d EPOLLHUP:%d EPOLLRDHUP:%d EPOLLIN:%d EPOLLOUT:%d\n",_localServerFd,clientListenFd,fdTmp,eventsTmp,EPOLLERR,EPOLLHUP,EPOLLRDHUP,EPOLLIN,EPOLLOUT);
            //处理错误和挂起
            if((eventsTmp&EPOLLERR)||(eventsTmp&EPOLLHUP)||(eventsTmp&EPOLLRDHUP)){
                printf("epoll error occured!\n");
                epollHandleError(epollFd,fdTmp,eventsTmp);
            }
            //处理有新连接接入
            else if((fdTmp==clientListenFd)&&(eventsTmp&EPOLLIN)){
                int connectedClient=epollHandleAccept(epollFd,clientListenFd);
                if(connectedClient>0){
                    connectedClientMap.insert(std::pair<int,int>(connectedClient,connectedClient));
                }
            }
            //this is used for read local client in background process 's data
            else if((fdTmp==_localServerFd)&&(eventsTmp&EPOLLIN)){
                memset(buffer,'\0',SENSORS_DATA_MAX_SIZE);
                int nRecv=recv(_localServerFd,buffer,SENSORS_DATA_MAX_SIZE,0); 
                if(nRecv==SENSORS_DATA_FRAME_SIZE){
                    for(int i=0;i<SENSORS_DATA_FRAME_SIZE;++i){
                //        printf("|%c",buffer[i]);
                    }
                    printf("valid client fd: ");
                    for(std::map<int,int>::iterator it=connectedClientMap.begin();it!=connectedClientMap.end();++it){
                        printf("%d ",it->second);   
                        send(it->second,buffer,SENSORS_DATA_FRAME_SIZE,0);
                    }
                    printf("\n");
                }
                else{
                    printf("local client received %d bytes errno: %d\n",nRecv,errno);
                }
            }
            //this is used for handle other client's data
            else if(eventsTmp&EPOLLIN){
                 if(epollHandleRead(epollFd,fdTmp,buffer,SENSORS_DATA_MAX_SIZE)<0){
                    //read error occured! remove fd from valid connected map
                    connectedClientMap.erase(fdTmp);
                 }
            }
            else if(eventsTmp&EPOLLOUT){
//                epollHandleWrite(epollFd,fdTmp,buffer,SENSORS_DATA_MAX_SIZE);
            }
        }     
    }
    free(events);
    close(clientListenFd);
    return 0;
}












