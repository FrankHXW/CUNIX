#include "stdio.h"
#include "malloc.h"
#include "stdbool.h"

int cal3Weights(int *weight, int i0,int i1,int i2)
{
	int tmp = weight[i0];
	tmp += weight[i1];
	tmp += weight[i2];
	return tmp;
}


int cal4Weights(int *weight, int i0, int i1, int i2,int i3)
{
	int tmp = weight[i0];
	tmp += weight[i1];
	tmp += weight[i2];
	tmp += weight[i3];
	return tmp;
}

int readInputWeights(int *weight) 
{
	printf("please input 12 weights,1 is lighter,2 is normal,3 is heavier,like 2 2 1 2...:\n");
	for (int i = 0; i <12; ++i) {
		scanf("%d", (weight + i));
		if ((*(weight + i) != 1)&& (*(weight + i) != 2) && (*(weight + i) != 3)) {
			printf("invalid input @%d\n", i);
		}
	}
	return 0;
}

//must known is lighter or heavier firstly
int judge3Weight(int *weight, int i0, int i1, int i2, bool heavierFlag)
{
	if (*(weight + i0) == *(weight + i1)) {
		if (heavierFlag) {
			printf("%d is heavier!\n", i2+1);
		}
		else {
			printf("%d is lighter!\n", i2+1);
		}
		return 0;
	}
	else if (*(weight + i0) > *(weight + i1)) {
		if (heavierFlag) {
			printf("%d is heavier!\n", i0+1);
		}
		else {
			printf("%d is lighter!\n", i1+1);
		}
		return 0;
	}
	else {
		if (heavierFlag) {
			printf("%d is heavier!\n", i1+1);
		}
		else {
			printf("%d is lighter!\n", i0+1);
		}
		return 0;	
	}
}

int judge(int *weight)
{
	int w0123 = cal4Weights(weight, 0, 1, 2, 3);
	int w4567 = cal4Weights(weight, 4, 5, 6, 7);
	if (w0123 == w4567) {					//1 
		int w012 = cal3Weights(weight, 0, 1, 2);
		int w8910 = cal3Weights(weight, 8, 9, 10);
		if (w012 == w8910) {				//2
			int w0 = *(weight);
			int w11 = *(weight + 11);
			if (w0 > w11) {				//3
				printf("12 is lighter!\n");
				return 0;
			}
			else {
				printf("12 is heavier!\n");
				return 0;
			}
		}
		else if (w012 < w8910) {			//2
			judge3Weight(weight, 8, 9, 10, true);	//3
			return 0;
		}
		else {
			judge3Weight(weight, 8, 9, 10, false);
			return 0;
		}
	}
	else if (w0123 > w4567) {				//1
		int w0124=cal4Weights(weight,0,1,2,4);
		int w38910=cal4Weights(weight,3,8,9,10);
		if(w0124==w38910){				//2
			judge3Weight(weight,5,6,7,false);	//3
			return 0;
		}
		else if(w0124>w38910){				//2
			judge3Weight(weight,0,1,2,true);	//3
			return 0;
		}
		else{						//2
			if(*(weight+3)>*(weight+8)){		//3
				printf("4 is heavier!\n");
				return 0;
			}
			else{
				printf("5 is lighter!\n");
				return 0;
			}
		}
		

	}
	else {
		int w0124=cal4Weights(weight,0,1,2,4);
		int w38910=cal4Weights(weight,3,8,9,10);
		if(w0124==w38910){				//2
			judge3Weight(weight,5,6,7,true);	//3
			return 0;
		}
		else if(w0124<w38910){				//2
			judge3Weight(weight,0,1,2,false);	//3
			return 0;
		}
		else{						//2
			if(*(weight+3)<*(weight+8)){		//3
				printf("4 is lighter!\n");
				return 0;
			}
			else{
				printf("5 is heavier!\n");
				return 0;
			}
		}
	}
}

int main(void)
{
	int *weight = (int *)malloc(12);
	while (1) {
		readInputWeights(weight);
		judge(weight);
	}
	free(weight);
	return 0;
}




