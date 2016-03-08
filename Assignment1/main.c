#include "stdio.h"


int cal3Weight(char *weight,int i0,int i1,int i2)
{
    int tmp=weight[i0];
    tmp+=weight[i1];
    tmp+=weight[i2];
    return tmp;	
}

int cal4Weight(char *weight,int i0,int i1,int i2,int i3)
{
    int tmp=weight[i0];
    tmp+=weight[i1];
    tmp+=weight[i2];
    tmp+=weight[i3];
    return tmp;	
}



int main(int argc,char *argv[])
{
  if(argc!=1){
	printf("invalid input argument!\n");
  }
  char weight[12]=argv[0];
  int w0123=cal4Weight(weight,0,1,2,3);
  int w4567=cal4Weight(weight,4,5,6,7);
  int w891011=cal4Weight(weight,8,9,10,11);
  if(w0123=w4567){     

  }
  else if(w0123>w4567){

  }
  else{

  }
  
  
  return 0;
}
