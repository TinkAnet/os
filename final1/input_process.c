// #include<rinput.h>
#include"common.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"input_process.h"

#define FI_USR_OFFSET 3 // ./apo YYYYMMDD YYYYMMDD u1 (pos of u1 = 0 + 3)

int is_existing_user(int id, cmd_t* inst) {
    int n = inst->num_user;
    for (int i = 0; i < n; i++) {
        if (id == inst->user_container[i].id) {
            return 1;
        }
    }
    return 0;
}

int name_to_int(const char *name, cmd_t* inst) {
    int n = inst->num_user;
    for (int i = 0; i < n; i++) {
        if (strcmp(name, inst->user_container[i].name) == 0) {
            return inst->user_container[i].id;
        }
    }
    return -1;
}

int parse_cmd(int argc, char* argv[], cmd_t * inst) {
    long long start_date, end_date;
    int valid = 1;
    long long s_valid = sscanf(argv[1], "%lld", &start_date);
    long long e_valid = sscanf(argv[2], "%lld", &end_date);
    
    valid = valid && s_valid == 1;
    valid = valid && e_valid == 1;
    /** check for invalid start date and end date. */
    if (!valid) {
        printf("Invalid format for start date and end date\n");
        return -1;
    }
    inst->start_date = start_date;
    inst->end_date = end_date;
    int num_of_users = argc - FI_USR_OFFSET;
    if (num_of_users < MIN_CALLEE_NUM || num_of_users > MAX_CALLEE_NUM) {
        printf("Invalid number of users!\n");
        return -1;
    }
    inst->num_user = num_of_users;
    for (int i = 0; i < num_of_users; i++) {
        int l = strlen(argv[i+FI_USR_OFFSET]);
        for (int j = 0; j < l; j++) {
            if (!isalpha(argv[i+FI_USR_OFFSET][j])) {
                printf("Invalid format for user name inputs\n");
                return -1;
            }
        }
        inst->user_container[i].id = i + 1;
        inst->user_container[i].name = argv[i + FI_USR_OFFSET];
    }
    return 0;
}