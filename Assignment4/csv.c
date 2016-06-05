#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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

    //将每一行文本转为单独的小文件
    for(int l=0;l<linesCount;++l){
        memset(buffer,'\0',MAX_LINE_SIZE);
        if(fgets(buffer,MAX_LINE_SIZE,fpAscii)!=NULL){
            char pieceFileName[100];
            memset(pieceFileName,'\0',100);
            snprintf(pieceFileName,100,"%d.csv",l);
            FILE *fpAsciiTemp=fopen(pieceFileName,"w");
            if(fpAsciiTemp==NULL){
                printf("create %d.csv file failed\n",l);
                return -1;
            }
            fputs(buffer,fpAsciiTemp);
            fclose(fpAsciiTemp);
        }
    }
    fclose(fpAscii);
    printf("\nread ascii file and split into small files success!\n\n");

    //开启多个进程分别进行转码
    int processCount=linesCount;
    pid_t*  childPidPtr;                 //动态申请进程数
    childPidPtr=(pid_t *)malloc(processCount);
    for(int i=0;i<processCount;++i){
        childPidPtr[i]=fork();
        if(childPidPtr[i]==-1){
            printf("create child process %d failed!\n",i);
            break;                      //确保只有父进程在创建子进程
        }
        else{
           //每个子进程读取对应与i编号的小文件并转成bin文件
            if(childPidPtr[i]==0){
                //创建输入输出的两个文件描述符
                char pieceFileName[100];
                memset(pieceFileName,'\0',100);
                snprintf(pieceFileName,100,"%d.csv",i);
                FILE *fpAsciiTemp=fopen(pieceFileName,"r");
                if(fpAsciiTemp==NULL){
                    printf("read %d.csv file failed!\n",i);
                    exit(-1);
                }
                memset(pieceFileName,'\0',100);
                snprintf(pieceFileName,100,"%d.csv.bin",i);
                FILE *fpBinTemp=fopen(pieceFileName,"w");
                if(fpBinTemp==NULL){
                    printf("create %d.csv.bin file failed!\n",i);
                    exit(-2);
                }
                //读取小文件并转码
                struct stuInfoLE stuInfoLETmp;
                struct stuInfoBE stuInfoBETmp;
                memset(buffer,'\0',MAX_LINE_SIZE);
                if(fgets(buffer,MAX_LINE_SIZE,fpAsciiTemp)!=NULL){
                    //解析读取的行数据到小段结构体
                    convAsciiLineBufferToStuInfoLE(buffer,&stuInfoLETmp);
                    //将小段结构体转为大端结构体
                    convStuInfoFromLEToBE(&stuInfoLETmp,&stuInfoBETmp);
                    //保存为单个的小bin文件
                    fwrite((char *)&stuInfoBETmp,sizeof(stuInfoBETmp),1,fpBinTemp);
                    fclose(fpAsciiTemp);
                    fclose(fpBinTemp);
                    exit(0);
                }
                exit(-3);                //子进程完成后退出，这句很重要
            }
            else{ 
                printf("create child process %d success!\n",childPidPtr[i]);  
            }
        }
    }

    //父进程等待所有子进程退出,子进程的代码不会运行到此处
    for(int i=0;i<processCount;++i){
        int status;
        waitpid(childPidPtr[i],&status,WUNTRACED);     //阻塞式等待
        printf("child process %d exit code：%d\n",childPidPtr[i],status);
    }
    free(childPidPtr);
    //父进程读取各个转码后的小bin文件转为一个大的bin文件
    FILE *fpBin=fopen(_binFileName,"w");
    if(fpBin==NULL){
        printf("create %s file failed!\n",_binFileName);
        return -1;
    }
    char pieceFileName[100];
    for(int i=0;i<linesCount;++i){
        memset(pieceFileName,'\0',100);
        snprintf(pieceFileName,100,"%d.csv.bin",i);
        FILE *fpBinTemp=fopen(pieceFileName,"r");
        if(fpBinTemp==NULL){
            printf("create %d.csv.bin file failed!\n",i);
            exit(-2);
        }
        memset(buffer,'\0',MAX_LINE_SIZE);
        if(fgets(buffer,MAX_LINE_SIZE,fpBinTemp)!=NULL){
            fwrite(buffer,sizeof(struct stuInfoBE),1,fpBin);
        }
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











































