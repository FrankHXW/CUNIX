#include "stdio.h"
#include "fraction.h"

int main(void)
{
    while(1){ 	
		printf("please input fraction like this:\n a/b op c/d \n");
		fraction a,b,c;
		char op='0';
		scanf("%d/%d %c %d/%d",&a.numerator,&a.denominator,&op,&b.numerator,&b.denominator);
		if((a.denominator==0)||(b.denominator==0)){
			printf("Nan: denominator can not be 0!\n");
			continue;
		}
		switch(op){
			case '+':
				c=add(a,b);
				break;
			case '-':
				c=substract(a,b);
				break;
			case '*':
				c=multiple(a,b);
				break;
			case '/':
				c=divide(a,b);
				break;	
			default:
				printf("invalid operation,please input '+' '-' '*' '/'\n");
				break;				
		}
		double f=convertToDouble(c);
		printf("%-5d       %-5d      %-5d\n",a.numerator,b.numerator,c.numerator);
		printf("—————  %c  —————  =  —————  =%-8f\n",op,f);
		printf("%-5d       %-5d      %-5d\n",a.denominator,b.denominator,c.denominator);
	}
	return 0;
}
