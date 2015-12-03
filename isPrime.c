#include <stdio.h>

int main()
{

int n, count = 0, i;

printf("Enter a number");
scanf("%d", &n);
for (i=0; i<n;i++)
	{
	if (n%i == 0)
	    count++;
	}
    if(count == 2)
        printf("%d is prime", n);
    else 
	printf("%d is not prime", n);
    

    return 0;
}
