#include <stdio.h>
int main()
{
int n, count = 0;
printf("Enter the number to check");
scanf("%d", &n);
for (i=1; i<=n;i++)
	{
	    if (n%i==0)
		count++;
	}
if (count = 2)
printf("IS Prime", n);
else
printf("%d isn't prime", n);
return 0;

}
