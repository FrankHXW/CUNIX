#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/wait.h>
#include "server.h"

int main(void)
{
    //打印机器信息:包括Ip和端口
    printMachineInfo1();
    //启动服务:包括监控设备和连接客户端
    startUpServer();
}
