#include<stdio.h>
void main()
{
    int c=getchar(); //getchar() returns the next input
    		     // character each time it is called
    while(c!=EOF)
    {
        putchar(c); //putchar() puts c on stdout
        c=getchar();  // grabs the next character
    }
}
