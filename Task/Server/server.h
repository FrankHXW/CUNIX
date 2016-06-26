#ifndef _SERVER_H
#define _SERVER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/wait.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <errno.h>
#include <map>

#define LOCAL_LISTEN_PORT   8888
#define DEVICE_LISTEN_PORT  12345
#define CLIENT_LISTEN_PORT  12346

#define EPOLL_FD_SIZE 100  
#define EPOLL_EVENTS_SIZE 100

#define SENSORS_DATA_MAX_SIZE  100
#define SENSORS_DATA_FRAME_SIZE 80

//打印机器信息:包括Ip和端口
int  printMachineInfo(void);

int printMachineInfo1(void);

//可重入函数:创建一个绑定到指定端口的套接字
int createSocketAndBind(int _port);

//可重入函数:设置socket为非阻塞式
int makeSocketNonBlocking(int _socketFd);

//可重入函数:添加epoll事件
int epollAddEvent(int _epollFd,int _fd,int _state);

//可重入函数:删除事件
int epollDeleteEvent(int _epollFd,int _fd,int _state);

//可重入函数:修改事件状态
int epollModifyEvent(int _epollFd,int _fd,int _state);

//可重入函数:处理错误
int epollHandleError(int _epollFd,int _fd,int _state);

//可重入函数:处理连接请求
int epollHandleAccept(int _epollFd,int _listenFd);

//可重入函数:处理读请求
int epollHandleRead(int _epollFd,int _fd,char *_buffer,int _bufferSize);

//可重入函数:处理写请求
int epollHandleWrite(int _epollFd,int _fd,char *_buffer,int _bufferSize);

//启动服务:包括监控设备和连接客户端
int startUpServer(void);

//后台进程:监听采集设备的连接，并发送数据到前台进程
int listenSensors(int _localClientFd);  

//前台进程:监听客户端设备的连接，接收后台进程数据发送给客户端
int listenClients(int _localServerFd);

#endif











