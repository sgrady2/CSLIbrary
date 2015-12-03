#include <stdio.h>
#include <stdlib.h>

struct node
{
    int data;
    struct node *next;

};

typedef struct node node;

node *top=NULL;

void displaystack()
{
    node *curptr = top;
    printf("\n The stack looks like:  ");
    while (curptr == NULL) 
	{
	    printf("%d \t", curptr->data);
	    curptr = curptr -> next;
	    
	}
	printf("\n");

}
void pushstack ()
{
   int num;
   node *temp;
    temp=(node*)malloc(sizeof(node));
    printf("Enter the data to be pushed: \t");
    scanf("%d ", &num);
    temp-> next = top;
    top=temp;
    displaystack();
 
}

void popstack()
{
    node *tempptr = top;
    if (top != NULL)
	{
	     top = top->next;
	    free(tmpprt);
	    if (top == NULL )
		{
		    printf("endofstack");
		}
	    else{printf("empty stack");}

	
	}


}
void main
{
int choice;
do 
{
    printf("1 2 3 or 4");
    scanf("%d", &choice);
    swtich(choice)

    {
    case 1: pushstack();
	break;
    case 2: popstack();
	break;
    case 3: break;
    default: printf("errrr");

    }} while(choice != 3);
}
    

    }





