#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#include "csv.h"


int convAsciiLineBufferToStuInfoLE(char *_buffer,struct stuInfoLE *_dst)
{
    char *pieceStartPos=_buffer;
    char bufferPiece[MAX_ISSUE_SIZE];
    memset(bufferPiece,'\0',MAX_ISSUE_SIZE);
    
    for(int i=0;i<MAX_ISSUE_SIZE;++i){
        if(*(pieceStartPos+i)==','){
            pieceStartPos+=(i+1);
            break;
        }
        else{
            bufferPiece[i]=pieceStartPos[i];
        }
    }
    sscanf(bufferPiece,"%u",&_dst->Id);
  

    memset(bufferPiece,'\0',MAX_ISSUE_SIZE);
    memset(_dst->Name,'\0',MAX_ISSUE_SIZE);
    for(int i=0;i<MAX_ISSUE_SIZE;++i){
        if(*(pieceStartPos+i)==','){
            pieceStartPos+=(i+1);
            break;
        }
        else{
           *(_dst->Name+i)=pieceStartPos[i];
        }
    }

    memset(bufferPiece,'\0',MAX_ISSUE_SIZE);
    memset(_dst->Department,'\0',MAX_ISSUE_SIZE);
    for(int i=0;i<MAX_ISSUE_SIZE;++i){
        if(*(pieceStartPos+i)==','){
            pieceStartPos+=(i+1);
            break;
        }
        else{
           *(_dst->Department+i)=pieceStartPos[i];
        }
    }


    memset(bufferPiece,'\0',MAX_ISSUE_SIZE);
    for(int i=0;i<MAX_ISSUE_SIZE;++i){
        if(*(pieceStartPos+i)=='\n'){
//            pieceStartPos+=(i+1);
            break;
        }
        else{
           bufferPiece[i]=pieceStartPos[i];
        }
    }
    sscanf(bufferPiece,"%u",&_dst->Age);

    return 0;
}

int convStuInfoFromLEToBE(struct stuInfoLE *_src,struct stuInfoBE *_dst)
{
    //将小端整数转为大端整数
    _dst->Id=htonl(_src->Id);
    _dst->Age=htonl(_src->Age);

    //由于是字符串，没有大端小端之分，故不用转换  
    memcpy(_dst->Name,_src->Name,MAX_ISSUE_SIZE);
    memcpy(_dst->Department,_src->Department,MAX_ISSUE_SIZE);
    return 0;
}


int convStuInfoFromBEToLE(struct stuInfoBE *_src,struct stuInfoLE *_dst)
{
     //将大端整数转为小端整数
    _dst->Id=ntohl(_src->Id);
    _dst->Age=ntohl(_src->Age);

    //由于是字符串，没有大端小端之分，故直接copy
    memcpy(_dst->Name,_src->Name,MAX_ISSUE_SIZE);
    memcpy(_dst->Department,_src->Department,MAX_ISSUE_SIZE);
    return 0;
}



//空白行将不会计入
int countFileLines(FILE *_fp)
{
    int count=0;
    char preC='0';
    char c='0';
    for(c=getc(_fp);c!=EOF;preC=c,c=getc(_fp)){
        if((c=='\n')&&(preC!='\n')){        
            ++count;
        }
    }
    return count;
}

int readCSVAsciiAndSaveBin(char *_asciiFileName,char *_binFileName) 
{
    //打开输入Ascii文件
    FILE *fpAscii=fopen(_asciiFileName,"r");
    if(fpAscii==NULL){
        printf("open .csv ascii file failed!\n");
        return -1;
    }

    int linesCount=countFileLines(fpAscii);

    //设置fp回到stream头位置
    fseek(fpAscii,0,SEEK_SET);
    char buffer[MAX_LINE_SIZE];
    memset(buffer,'\0',MAX_LINE_SIZE);
    
    //创建多个子进程，读取管道数据进行转码
    int processCount=linesCount;
    pid_t*  childPidPtr;                 
    childPidPtr=(pid_t *)malloc(processCount);      //子进程数等于行数
    int parentPipePtr[2*processCount];              //父进程写管道
    int childPipePtr[2*processCount];               //子进程写管道
    for(int i=0;i<processCount;++i){
        if(pipe(&parentPipePtr[2*i])<0){
            printf("create parentPipe %d failed!\n",i);
            return -1;
        }
        if(pipe(&childPipePtr[2*i])<0){
            printf("create childPipe %d failed!\n",i);
            return -1;
        }
    }
    for(int i=0;i<processCount;++i){
        childPidPtr[i]=fork();
        if(childPidPtr[i]==-1){
            printf("create child process %d failed!\n",i);
            break;                      //确保只有父进程在创建子进程
        }
        else{
           //每个子进程读取对应与i编号的小文件并转成bin文件
            if(childPidPtr[i]==0){
                //子进程从管道读取父进程发送的数据
                close(parentPipePtr[2*i+1]);
                read(parentPipePtr[2*i],buffer,MAX_LINE_SIZE);
                //解析读取的行数据到小段结构体
                struct stuInfoLE stuInfoLETmp;
                struct stuInfoBE stuInfoBETmp;
                convAsciiLineBufferToStuInfoLE(buffer,&stuInfoLETmp);
                //将小段结构体转为大端结构体
                convStuInfoFromLEToBE(&stuInfoLETmp,&stuInfoBETmp);
                //子进程通过管道发送转码后的数据
                close(childPipePtr[2*i]);
                write(childPipePtr[2*i+1],(char *)&stuInfoBETmp,sizeof(stuInfoBETmp));
                exit(0);
            }
            else{ 
                printf("create child process %d success!\n",childPidPtr[i]);  
            }
        }
    }

    //父进程拆分每一行文本，通过管道发送给指定的子进程，子进程的代码不会运行到此处
    for(int i=0;i<processCount;++i){
        memset(buffer,'\0',MAX_LINE_SIZE);
        if(fgets(buffer,MAX_LINE_SIZE,fpAscii)!=NULL){
            close(parentPipePtr[2*i]);
            write(parentPipePtr[2*i+1],buffer,MAX_LINE_SIZE);
        }
    }
    fclose(fpAscii);
    printf("\nread ascii file and send to child process success!\n\n");

    //父进程等待所有子进程退出,子进程的代码不会运行到此处
    for(int i=0;i<processCount;++i){
        int status;
        waitpid(childPidPtr[i],&status,WUNTRACED);     //阻塞式等待
        if(status==0){
            printf("child process %d exit success!\n",childPidPtr[i]);
        }
        else{
            return -1;
        }
    }
    free(childPidPtr);

    //父进程读取各个转码后的小bin文件转为一个大的bin文件
    FILE *fpBin=fopen(_binFileName,"w");
    if(fpBin==NULL){
        printf("create %s file failed!\n",_binFileName);
        return -1;
    }
    for(int i=0;i<processCount;++i){
        memset(buffer,'\0',MAX_LINE_SIZE);
        close(childPipePtr[2*i+1]);
        read(childPipePtr[2*i],buffer,MAX_LINE_SIZE);
        fwrite(buffer,sizeof(struct stuInfoBE),1,fpBin); 
    }
    fclose(fpBin);
    printf("\nmerge small bin files success!\n\n");
    return 0;
}


int readCSVBinAndPrint(char *_binFileName)
{
    FILE *fpBin=fopen(_binFileName,"r");
    if(fpBin==NULL){
        printf("open .csv.bin file failed!\n");
        return -1;
    }
   
    //加载二进制数据
    struct stuInfoLE stuInfoLETmp;
    struct stuInfoBE stuInfoBETmp;
    
    while(1){
        //读取二进制文件到大端结构体
        memset((char*)&stuInfoBETmp,'\0',sizeof(stuInfoBETmp));
        if(fread((char *)(&stuInfoBETmp),sizeof(stuInfoBETmp),1,fpBin)==1){    
            //从大端转换到小端并打印结果
            convStuInfoFromBEToLE(&stuInfoBETmp,&stuInfoLETmp);
            printf("%u,%s,%s,%u\n",stuInfoLETmp.Id,stuInfoLETmp.Name,stuInfoLETmp.Department,stuInfoLETmp.Age);
        }
        else{
            break;
        }
    }
    printf("read bin file and convert to struct success!\n\n");

    return 0;
}


//使用mmap来完成二进制文件的读
int readCSVBinAndPrintByMMap(char *_binFileName)
{
    int fdBin;
    if((fdBin=open(_binFileName,O_RDONLY))<0){
        printf("open %s file failed!",_binFileName);
        return -1;
    }
    
    struct stat fileStat;
    if(fstat(fdBin,&fileStat)<0){
        printf("file stat failed!");
        return -2;
    }

    char *fpStartAddr;
    if((fpStartAddr=mmap(NULL,fileStat.st_size,PROT_READ,MAP_SHARED,fdBin,0))==MAP_FAILED){ 
        printf("mmap failed!");
        return -3;
    }
    
    //读取二进制文件到大端结构体
    struct stuInfoLE stuInfoLETmp;
    struct stuInfoBE stuInfoBETmp;
    int linesCount=fileStat.st_size/sizeof(struct stuInfoBE);
    char *addrTemp=fpStartAddr;
    for(int i=0;i<linesCount;++i,addrTemp+=sizeof(struct stuInfoBE)){
        memset((char *)&stuInfoBETmp,'\0',sizeof(stuInfoBETmp));
        memcpy((char *)&stuInfoBETmp,addrTemp,sizeof(stuInfoBETmp));
        //从小段结构体转为大端结构体并打印结果
        convStuInfoFromBEToLE(&stuInfoBETmp,&stuInfoLETmp);
        printf("%u,%s,%s,%u\n",stuInfoLETmp.Id,stuInfoLETmp.Name,stuInfoLETmp.Department,stuInfoLETmp.Age);
    }
    //必须要卸载映射
    if(munmap(fpStartAddr,fileStat.st_size)<0){
        printf("unmap failed!");
        return -4;
    }
    printf("read bin file and convert to struct by mmap success!\n\n");
    return 0;
}








































