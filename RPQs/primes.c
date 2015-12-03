//Given a range a to b and a number k, find all k prime numbers between a to b with
//exactly k distinct prime factors


#include<stdio.h>
#include<stdlib.h>
int main(){
    int numOfInp;
    scanf("%d",&numOfInp);
    int a,b,k;  
    scanf("%d %d %d",&a,&b,&k);
    int *arr;
    arr = (int*)calloc(b+1,sizeof(int));

    int i=2,j=2,count=0; 
    //Count is the count of distic k prim factors for a particular number
    while(i<=b){
        if(arr[i]==0){
            for(j=i;j<=b;j=j+i){
                arr[j]++;
            }
        }
        if(i>=a && arr[i]==k)
            count++;
        i++;
    }
    printf("%d\n",count);
    free(arr);

    return 0;
}
