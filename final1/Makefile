CC = /usr/bin/llvm-gcc

CFLAGS = -g -std=gnu99

EXECUTABLE = ./bin/test

CLI = ./CLI.c
COMMAND = ./command_op.c
PROCESS = ./input_process.c

# main
MAIN_PROG = ./main.c

.PHONY:cli
cli:
	$(CC) $(CFLAGS) $(CLI) $(COMMAND) $(PROCESS) $(MAIN_PROG) -o $(EXECUTABLE)
	
clean:
	rm -f *.o *~ $(EXECUTABLE)

run:
	./$(EXECUTABLE) 20230401 20230430 john mary lucy paul