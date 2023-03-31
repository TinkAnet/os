#ifndef CLI_GUARD
#define CLI_GUARD

#include<stdint.h>
#include<stdlib.h>
#include"input_process.h"
#include"command_op.h"

extern pt_t priv_t_entry; // private time entry
extern pm_t pgg_entry; // project meeting, group study, gathering entry
extern int sequence_number; // sequence number of report printed, starting from 1
extern int op_id; // appointment id, each appointment has a unique id, once we receive an appointment, let op_id += 1, alos used as counter of "Number of Request Received".
extern int user_accept_count[MAX_CALLEE_NUM + 1]; // user_accept_count[id] = This value is equal to the number of appointments that the user with id has successfully created as a caller under a specific scheduling algorithm X.
extern int accepted_act_people; // The total number of people participating in activities under the current scheduling algorithm.
extern int caller_request_accepted; // in a specifc scheduling algorithms, iterate all users, and count the number of appointments they make (as caller)
extern int user_time_slots[MAX_CALLEE_NUM + 1]; // user_time_slots[id] = The total time of all appointments that the user with id has participated in, under a specific scheduling algorithm X.

/**
 * @brief after format checking of the command line arguments, run the APO directly.
 * @param cmd_in valid command line arguments
*/
int run(cmd_t* cmd_in);
#endif