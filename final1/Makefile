CC = cc

CFLAGS = -g -ggdb -std=c99 -Wall -Werror 
# -DDEBUG 

EXECUTABLE = ./apo

# CLI = ./CLI.c
# COMMAND = ./command_op.c
# PROCESS = ./input_process.c
SRC_FILES = $(filter-out $(wildcard *_test.c), $(wildcard *.c))

# main
MAIN_PROG = ./main.c
# $(CLI) $(COMMAND) $(PROCESS) $(MAIN_PROG)


#  20230401 20230430 john mary lucy paul

.PHONY:cli
cli:
	$(CC) $(CFLAGS) $(SRC_FILES) -lm -o $(EXECUTABLE) 
	
clean:
	rm -f *.o *~ $(EXECUTABLE) *.txt *.dat

run:
	$(EXECUTABLE) 20230501 20230531 billy amada cathy danny emily
	

lldb:
	clear && lldb -- $(EXECUTABLE) 20230503 20230530 billy amada cathy danny emily

gdb:
	clear && gdb --args $(EXECUTABLE) 20230503 20230530 billy amada cathy danny emily