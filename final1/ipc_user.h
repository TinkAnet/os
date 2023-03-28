#ifndef IPC_USER_H
#define IPC_USER_H

#include "common.h"
#include "appointment.h"

extern schd_t schd_buffer[MAX_APPOINTMENT_NUM];
extern int user_pipe_list[MAX_USER_NUM][2];


/**
 * @brief start a user process
 * 
 * @param user_id 
 * @param start_day 
 * @param end_day 
 * @param people_num 
 */
void ipc_start_user_process(int user_id, long long start_day, long long end_day, int people_num);

/**
 * @brief stop a user process
 * 
 * @param user_id 
 */
void ipc_stop_user_process(int user_id);


/**
 * @brief launch all user process
 * 
 * @param tot_user_number the total user number
 * @param start_day 
 * @param end_day 
 * @param people_num 
 */
void ipc_launch_user(int tot_user_number, long long start_day, long long end_day, int people_num);

/**
 * @brief Shutdown all users processes
 *
 * @param tot_user_number the total user number
 */
void ipc_shutdown_user(int tot_user_number);


/**
 * Query if this schedule could be inserted
 * 
 * @param user_id the user id
 * @param s the schedule
*/
bool ipc_user_insert_query(int user_id, schd_t *s);

/**
 * Insert this schedule into user. (No additional check, please
 * use this function after `insert_query`)
 * 
 * @param user_id the user_id
 * @param s the schedule
*/
void ipc_user_insert(int user_id, schd_t *s);

/**
 * Query what needs to be deleted when inserting this schedule
 * 
 * @param user_id the user_id
 * @param s the schedule
 * @return the number of event that will be deleted. 
 *      (The result is stored at `schd_buffer`)
*/
int ipc_user_delete_query(int user_id, schd_t *s);

/**
 * Delete the schedule which is indicated by its id
 * 
 * @param user_id the user_id
 * @param schd_id the schedue id
 * 
*/
void ipc_user_delete(int user_id, int schd_id);

/**
 * @brief Dump the current user's schedules to `schd_list`
 * 
 * @param user_id 
 * @return int the number of appointments
 */
int ipc_user_print(int user_id);

#endif