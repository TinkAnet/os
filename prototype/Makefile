# CC = /usr/bin/llvm-gcc
CC = gcc
# CC is the path of compiler
CFLAGS = -g -lm
# -Werror: Make all warnings into errors.
# -Wall: enables all the warnings about constructions that some users consider questionable
# -g: tells the compiler to embed debugging information for the debugger to use.
# -O2: O2 optimisation
# -Wno-unused-function: turns off gcc warnings about unused function.
EXECUTABLE = ./prototype

# SRC_DIR = ./src

INPUT_HANDLER = ./input_process.c
SERVICE = ./*.c
INTER_PROCESS = ./ipc.c

# main
MAIN_HARDWARE = ./prototype.c


SRC = $(wildcard *.c)

.PHONY:prototype

prototype:
	$(CC) -o $(EXECUTABLE) $(CFLAGS) $(SERVICE) -lm
# cli: 
#	$(CC) $(CFLAGS) $(INTER_PROCESS) $(SERVICE) $(INPUT_HANDLER) $(MAIN_HARDWARE) -lm -o $(EXECUTABLE)
# -I means set path of #include to the path of specified (SRC) working directory so that we could use relative path when writing #include
# -o means generate an executable named EXE. 

clean:
	rm -f *.o *~ $(EXECUTABLE)
# rm -f means delete a directory recursively. 
# *~ means all files ending in ~. Many Unix/Linux systems programs create backup files that end in ~.
run:
	./$(EXECUTABLE) 20230401 20230430 john mary lucy paul