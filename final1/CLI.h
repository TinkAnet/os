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
extern int user_accept_count[MAX_CALLEE_NUM + 1]; // user_accept_count[id] = the number of successful appointments that the user with id participated in, regardless of whether the user was a caller or a callee.
extern int accepted_act_people[MAX_CALLEE_NUM + 1]; // accepted_act_people[id] = 1 -> user with id has participated in some activities under the current scheduling algorithm, accepted_act_people[id] = 0 -> has not
extern int caller_request_accepted; // in a specifc scheduling algorithms, iterate all users, and count the number of appointments they make (as caller)
extern int user_time_slots[MAX_CALLEE_NUM + 1]; // user_time_slots[id] = The total time of all appointments that the user with id has participated in, under a specific scheduling algorithm X.

/**
 * @brief after format checking of the command line arguments, run the APO directly.
 * @param cmd_in valid command line arguments
*/
int run(cmd_t* cmd_in);
#endif