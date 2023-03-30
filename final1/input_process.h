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

extern cmd_t cmd_in;

/**
 * @brief parse init command line arguments. 
 * @return -1 invalid init command line arguments, 0 valid
*/
int parse_cmd(int argc, char*argv[], cmd_t* inst);

/**
 * @brief map from name to int. The first user has id of 1
 * @return -1 not found mapping, otherwise valid mapping
*/
int name_to_int(const char *name, cmd_t* inst);

/**
 * @brief check whether the id is the id of existing user
 * @param id id to be checked
 * @return 1 yes 0 no
*/
int is_existing_user(int id, cmd_t* inst);
#endif