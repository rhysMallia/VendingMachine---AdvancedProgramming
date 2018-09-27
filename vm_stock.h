#ifndef VM_STOCK_H
#define VM_STOCK_H

#include "vm_coin.h"

/**
 * The default stock level that all new stock should start with and the value
 * to use when restting stock.
 **/
#define DEFAULT_STOCK_LEVEL 20

#define STOCK_DELIM "|"
#define EXTRA_DELIM "."
#define COLOR_YELLOW "\33[33m"
#define COLOR_RESET "\33[0m"

void addNode( VmSystem *system, Node *current);
void addStock(Stock *stockFile, char *buffer);
void organiseStock(VmSystem * system);
void organiseStockByID(VmSystem * system);
List *createList();

#endif
