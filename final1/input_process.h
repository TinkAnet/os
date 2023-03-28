#ifndef INPUT_GUARD
#define INPUT_GUARD
#include"common.h"
/**
 * TODO: needs 
*/
typedef struct COMMAND_LINE {
    int start_date;
    int end_date;
    int num_user;
    char *users[MAX_CALLEE_NUM];
    /* data */
} cmd_t;

cmd_t cmd_in;

int parse_cmd(int argc, char*argv[], cmd_t * inst);
#endif