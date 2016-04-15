#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "csv.h"

int main(void)
{
    char *asciiFileName="./test.csv";    
    char binFileName[20]; 
    memset(binFileName,'\0',20);
    strcat(binFileName,asciiFileName);
    strcat(binFileName,".bin");

    readCSVAsciiAndSaveBin(asciiFileName,binFileName);

    readCSVBinAndPrint(binFileName);

    return 0;
}
