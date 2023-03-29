#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include"CLI.h"
#include"input_process.h"

int main(int argc, char*argv[]) {
    cmd_t *usr_in = (cmd_t*)&cmd_in;
    int is_passed = parse_cmd(argc, argv, usr_in);
    if (is_passed == -1) {
        printf("invalid command line argument!\n");
        return -1;
    }
    run(usr_in);
    return 0;
}