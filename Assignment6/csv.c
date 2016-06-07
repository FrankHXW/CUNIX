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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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
   
    printf("creating servers......\n");
   
    //创建多个子进程作为服务器端，每个端口不一致
    int processCount=linesCount;
    pid_t*  childPidPtr;                 
    childPidPtr=(pid_t *)malloc(processCount);      //子进程数等于行数
    int *portPtr;                                   //端口个数等于进程数
    portPtr=(int *)malloc(processCount);
    for(int i=0;i<processCount;++i){
        portPtr[i]=18888+i;
    }
    for(int i=0;i<processCount;++i){
        childPidPtr[i]=fork();
        if(childPidPtr[i]==-1){
            printf("create child process %d failed!\n",i);
            break;                      //确保只有父进程在创建子进程
        }
        else{
           //每个子进程作为服务器端，接收客户端发来的文件并将其转码后返回
            if(childPidPtr[i]==0){
                printf("create server %d success!\n",i);
                int srv_sock,clt_sock,c,read_size;
                struct sockaddr_in srv,clt;
                int errorCode=0;
                //创建对应不同端口的套接口
                if((errorCode=srv_sock=socket(AF_INET,SOCK_STREAM,0))<0){
                    printf("server %d create socket error! error code:%d\n",i,errorCode);
                    exit(-1);
                }
                //设置端口可复用，否则上次结束后这次绑定不成功
                int yes=1;
                if(setsockopt(srv_sock,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes))==-1){
                    printf("server %d set reuse address error!",i);
                    exit(-2);
                }
                memset(&srv,0,sizeof(srv));
                srv.sin_family=AF_INET;
                srv.sin_addr.s_addr=INADDR_ANY;             //在所有地址上监听
                srv.sin_port=htons(portPtr[i]);             //监听不同端口
                //绑定套接字
                if((errorCode=bind(srv_sock,(struct sockaddr *)&srv,sizeof(srv)))<0){
                    printf("server %d bind error!error code:%d\n",i,errorCode);
                    exit(-3);
                }
                //监听套接字
                if((errorCode=listen(srv_sock,3)<0)){
                    printf("server %d listen error!error code:%d\n",i,errorCode);
                    exit(-4);
                }
                //等待客户端连接
                c=sizeof(struct sockaddr_in);
                if((errorCode=clt_sock=accept(srv_sock,(struct sockaddr *)&clt,(socklen_t *)&c))<0){
                    printf("server %d accept error! error code:%d\n",i,errorCode);
                    exit(-5);
                }
                //服务端读取客户端发送的数据
                read_size=recv(clt_sock,buffer,MAX_LINE_SIZE,0);
                if(read_size==0){
                    printf("server %d:client disconnectd\n",i);
                    exit(-6);
                }
                else if(read_size==-1){
                    printf("server %d:recv failed\n",i);
                    exit(-7);
                }      
                //解析读取的行数据到小段结构体
                struct stuInfoLE stuInfoLETmp;
                struct stuInfoBE stuInfoBETmp;
                convAsciiLineBufferToStuInfoLE(buffer,&stuInfoLETmp);
                //将小段结构体转为大端结构体
                convStuInfoFromLEToBE(&stuInfoLETmp,&stuInfoBETmp);
                //服务端通过套接口发送转码后的数据
                send(clt_sock,(char *)&stuInfoBETmp,sizeof(stuInfoBETmp),0);
                close(clt_sock);            
                close(srv_sock);
                exit(0);
            }
        }
    }
    //插入延时确保各服务端创建成功
    usleep(50000);

    //客户端拆分每一行文本，通过套接口发送给各服务器
    int *sockPtr;
    sockPtr=(int *)malloc(processCount);
    struct sockaddr_in *srvPtr;
    srvPtr=(struct sockaddr_in *)malloc(processCount);
    for(int i=0;i<processCount;++i){
        sockPtr[i]=socket(AF_INET,SOCK_STREAM,0);
        srvPtr[i].sin_addr.s_addr=inet_addr("127.0.0.1");
        srvPtr[i].sin_family=AF_INET;
        srvPtr[i].sin_port=htons(portPtr[i]);
        int errorCode=0;
        if((errorCode=connect(sockPtr[i],(struct sockaddr *)&(srvPtr[i]),sizeof(srvPtr[i])))<0){
            printf("client connect to server %d failed! error code:%d\n",i,errorCode);
            return -1;
        }
        memset(buffer,'\0',MAX_LINE_SIZE);
        if(fgets(buffer,MAX_LINE_SIZE,fpAscii)!=NULL){
            send(sockPtr[i],buffer,MAX_LINE_SIZE,0);
        }
    }
    fclose(fpAscii);
    printf("\nread ascii file and send to servers success!\n\n");

    //客户端等待所有服务端退出
    for(int i=0;i<processCount;++i){
        int status;
        waitpid(childPidPtr[i],&status,WUNTRACED);     //阻塞式等待
        if(status==0){
            printf("server %d translate data success!\n",childPidPtr[i]);
        }
        else{
            printf("server %d translate data failed!\n",childPidPtr[i]);
            return -1;
        }
    }
    free(childPidPtr);

    //客户端读取各服务端转码后的文件转为一个大的bin文件
    FILE *fpBin=fopen(_binFileName,"w");
    if(fpBin==NULL){
        printf("create %s file failed!\n",_binFileName);
        return -1;
    }
    for(int i=0;i<processCount;++i){
        memset(buffer,'\0',MAX_LINE_SIZE);
        recv(sockPtr[i],buffer,MAX_LINE_SIZE,0);
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








































