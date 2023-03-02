#ifndef IPC_H
#define IPC_H

#include "G_user.h"


/**
 * Init children processes
 * 
 * @param start_date the start date
 * @param end_date the end date
 * @param number number of children process
 * @param user_name_list the user list
 * @return the instruction execution result. 0 -> success; !0 -> failed
 * 
*/
int init_child_process(int start_date, int end_date, int number, char *user_name_list[]);

/**
 * Private Time instruction
 * 
 * @param user_name user name
 * @param event_date event date
 * @param event_time event time
 * @param event_duration event duration
 * @return the instruction execution result. 0 -> success; !0 -> failed
 * 
 * More details are in the project materials
 * 
*/
int private_time(const char* user_name, int event_date, int event_time, double event_duration);

/**
 * Retrieve User All Appointment
 * 
 * @param user_name the user name
 * @param meta the returned meta address
 * @param list the returned list address
 * @return the instruction execution result. 0 -> success; !0 -> failed
 * 
*/
int retrieve_user_appointment(char* user_name, user_meta_data *meta, user_appointment_data **list);

/**
 * Shutdown child process
 * 
 * @return the instruction execution result. 0 -> success; !0 -> failed
*/
int shutdown_child_process();

#endif
