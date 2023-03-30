CC = cc

CFLAGS = -g -ggdb -std=c99 -Wall -Werror
# -DDEBUG 

EXECUTABLE = ./bin/test

# CLI = ./CLI.c
# COMMAND = ./command_op.c
# PROCESS = ./input_process.c
SRC_FILES = $(filter-out $(wildcard *_test.c), $(wildcard *.c))

# main
MAIN_PROG = ./main.c
# $(CLI) $(COMMAND) $(PROCESS) $(MAIN_PROG)

.PHONY:cli
cli:
	$(CC) $(CFLAGS) $(SRC_FILES) -lm -o $(EXECUTABLE)
	
clean:
	rm -f *.o *~ $(EXECUTABLE)

run:
	./$(EXECUTABLE) 20230401 20230430 john mary lucy paul

lldb:
	clear && lldb -- ./$(EXECUTABLE) 20230401 20230430 john mary lucy paul

gdb:
	clear && gdb --args ./$(EXECUTABLE) 20230401 20230430 john mary lucy paul