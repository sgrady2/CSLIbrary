#include <stdio.h>
int main()
{
    int m, n, c, d, matrix[10][10], transpose[10][10];
    printf("Enter the number of rows and columns: ");
    scanf("%d%d", &m, &n);
    printf("Enter the element of matrix: \n");
    
    for (c=0;c<m;c++)
	{
	    for (d=0;d<n;d++)
		{
		    scanf("%d",&matrix[c][d]);
		}
	}
    for (c=0;c<m;c++)
	{
	    for(d=0;d<n;d++)
		{
		    transpose[d][c] = matrix[c][d];
		}
	}
    printf("Transposed: ");
    for (c=0;c<m;c++){
        for (c=0;c<m;c++){
	    printf("%d\t",transpose[c][d]);
	}
	printf("\n");
    }

return 0;


}
