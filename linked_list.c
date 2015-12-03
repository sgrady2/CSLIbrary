#include <stdio.h>
#include <stdlib.h>

struct NODE {
	int number;
	struct NODE *next;
};

int search_value(struct NODE *llist, int num);
void append_node(struct NODE *llist, int num);
void display_list(struct NODE *llist, int num);
void delete_node(struct NODE *llist, int num);

int main(void){

	int num = 0;
	int input =1;
	int retval =0;
	
	struct NODE *llist;
	llist = (struct NODE *) malloc(sizeof(struct NODE));
	

	llist->number =0;
	llist->next = NULL;

	while (input !=0){
		printf("\n Menu Selection\n");
		printf("0: Quit \n");
		printf("1: Insert \n");
		printf("2: Delete \n");
		printf("3: Search \n");
		printf("4: Display \n");
		scanf("%d", &input);


	switch(input){

	}
	
	}

}
