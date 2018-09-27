#include "vm_stock.h"

void addNode( VmSystem *system, Node *current)
{
	Node *temp = malloc(sizeof(Node));
	Boolean check = FALSE;
	
	/*checking if the head is empty */
	if(system->itemList->head != NULL)
	{
		/*if its not, runs through nodes and attachs to end*/
		temp = system->itemList->head;
		while(!check)
		{
			/*if end is found, attach*/
			if(temp->next == NULL)
			{
				temp->next = current;
				check = TRUE;
				break;
			}
			else
			{
				temp = temp->next;
			}
		}
	}
	else
	{
		system->itemList->head = current;
		free(temp);
	}
	system->itemList->size++;
 }
 
void addStock(Stock *stockFile, char *buffer)
 {
   char *id;
   char *name;
   char *desc;
   char *price;
   char *dollars;
   char *cents;
   char *onHand;
   unsigned dollarsI;
   unsigned centsI;
   unsigned onHandI;

   /*big line of code grabs information from buffer,
   translates the strings to unsigned, and places them 
   into the provided stockFile */
   id = strtok(buffer, STOCK_DELIM);
   name = strtok(NULL, STOCK_DELIM);
   desc = strtok(NULL, STOCK_DELIM);
   price = strtok(NULL, STOCK_DELIM);
   onHand = strtok(NULL, STOCK_DELIM);
   
   dollars = strtok(price, ".");
   cents = strtok(NULL, ".");
   
   dollarsI = (unsigned) strtol(dollars, NULL, 10);
   centsI = (unsigned) strtol(cents, NULL, 10);
   onHandI = (unsigned) strtol(onHand, NULL, 10);

   stockFile->price.dollars = dollarsI;
   stockFile->price.cents = centsI;
   stockFile->onHand = onHandI;
   strcpy(stockFile->id, id);
   strcpy(stockFile->name, name);
   strcpy(stockFile->desc, desc);

 }
 
 void organiseStock(VmSystem * system)
 {
	Stock *holder;
	Node *current;
	int x = 0;
	int check;
	int counter = system->itemList->size;
	
	/* Rerunning to ensure even the end node can be brought to the front*/
	for (x = 0; x < counter; x++) 
	{
      current = system->itemList->head;
      while (current->next != NULL)
		{
			check = strcmp(current->next->data->name, current->next->data->name);
			/*if the next string has a larger name!!*/
			if (check < 0) 
			{
				holder = current->data;
				current->data = current->next->data;
				current->next->data = holder;
			}
			current = current->next;
      }
   }
 }
 
 void organiseStockByID(VmSystem * system)
 {
	Node *current;
	Node *future;
	Stock *stockP;
	int compare = 0; 
	int count;
	int sys = system->itemList->size;
	
	/* same as above for loop :) */
	for(count = 0; count < sys; count++)
	{		
		current = system->itemList->head;
		while(current->next != NULL)
		{
			future = current->next; 
			compare = strcmp(current->data->id, future->data->id);
			/* If id is larger, swaps */
			if(compare > 0)
			{
				stockP = current->data;
				current->data = future->data;
				future->data = stockP;
			}
			current = future;
		}
	}
 }
 
 /* Just creates a new list and returns it to function */
 List *createList()
 {
	List *myList = malloc(sizeof(List));
	myList->head = NULL;
	myList->size = 0;
	return myList; 
 }