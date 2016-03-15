#ifndef _FRACTION_H_
#define _FRACTION_H_

typedef struct fraction{
	int numerator;
	int denominator;
}fraction;

fraction add(fraction a,fraction b);
fraction substract(fraction a,fraction b);
fraction multiple(fraction a,fraction b);
fraction divide(fraction a,fraction c);
fraction simpling(fraction a);
double convertToDouble(fraction a);

#endif
