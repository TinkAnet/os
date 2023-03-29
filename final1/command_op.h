#ifndef COMMAND_GUARD
#define COMMAND_GUARD

#include"common.h"

#define BUFFER_SIZE 255
#define MAX_OPEARTOR_CHAR 15 // which is the len of projectMeeting
#define DATE_LEN 9
#define START_TIME_LEN 5
#define DURATION_LEN 4

// typedef enum OPEARTOR_TYPE {
//     PRIVATE_TIME,   // 0
//     PROJECT_MEETING,// 1
//     GROUP_STUDY,    // 2
//     GATHERING,      // 3
// } op_t;

/**
 * @brief struct to store the info of private time command.
*/
typedef struct PRIVATE_TIME {
    int op_type = 0;
    int caller;
    int date; // read by atoi
    long long starting_day; // 20230401 = [April 1, 2023]
    double duration; // 
} pt_t;

/**
 * @brief struct to store the info of project meeting command.
*/
typedef struct PROJECT_MEETING {
    int op_type = 1;
    int caller;
    int starting_day;
    int starting_time;
    int num_callee;
    int callee[MAX_CALLEE_NUM];
} pm_t;


extern pt_t priv_t_entry;
/**
 * @brief construct a private time entry according to input
 * @param input the input of private time operation
 * @param res the private time entry to be reflect
*/
void private_time_handler(const char *input, pt_t *res);
#endif