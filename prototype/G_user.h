#ifndef _USER_H
#define _USER_H
#define max_appointment_num 10000
#include "G_ap.h"
#include <string.h>


/**
 * A structure that used for transferring information to Parent process
 * 
 */
typedef struct user_meta_data
{
    int num; // number of appintment
}user_meta_data;

/**
 * User appointment data
 */
typedef struct user_appointment_data
{
    long long date; // appointment date
    int start_time; // appointment start time
    int end_time;   // end time
    char type[32];  // type description
    int people_len; // people description length
    int people[callee_max_num];   // people description length
}user_appointment_data;

/**
 * global variable
 */
extern user_meta_data retrieve_num;

/**
 *global variable
 */
extern user_appointment_data retrieve_ap_list[max_appointment_num];

/**
 * @brief Set the current user object
 * 
 * @param name the user name
 * @return int 
 */
int set_current_user(char* name);

/**
 * @brief Add an appointment of private time
 * 
 * @param event_date the private_time date
 * @param event_time the private_time time
 * @param event_duration the private_time duration
 * @return int 
 */
int add_private_time(int event_date, int event_time, double event_duration);

/**
 * @brief retrieve the appointment
 * 
 * @return int 
 */
int retrieve_my_appointment();

#endif
