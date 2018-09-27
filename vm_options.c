#include "vm_options.h"
#include "stdio.h"
#include "string.h"
#define MAX_DATA 256
#define MAX_PRICE_L 4
#define wizardNo 3
#define warlockNo 29

Boolean systemInit(VmSystem * system)
{
	/* This function creates a list, adds it to 
	the system, and sets the file headers to null */
	system->itemList = malloc(sizeof(List));
	if(system->itemList == NULL)
		perror("ITEM LIST FAILED TO ALLOCATE");
	
	system->stockFileName = NULL;
	system->coinFileName = NULL;
    return FALSE;
}

void systemFree(VmSystem * system)
{ 
	/*this function runs a loop from the head, and frees each node, 
	it then frees the itemlist itself and then finally the system */
   Node *current;
   Node *previous;
   current = system->itemList->head;
   while (current != NULL) 
   {
      previous = current;
	  current = current->next;
      free(previous->data);
      free(previous);
   }
   free(system->itemList);
   free(system);
}


Boolean loadData(
    VmSystem * system, const char * stockFileName, const char * coinsFileName)
{
	/* loads the file names into system */
	system->stockFileName = stockFileName;
	system->coinFileName = coinsFileName;
	
	return TRUE;
}


Boolean loadStock(VmSystem * system, const char * fileName)
{
	FILE *fp;
	char buffer[MAX_DATA];
	Node *current;
	Stock *stockFile;
	
	fp = fopen(fileName, "r");
	if(fp == NULL)
		printf("FILE POINTER EQUALS NULL");
	
	/* this loop runs through the buffer, adding the 
	information to the stock and adding it to a node, before
	adding the node to the end of the list */
	while(fgets(buffer, sizeof(buffer), fp) != NULL)
	{
			current = malloc(sizeof(Node));
			if(current == NULL)
			{
				printf(COLOR_YELLOW"\n NODE FAILED TO MALLOC"COLOR_RESET);
				free(current);
				exit(0);
			}
			current->next = NULL;
			stockFile = malloc(sizeof(Stock));
			if(stockFile == NULL)
			{
				printf(COLOR_YELLOW"\n NODE FAILED TO MALLOC"COLOR_RESET);
				free(stockFile);
				exit(0);
			}
			addStock(stockFile, buffer);
			current->data = stockFile;
			addNode(system, current);
			current = current->next;
	}
	fclose(fp);
	return FALSE;
}


Boolean loadCoins(VmSystem * system, const char * fileName)
{
    return FALSE;
}


Boolean saveStock(VmSystem * system)
{
    return FALSE;
}


Boolean saveCoins(VmSystem * system)
{
    return FALSE;
}


void displayItems(VmSystem * system)
{ 
	Node *current;
	int largest = 0;
	int tempI = 0;
	int hold = 0;
	int counter = 0;
	
	current = system->itemList->head;
	largest = strlen(current->data->name);
	
	printf("\nItems Menu\n\n");
	printf("ID     |");
	printf(" Name");

	/* runs through all nodes and finds largest name for 
		indentation  and the "-"*/
	while(current->next != NULL)
	{
		tempI = strlen(current->next->data->name);
		if(tempI > largest)
			largest = tempI;
		current = current->next;
	}
	
	hold = largest;

	largest -= wizardNo;
	
	/*prints the indentation of largest name */
	while(largest > 0)
	{
		printf(" ");
		largest--;
	}
	printf("|");
	printf(" Available |");
	printf(" Price\n");
	
	largest = hold;
	largest += warlockNo;
	/*prints the "-"*/
	while(largest > 0)
	{
		printf("-");
		largest--;
	}
	printf("\n");
	current = system->itemList->head;
	/* prints each item to the board, with indentation and 
	the items information */
	while(current != NULL)
	{
		counter = hold;
		printf("%s  |", current->data->id);
		printf(" %s ", current->data->name);
		counter -= strlen(current->data->name);
		while(counter > 0)
		{
			printf(" ");
			counter--;
		}
		printf("| %d        |", current->data->onHand);
		printf(" $ %d.%02d\n", current->data->price.dollars, current->data->price.cents);
		current = current->next;
	}
	printf("\n");
}


void purchaseItem(VmSystem * system)
{ 
	Node *current;
	Node *nextN;
	Node *picked;
	Boolean buffer = FALSE;
	Boolean check = FALSE;
	Boolean valid = FALSE;
	char input[6];
	int owed;
	int dollars;
	int cents;
	int deepCents;
	int answer;
	int entered;
	int xX;

	printf("\nPurchase Item\n");
	printf("---------------");
	printf("\nPlease enter the ID of the item you wish to purchase:");
	
	/* Captures the ID entered by user and runs checks to ensure its 
	valid */
	while(!valid)
	{
		fgets(input, sizeof(input), stdin);
		/* System to check if the buffer has overflowed! */		
		for(xX = 0; xX < 6; xX++)
		{
			if(input[xX] == '\n')
			buffer = TRUE;
		}
		/* checking if buffer has overflowed */
		if(!buffer)
			readRestOfLine();
			
		/*checking if input is digit */
		if(isdigit(input[4]))
			valid = TRUE;
		/* if the user hits enter, this method returns false and the above methods will make it end the game ect */
		else if(input[0] == '\n')
		{
			printf(COLOR_YELLOW"Returning to menu!\n"COLOR_RESET);
			return;
		}
		else
		{
			printf("Error, please enter a valid ID!");
			printf("\nPlease enter the ID of the item you wish to purchase:");
		}
	}
	valid = FALSE;
	current = system->itemList->head;
	buffer = FALSE;
	
	/*attempts to find the item, and checks its stock levels */
	while(!valid)
	{
		if(strcmp(current->data->id, input) == 0)
		{
			if(current->data->onHand == 0)
			{
				printf(COLOR_YELLOW"\n Sorry! It seems like this item is out of stock, please try another item"COLOR_RESET);
				return;
			}
			else
			{
				picked = current;
				valid = TRUE;
			}
		}
		
		nextN = current->next;
		current = nextN;
	}
	
	/* prints all the nessicary dialoge, and begins money input sequence */
	printf("\nYou have selected %s, %s. ",picked->data->name,picked->data->desc);
	printf("This will cost you $%d.%d!\n", picked->data->price.dollars,picked->data->price.cents);
	printf("Please hand over the amount owed - type in the value of each note/coin in cents");
	printf("\n~Note~ Press enter on a new and empty line to cancel this purchase!");
	
	entered = picked->data->price.dollars;
	owed = entered*100;
	entered = picked->data->price.cents;
	owed += entered;
	
	valid = FALSE;
	
	/*input loop to prompt user to enter cash, also applies checks on
	each input */
	while(!check)
	{
		if(owed <= 0)
		{
			dollars = owed / 100;
			cents = (owed % 100) / 10;
			deepCents = (owed % 100) % 10;
			dollars = abs(dollars);
			cents = abs(cents);
			deepCents = abs(deepCents);
			
			printf("\nThank you. Here is your %s,", picked->data->name);
			printf(" and your change, $%d.%d%d\n", dollars, cents, deepCents);
			picked->data->onHand--;
			check = TRUE;
			return;
		}
		else if(owed > 0)
		{
			dollars = owed / 100;
			cents = (owed % 100) / 10;
			deepCents = (owed % 100) % 10;
			printf("\nYou still owe $%d.%d%d: ", dollars, cents,deepCents);	
			while(!valid)
			{
				fgets(input, sizeof(input), stdin);
				/* System to check if the buffer has overflowed! */		
				for(xX = 0; xX < 6; xX++)
				{
					if(input[xX] == '\n')
					buffer = TRUE;
				}
				/* checking if buffer has overflowed */
				if(!buffer)
					readRestOfLine();
			
				sscanf(input, "%d", &answer);
				/*checking if input is digit */
				if(answer==100||answer==200||answer==500||answer==1000)
					valid = TRUE;
				else if(answer == 5||answer == 10||answer==20||answer == 50)
					valid = TRUE;
				else if(input[0] == '\n')
				{
					printf(COLOR_YELLOW"Returning to menu!\n"COLOR_RESET);
					return;
				}
				else{
					printf("Error, please enter a valid currency!");
					printf("\nYou still owe $%d.%d%d: ", dollars, cents,deepCents);
				}
			}
			valid = FALSE;
			owed -= answer;
		}
	
	}
}


void saveAndExit(VmSystem * system)
{ 
	FILE *fp;
	const char *temp;
	Node *current;
	Stock *info;
	Price money;
	char *id;
	char *name;
	char *desc;
	unsigned onHand;
	unsigned dollars;
	unsigned cents;
	
	organiseStockByID(system);
	
	temp = system->stockFileName;
	fp = fopen(temp, "w");
	current = system->itemList->head;
	
	/* Loop to print all data from the nodes to the file */
	while(current != NULL)
	{
		info = current->data;
		money = current->data->price;
		
		id = info->id;
		name = info->name;
		desc = info->desc;
		onHand = info->onHand;
		dollars = money.dollars;
		cents = money.cents;
		
		fprintf(fp, "%s|%s|%s|%d.%02d|%d", id, name,desc,dollars, cents,onHand);
		fprintf(fp, "\n");
		current = current->next;
	}
	systemFree(system);
	fclose(fp);
	printf(COLOR_YELLOW "System state has been saved! Have a lovely day.\n" COLOR_RESET);
	exit(0);
}


void addItem(VmSystem * system)
{ 
	Boolean buffer = FALSE;
	Boolean check = FALSE;
	int xX; 
	Stock *newStock = malloc(sizeof(Stock));
	Node *newNode = malloc(sizeof(Node));
	unsigned count = system->itemList->size + 1;
	char id[ID_LEN + EXTRA_SPACES];
	char name[NAME_LEN + EXTRA_SPACES];
	char desc[DESC_LEN + EXTRA_SPACES];
	char price[MAX_PRICE_L + EXTRA_SPACES];
	unsigned dollars;
	unsigned cents;
	
	if(newNode == NULL)
	{
		printf(COLOR_YELLOW"\n NODE FAILED TO MALLOC"COLOR_RESET);
		free(newNode);
		exit(0);
	}
	if(newStock == NULL)
	{
		printf(COLOR_YELLOW"\n NODE FAILED TO MALLOC"COLOR_RESET);
		free(newStock);
		exit(0);
	}
	newNode->next = NULL; 

	/*Generating new ID*/
	printf("This new meal item will have the Item ID of I%04d", count);
	sprintf(id, "I%04d", count); 
	
	/* loop and checks for name of item from user*/
	while(!check)
	{
		printf("\nEnter the item name: ");
		fgets(name, sizeof(name), stdin);
		/* System to check if the buffer has overflowed! */		
		for(xX = 0; xX < sizeof(id); xX++){
			if(name[xX] == '\n')
			{
				buffer = TRUE;
				break;
			}
		}
		/* checking if buffer has overflowed */
		if(!buffer)
			readRestOfLine();
		if(name[0] == '\n')
		{
			printf(COLOR_YELLOW"Returning to menu!\n"COLOR_RESET);
			free(newStock);
			free(newNode);
			return;
		}	
		else
		{
			name[strlen(name) - 1] = '\0'; 
			check = TRUE;
			break;
		}
	}
	check = FALSE;
	buffer = FALSE;
	
	/* Loop for desc input and checks */
	while(!check)
	{
		printf("\nEnter the item description: ");
		fgets(desc, sizeof(desc), stdin);
		/* System to check if the buffer has overflowed! */		
		for(xX = 0; xX < sizeof(id); xX++){
			if(desc[xX] == '\n')
			{
				buffer = TRUE;
				break;
			}
		}
		/* checking if buffer has overflowed */
		if(!buffer)
			readRestOfLine();
		if(desc[0] == '\n')
		{
			printf(COLOR_YELLOW"Returning to menu!\n"COLOR_RESET);
			free(newStock);
			free(newNode);
			return;
		}	
		else
		{
			desc[strlen(desc) - 1] = '\0'; 
			check = TRUE;
			break;
		}
	}
	check = FALSE;
	buffer = FALSE;
	
	/* loop for price input and checks */
	while(!check)
	{
		printf("\nEnter the price for this item: ");
		fgets(price, sizeof(price), stdin);
		/* System to check if the buffer has overflowed! */		
		for(xX = 0; xX < sizeof(id); xX++){
			if(price[xX] == '\n')
			{
				buffer = TRUE;
				break;
			}
		}
		/* checking if buffer has overflowed */
		if(!buffer)
			readRestOfLine();
		/* if the user hits enter, this method returns false and the above methods will make it end the game ect */
		if(price[0] == '\n')
		{
			printf(COLOR_YELLOW"Returning to menu!\n"COLOR_RESET);
			free(newStock);
			free(newNode);
			return;
		}
		else
		{
			dollars =(unsigned)strtol((strtok(price, ".")), NULL, 10);
			cents =(unsigned)strtol((strtok(NULL, "\n")), NULL, 10);
			check = TRUE;
			break;
		}
	}
	
	/* Placing all user information into a stock file */
	strcpy(newStock->id, id);
	strcpy(newStock->name, name);
	strcpy(newStock->desc, desc);
	newStock->onHand = DEFAULT_STOCK_LEVEL;
	newStock->price.dollars = dollars;
	newStock->price.cents = cents;
	
	/* attaching the stock file to a node, adding the node to the list, and then reorganising 
	the list */
	newNode->data = newStock;
	addNode(system, newNode);
	organiseStockByID(system);
	system->itemList->size++;
	printf(COLOR_YELLOW"\nThis item \"%s - %s\" has now been added to the system.\n"COLOR_RESET,
		name,desc);
}


void removeItem(VmSystem * system)
{ 
	Node *delete = NULL;
	Node *current;
	Node *last;
	char input[ID_LEN + EXTRA_SPACES];
	int xX;
	int checker;
	int counter = 0;
	Boolean check = FALSE;
	Boolean buffer = FALSE;
	
	current = system->itemList->head;
	/* Loop to grab ID input and chekcs */
	while(!check)
	{
		printf("Please enter the ID of the item to remove from the menu: ");
		fgets(input, sizeof(input), stdin);
		for(xX = 0; xX < sizeof(input); xX++){
			if(input[xX] == '\n')
			{
				buffer = TRUE;
				break;
			}
		}
		/* checking if buffer has overflowed */
		if(!buffer)
			readRestOfLine();
		/* if the user hits enter, this method returns false and the above methods will make it end the game ect */
		if(input[0] == '\n')
		{
			printf(COLOR_YELLOW"Returning to menu!\n"COLOR_RESET);
			return;
		}
		else
		{
			/*Finding the node, and deleting, this contains ifs for different circumstances, such as
			if the node is located in the itemList->head */
			input[strlen(input) - 1] = '\0';
			while(current != NULL)
			{	
				checker = strcmp(input, current->data->id);
				if(checker == 0)
				{
					check = TRUE;
					delete = current;
					if(counter == 0)
						system->itemList->head = current->next;
					else
						last->next = delete->next; 
					
					check = TRUE;
					break;
				}
				
				if(counter != 0)
					last = current;
				
				current = current->next;
				counter++;
			}
			/* if data can't be found */
			if(delete == NULL)
				printf(COLOR_YELLOW"Sorry, that item couldn't be found\n!"COLOR_RESET);
			else
				check = TRUE;
		}
			
	}
	printf(COLOR_YELLOW"%s -\"%s %s\" has now been removed from the Vmsystem.\n"COLOR_RESET,
		delete->data->id, delete->data->name, delete->data->desc);
	/* freeing the node and data */ 
	free(delete->data);
	free(delete);
	system->itemList->size--;
	organiseStockByID(system);
}


void displayCoins(VmSystem * system)
{ 
	printf(COLOR_YELLOW"Sorry, This feature is for a future release!\n"COLOR_RESET);
}


void resetStock(VmSystem * system)
{ 
	Node *current; 
	current = system->itemList->head;
	
	/* Loops through all the nodes and sets all of their
	stock levels to the default levels */
	while(current != NULL)
	{
		current->data->onHand = DEFAULT_STOCK_LEVEL;
		current = current->next;
	}
	
	printf(COLOR_YELLOW "Stock has been successfully reset!\n" COLOR_RESET);
}


void resetCoins(VmSystem * system)
{
	printf(COLOR_YELLOW"Sorry, This feature is for a future release!\n"COLOR_RESET);
}


void abortProgram(VmSystem * system)
{ 
	/* Frees the system before aborting */
	systemFree(system);
	printf(COLOR_YELLOW "System successfully aborted without saving state! Have a lovely day.\n" COLOR_RESET);
	exit(0);
}
