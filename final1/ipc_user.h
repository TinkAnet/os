#ifndef IPC_USER_H
#define IPC_USER_H

#include "appointment.h"

/**
 * Query if this schedule could be inserted
 * 
 * @param user_id the user id
 * @param s the schedule
*/
bool user_insert_query(int user_id, schd_t *s);

/**
 * Insert this schedule into user. (No additional check, please
 * use this function after `insert_query`)
 * 
 * @param user_id the user_id
 * @param s the schedule
*/
void user_insert(int user_id, schd_t *s);

/**
 * Query what needs to be deleted when inserting this schedule
 * 
 * @param user_id the user_id
 * @param s the schedule
 * @return the number of event that will be deleted. 
 *      (The result is stored at `user_appointment_list`)
*/
int user_delete_query(int user_id, schd_t *s);

/**
 * Delete the schedule which is indicated by its id
 * 
 * @param user_id the user_id
 * @param schd_id the scheduer id
 * 
*/
void user_delete(int user_id, int schd_id);

#endif