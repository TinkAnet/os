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
    int caller;
    long long starting_day_time; // 202304011800 = [April 1, 2023 18:00]
    double duration;
} pt_t;

/**
 * @brief struct to store the info of project meeting, group study, gathering command.
*/
typedef struct PROJECT_GROUP_GATHER {
    int caller;
    long long starting_day_time; // 202304011800 = [April 1, 2023 18:00]
    double duration;
    int num_callee;
    int callee[MAX_CALLEE_NUM];
} pm_t;

pt_t priv_t_entry; // private time entry
pm_t pgg_entry; // project meeting, group study, gathering entry

/**
 * @brief construct a private time entry according to input
 * @details raise error message when: not alpha char in name, not number char in time, caller name does not exist
 * @param input private time command excluding "privateTime"
 * @param res the private time entry to be reflect
 * @param inst init command line info
 * @return 0 success, -1 error
*/
int private_time_handler(const char *input, pt_t *res, cmd_t *inst);

/**
 * @brief construct a project meeting / group study / gathering entry entry according to input
 * @details raise error message when: not alpha char in name, not number char in time, caller/callee name does not exist
 * @param input projectMeeting, groupStudy, gathering command
 * @param res the project meeting / group study / gathering entry entry to be reflected.
 * @param inst init command line info
 * @return 0 success, -1 error
*/
int project_group_gather_handler(const char *input, pm_t *res, cmd_t *inst);
#endif