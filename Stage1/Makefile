CC = /usr/bin/llvm-gcc
# CC is the path of compiler
CFLAGS = -g -std=gnu99 -Wno-unused-function
# -Werror: Make all warnings into errors.
# -Wall: enables all the warnings about constructions that some users consider questionable
# -g: tells the compiler to embed debugging information for the debugger to use.
# -O2: O2 optimisation
# -Wno-unused-function: turns off gcc warnings about unused function.
EXECUTABLE = ./CLI_t

# SRC_DIR = ./src

INPUT_HANDLER = ./input_process.c

# main
MAIN_HARDWARE = ./CLI.c

.PHONY:cli
cli:
	$(CC) $(CFLAGS) $(INPUT_HANDLER) $(MAIN_HARDWARE) -o $(EXECUTABLE)
# -I means set path of #include to the path of specified (SRC) working directory so that we could use relative path when writing #include
# -o means generate an executable named EXE. 

clean:
	rm -f *.o *~ $(EXECUTABLE)
# rm -f means delete a directory recursively. 
# *~ means all files ending in ~. Many Unix/Linux systems programs create backup files that end in ~.
run:
	./$(EXECUTABLE) 20230401 20230430 john mary lucy paul