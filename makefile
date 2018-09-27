CFLAGS= -ansi -Wall -pedantic 
LFLAGS =
EXECUTABLE = vm
SOURCES = utility.c vm.c vm_coin.c vm_menu.c vm_options.c vm_stock.c 
GAME_OBJECTS = utility.o vm.o vm_coin.o vm_menu.o vm_options.o vm_stock.o
HEADERS = utility.h vm.h vm_coin.h vm_menu.h vm_options.h vm_stock.h vm_system.h

all: ${EXECUTABLE}
vm:${GAME_OBJECTS}
	gcc ${LFLAGS} $^ -o $@
	
%.o: %.c ${HEADERS}
	gcc ${CFLAGS} -c $<
	
.PHONY:clean
clean:
	rm -f ${GAME_OBJECTS} ${EXECUTABLE}

archive:
	zip $(USER)-a2 $(SOURCES) $(HEADERS) makefile valgrindLog.txt
