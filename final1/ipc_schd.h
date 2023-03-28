#ifndef IPC_SCHD_H
#define IPC_SCHD_H

#include "appointment.h"

/**
 * Query if this schedule could be inserted
 * 
 * @param schder_id the scheduler id
 * @param s the schedule
*/
bool schd_insert_query(int schder_id, schd_t *s);


/**
 * Insert this schedule to the scheduer
 * 
 * @param schder_id the scheduer id
 * @param s the scheduer
*/
bool schd_insert(int schder_id, schd_t *s);



#endif