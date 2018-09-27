#include "vm.h"
#include "stdio.h"
#define MENU_L 9
int main(int argc, char ** argv)
{
	Boolean check = FALSE;
	VmSystem *system = malloc(sizeof(VmSystem));
	MenuFunction mFun;
	MenuItem menu[MENU_L];
	
	
	
	/* initializing the system */
	systemInit(system); 
	
	
	if(argc == 1)
	{
		printf(COLOR_YELLOW "NO STOCK FILE LOADED, EXITING...\n" COLOR_RESET);
		exit(0);
	}
	else
	{
		loadData(system, argv[1],argv[2]);
		loadStock(system, argv[1]);
		organiseStock(system);
	} 
	/* initializing of menu -> vm_MENU */
	initMenu(menu);
	while(!check)
	{
		mFun = getMenuChoice(menu);
		mFun(system);
	}

	
    return EXIT_SUCCESS;
}
