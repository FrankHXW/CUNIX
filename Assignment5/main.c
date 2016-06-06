#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/wait.h>
#include "csv.h"


int main(void)
{
    char *asciiFileName="./test.csv";    
    char binFileName[20]; 
    memset(binFileName,'\0',20);
    strcat(binFileName,asciiFileName);
    strcat(binFileName,".bin");

    //通过管道在父子进程间传递数据
    readCSVAsciiAndSaveBin(asciiFileName,binFileName);

    //通过mmap映射二进制文件到内存来打印
    readCSVBinAndPrintByMMap(binFileName);

    return 0;
}














