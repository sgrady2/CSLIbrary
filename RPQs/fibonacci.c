#include <stdio.h>

int main()
{
    int num, i, j;
    printf("enter a number to continue to");
    scanf("%d" , &num);
    int lower, higher, next;
    lower = 0;
    higher = 1;
    printf("%d", lower);
    printf("%d", higher);
    for (i=0;i<num;i++)
	{
	
	next = lower + higher;
	lower = higher;
	higher = next;
	printf("%d\n", next);
	}


}
