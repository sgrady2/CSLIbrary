#include <stdio.h>
#include <math.h>
#include <iostream>
using namespace std;

int main()
{
	int n;
    printf("Enter a number for primes less than that number");
    scanf("%d", &n);

    for (int i=2;i<n;i++)
	{
	    for (int j = 2; j*j<=i;j++)
		{
		    if(i%j == 0)
			break;
		    else if (j+1>sqrt(i)){
			cout <<i<< " ";	
			}
		}
	}
	return 0;

}
