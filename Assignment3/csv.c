#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    //创建输出Bin文件
    FILE *fpBin=fopen(_binFileName,"w");
    if(fpBin==NULL){
        printf("create .csv.bin file failed!\n");
        return -1;
    }

    int linesCount=countFileLines(fpAscii);

    //设置fp回到stream头位置
    fseek(fpAscii,0,SEEK_SET);
    char buffer[MAX_LINE_SIZE];
    memset(buffer,'\0',MAX_LINE_SIZE);

    //读取第一行的表头并写入到Bin文件
    if(fgets(buffer,MAX_LINE_SIZE,fpAscii)!=NULL){
        //统计每行的字符个数
        int n=0;
        while(1){
            if(buffer[n++]=='\n')
                break;
        }
        fwrite(buffer,1,n,fpBin);
    }
    else{
        printf("get table name failed!\n");
        return -1;
    }
    printf("%s",buffer);


    //将每一行的数据读到buffer里去
    struct stuInfoLE stuInfoLETmp;
    struct stuInfoBE stuInfoBETmp;

    for(int l=2;l<=linesCount;++l){
        memset(buffer,'\0',MAX_LINE_SIZE);
        if(fgets(buffer,MAX_LINE_SIZE,fpAscii)!=NULL){
            //解析每一行的字符数据到自定义的结构体(小端)
            convAsciiLineBufferToStuInfoLE(buffer,&stuInfoLETmp);
            printf("%u,%s,%s,%u\n",stuInfoLETmp.Id,stuInfoLETmp.Name,stuInfoLETmp.Department,stuInfoLETmp.Age);    
        
            //转换小端结构体到大端结构体
            convStuInfoFromLEToBE(&stuInfoLETmp,&stuInfoBETmp); 
//            printf("%u,%s,%s,%u\n\n",stuInfoBETmp.Id,stuInfoBETmp.Name,stuInfoBETmp.Department,stuInfoBETmp.Age);    
          
            //将大端结构体以二进制写到输出文件流
            fwrite((char *)&stuInfoBETmp,sizeof(stuInfoBETmp),1,fpBin);
//            fwrite("\n",1,1,fpBin);
        } 
        else{
            printf("get %d line failed\n",l);
            return -1;
        }
    }
    
    printf("read ascii file and convert to bin file success!\n\n");

    fclose(fpAscii);
    fclose(fpBin);
    return 0;
}





int readCSVBinAndPrint(char *_binFileName)
{
    FILE *fpBin=fopen(_binFileName,"r");
    if(fpBin==NULL){
        printf("open .csv.bin file failed!\n");
        return -1;
    }
    //读取第一行的表头，以‘\n’结尾
    char buffer[MAX_LINE_SIZE];
    memset(buffer,'\0',MAX_LINE_SIZE);
    if(fgets(buffer,MAX_LINE_SIZE,fpBin)!=NULL){
        printf("%s",buffer);
    }
    else{
        printf("read bin file table name failed!\n");
    }

    //现在的文件指针位于表的第二行开头，加载二进制数据
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











































