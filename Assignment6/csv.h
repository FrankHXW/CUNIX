#ifndef _CSV_H
#define _CSV_H


#define MAX_ISSUE_SIZE 16
#define MAX_LINE_SIZE 128
#define MAX_ROWS_SIZE 128


struct stuInfoLE
{
    uint32_t Id;
    char Name[MAX_ISSUE_SIZE];
    char  Department[MAX_ISSUE_SIZE];
    uint32_t Age;
}__attribute__((packed,aligned(4)));

struct stuInfoBE
{
    uint32_t Id;
    char Name[MAX_ISSUE_SIZE];
    char Department[MAX_ISSUE_SIZE];
    uint32_t Age;
}__attribute__((packed,aligned(4)));

int convAsciiLineBufferToStuInfoLE(char *_buffer,struct stuInfoLE *_dst);

int convStuInfoFromLEToBE(struct stuInfoLE *_src,struct stuInfoBE *_dst);

int convStuInfoFromBEToLE(struct stuInfoBE *_src,struct stuInfoLE *_dst);

int countFileLines(FILE *_fp);

int readCSVAsciiAndSaveBin(char *_asciiFileName,char *_binFileName); 

int readCSVBinAndPrint(char *_binFileName);

int readCSVBinAndPrintByMMap(char *_binFileName);

#endif
