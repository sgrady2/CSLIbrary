#include <stdio.h>
//C program to print out the number in binary
//NOTE: This assumes the size of the interger in 32 bit signed
int main()
{
	int in, size, shift;
	printf("Enter a number to convert to binary");
	scanf("%d", &in);
	printf("%d in binar is:  \n", in);
	for (size=32;size>=0;size--)
	    {
		shift = in >> size;
		if (shift&1)
		    printf("1");
		else
		    printf("0");
	    }
	printf("\n");
	return 0;

}
