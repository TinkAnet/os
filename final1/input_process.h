#ifndef INPUT_GUARD
#define INPUT_GUARD
#include"common.h"

typedef struct USERS {
    char* name;
    int id;
} usr_t;

/**
 * @brief command line info storage.
*/
typedef struct COMMAND_LINE {
    long long start_date;
    long long end_date;
    int num_user;
    usr_t user_container[MAX_CALLEE_NUM];
} cmd_t;

cmd_t cmd_in;

int parse_cmd(int argc, char*argv[], cmd_t* inst);
int name_to_int(const char *name, cmd_t* inst);

#endif