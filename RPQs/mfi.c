#include <stdio.h>
//This code is for solving the problem:
//Find the most frequent integer in an array (solution should print 2)
//FIX!: optimize this so it doesnt do uneccesary comparisons

int main()
{
	int list[] = {2, 2, 2, 2, 4};
	int i,j;
	int arrsize;
	//if 
	if (list[0] != 0)
		arrsize = sizeof(list)/sizeof(list[0]);
	else 
		{printf("ARRAY EMPTY");}

	for (i =0; i<arrsize;i++)
	    {
		for (j = i+1; j<arrsize;j++)
			{
 				if (list[i] == list[j])
				printf("\nDuplicate %d\n", list[i]);
			}
		//print debug stmt printf("\n%d\n" , list[i]);
	    }



}
