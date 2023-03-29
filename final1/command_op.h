#ifndef COMMAND_GUARD
#define COMMAND_GUARD

#include"common.h"
#include"input_process.h"


#define BUFFER_SIZE 255
#define MAX_OPEARTOR_CHAR 15 // len of projectMeeting + 1

#define DATE_LEN 9 // len of starting day + 1
#define START_TIME_LEN 5 // len of starting time + 1
#define DURATION_LEN 4 // len of duration + 1

/**
 * @brief struct to store the info of private time command.
*/
typedef struct PRIVATE_TIME {
    // int op_type = 0;
    int caller;
    long long starting_day_time; // 202304011800 = [April 1, 2023 18:00]
    double duration;
} pt_t;

/**
 * @brief struct to store the info of project meeting command.
*/
typedef struct PROJECT_MEETING {
    // int op_type = 1;
    int caller;
    int starting_day;
    int starting_time;
    int num_callee;
    int callee[MAX_CALLEE_NUM];
} pm_t;


pt_t priv_t_entry;
/**
 * @brief construct a private time entry according to input
 * @param input the input of private time operation
 * @param res the private time entry to be reflect
 * @param inst init command line info
 * @return 0 success, -1 error
*/
int private_time_handler(const char *input, pt_t *res, cmd_t *inst);
#endif