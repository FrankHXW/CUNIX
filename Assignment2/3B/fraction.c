#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "fraction.h"

fraction add(fraction a,fraction b)
{	
	fraction c={0,0};
	if((a.denominator==0)||(b.denominator==0)){
		printf("Nan: denominator can not be 0!\n");
		return c;
	}
    c.numerator=a.numerator*b.denominator+b.numerator*a.denominator;
	c.denominator=a.denominator*b.denominator;
	c=simpling(c);
	return c;
}

fraction substract(fraction a,fraction b)
{
	fraction c={0,0};
	if((a.denominator==0)||(b.denominator==0)){
		printf("Nan: denominator can not be 0!\n");
		return c;
	}
    c.numerator=a.numerator*b.denominator-b.numerator*a.denominator;
	c.denominator=a.denominator*b.denominator;
	c=simpling(c);
	return c;
}


fraction multiple(fraction a,fraction b)
{
	fraction c={0,0};
	if((a.denominator==0)||(b.denominator==0)){
		printf("Nan: denominator can not be 0!\n");
		return c;
	}
    c.numerator=a.numerator*b.numerator;
	c.denominator=a.denominator*b.denominator;
	c=simpling(c);
	return c;
}


fraction divide(fraction a,fraction b)
{
	fraction c={0,0};
	if((a.denominator==0)||(b.denominator==0)){
		printf("Nan: denominator can not be 0!\n");
		return c;
	}
    c.numerator=a.numerator*b.denominator;
	c.denominator=a.denominator*b.numerator;
	c=simpling(c);
	return c;
}


fraction simpling(fraction a)
{
	int z=(abs(a.numerator)>abs(a.denominator)?abs(a.denominator):abs(a.numerator));
	while(z>1){
		if((a.numerator%z==0)&&(a.denominator%z==0)){
			a.numerator=a.numerator/z;
			a.denominator=a.denominator/z;
			z=(abs(a.numerator)>abs(a.denominator)?abs(a.denominator):abs(a.numerator));
		}
		--z;
	}	
	return a;
}


double convertToDouble(fraction a)
{
	if(a.denominator==0){
		printf("Nan: denominator can not be 0!\n");
		return -1;
	}
	return (((double)a.numerator)/((double)a.denominator));
}

































