/* Data types in C */

#include <stdio.h>

int main() {

	/* Numeric data types */
    int a = 4;      /* integer data type: char, short, int, long, long long */
    printf("The value of a is %d\n", a);

	float pi = 3.14;         /* floating point types: float, double and long double */
    printf("The value of pi is %f\n", pi);

	double z = 3.0;
	printf("z is a double = %lf \n", z); //the format specifier must meet the datatype

	/* Expressions */

    int b, d;
    a = b = d = 10;                     /* in C assignments are expressions */

	int celsius, fahr;    				
    fahr = 90;                          /* assignments */
    celsius = 5 * (fahr - 32) / 9;      /* integer arithmetic expression */
    printf("From %d Fahrenheit to %d Celsius degrees\n", fahr, celsius);

    float c, f;
    f = 90.0;
    c = 5.0 * (f - 32.0) / 9.0;         /* float arithmetic expression */
    printf("From %f Farenheit to %f Celsius degrees\n", f, c);

	int x= 10, y = 3;
    double z1 = x / y;       /* returns an int value */
    printf("The value of z1 is %lf\n", z1);

    z1 = (double) x / y;        /* typecasting x to a double */
    printf("The value of z1 is %lf\n", z1);

    a = (4>5? 2 : 3);                   /* conditional assignment */
    printf("The value of a is %d\n", a);    

	/* Character data types */

    char v = 'a';      /* character data type = 8 bit integer */
    printf("The value of v is %c\n", v);

    char e = '8', g = '0';   /* ASCII values of 8 and 0 are 56 and 48 respectively */
    int p = e - g;       
    printf("The value of p is %d\n", p); 

    return 0;
}
 