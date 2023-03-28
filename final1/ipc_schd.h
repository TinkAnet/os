#ifndef IPC_SCHD_H
#define IPC_SCHD_H

#include "common.h"
#include "appointment.h"

extern int schd_pipe_list[SCHD_NUM][2];

/**
 * @brief start a scheduler process with a scheduer function id
 * @details 
    0 -> FCFS
    1 -> Priority
    2 -> Round Robine
    3 -> "Big Meeting" First
 * 
 * @param schder_id the scheduler id
 */
void ipc_start_schd_process(int schder_id);

/**
 * @brief Stop a scheduer process
 * 
 * @param schder_id 
 */
void ipc_stop_schd_proces(int schder_id);

/**
 * @brief launch all scheduler processes
 */
void ipc_launch_schd();

/**
 * @brief shutdown all scheduler processes
 * 
 */
void ipc_shutdown_schd();


/**
 * @brief Query if this schedule could be inserted
 * 
 * @param schder_id the scheduler id
 * @param s the schedule
*/
bool ipc_schd_insert_query(int schder_id, schd_t *s);


/**
 * @brief Insert this schedule to the scheduer
 * 
 * @param schder_id the scheduer id
 * @param s the scheduer
*/
bool ipc_schd_insert(int schder_id, schd_t *s);

/**
 * @brief Dump the user scheduler
 * 
 * @param schder_id the scheduler id
 * @param user_id the user id
 * @return int the number of appointments
 */
int ipc_schd_print(int schder_id, int user_id);


#endif