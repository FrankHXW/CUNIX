#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/wait.h>
#include "csv.h"

int forkExample(void){
    pid_t child1=-2,child2=-2,child=-2;
   int testVariable=-2;
   //先创建子进程1
   child1=fork();
   //对子进程1的处理
   if(child1==-1){
      printf("fork child1 error\n");
      exit(1);
   }
   //在子进程里调用execlp()函数
   else if(child1==0){
    testVariable=11;
     printf("this is child1 and execute 'ls -l' %d %d %d %d\n",child1,child2,child,testVariable);
     execlp("ls","ls","-l",NULL);
   }
   //在父进程里创建子进程2
   else{
     child2=fork();
     if(child2==-1){
        printf("fork child2 error\n");
        exit(1);
     }
     //在子进程2里暂停5s
     else if(child2==0){
        testVariable=22;
        printf("this is child2,sleep for 5s %d %d %d %d\n",child1,child2,child,testVariable);
        sleep(5);
        printf("this is child2,sleep for 5s finished\n");
        exit(-1);
     }
     printf("this is father progress %d %d %d\n",child1,child2,child);
     child=waitpid(child1,NULL,WCONTINUED);
     if(child==child1){
        printf("this is father progress,get child1 exit code:%d \n",child);
     }
     else{
        printf("wait for child1 exit error occured:\n");
     }
     do{
        child=waitpid(child2,NULL,WNOHANG);
        if(child==0){
            printf("this is father progress,the child2 has not exited!\n");
        }
        sleep(1);
     }while(child==0);
     if(child==child2){
        printf("this is father progress,get child2 exit code:%d\n",child);
     }
     else{
        printf("wait for child2 exit error occured: %d\n",child);
     }
   }
   return 0;
}
