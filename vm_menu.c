#include "vm_menu.h"

/**
 * vm_menu.c handles the initialisation and management of the menu array.
 **/

/**
 * In this function you need to initialise the array of menu items
 * according to the text to be displayed for the menu. This array is
 * an array of MenuItem with text and a pointer to the function
 * that will be called.
 **/
void initMenu(MenuItem * menu)
{ 
	/* Adding all the menu functions and text to the menu object array */
	menu[0].function = displayItems;
	strcpy(menu[0].text, "1. Display Items\n");
	
	menu[1].function = purchaseItem;
	strcpy(menu[1].text, "2. Purchase Items\n");
	
	menu[2].function = saveAndExit;
	strcpy(menu[2].text, "3. Save and Exit\n");
		
	menu[3].function = addItem;
	strcpy(menu[3].text, "4. Add Item\n");
	
	menu[4].function = removeItem;
	strcpy(menu[4].text, "5. Remove Item\n");
	
	menu[5].function = displayCoins;
	strcpy(menu[5].text, "6. Display Coins\n");
	
	menu[6].function = resetStock;
	strcpy(menu[6].text, "7. Reset Stock\n");
	
	menu[7].function = resetCoins;
	strcpy(menu[7].text, "8. Reset Coins\n");
	
	menu[8].function = abortProgram;
	strcpy(menu[8].text, "9. Abort Program");
}

/**
 * Gets input from the user and returns a pointer to the MenuFunction
 * that defines how to perform the user's selection. NULL is returned
 * if an invalid option is entered.
 **/
MenuFunction getMenuChoice(MenuItem * menu)
{
	Boolean valid = FALSE;
	Boolean buffer = FALSE;
	int x;
	int xX;
	int answer;
	char input[3];

	for(x = 0; x < 9; x++)
	{
		if(x == 0)
			printf("Main Menu:\n");
		printf("%s",menu[x].text);
		if(x == 2)
			printf("Administrator-Only Menu: \n");
	}
	
	/* Input loop and checkers for menu */
	while(!valid)
	{
		printf("\nSelect your option (1-9):");	
		fgets(input, sizeof(input), stdin);
		/* System to check if the buffer has overflowed! */		
		for(xX = 0; xX < 3; xX++)
		{
			if(input[xX] == '\n')
			buffer = TRUE;
		}
		/* checking if buffer has overflowed */
		if(!buffer)
			readRestOfLine();
			
		/*checking if input is digit */
		if(isdigit(input[0]) && input[1] == '\n')
			valid = TRUE;
		else
		{	
			printf(COLOR_YELLOW"Error, please enter a single integer!" COLOR_RESET);
		}
		
	}
	
	/* This scans the string and pulls the int out */
	sscanf(input, "%d", &answer);
	answer--;
	return menu[answer].function;
}
