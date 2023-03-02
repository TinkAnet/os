#ifndef _AP_H
#define _AP_H
#define callee_max_num 10
#define level_max 5
#define private_level 4
#define project_metting_level 3
#define group_study 2
#define gathering 1
#define level_name_length 20
#define name_length 15
#define user_max_num 11
#include "G_time.h"
#include "G_user.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief 
 * 
 */
typedef struct USER{
    int uid;
}User;

/**
 * @brief Init user
 * 
 * @param number the number of users
 * @param s an 2-D string that holds the information of users
 */
void init_user(int number, char s[][name_length]);

/**
 * @brief print user
 * 
 * @param a the User
 */
void user_print(User a);

/**
 * @brief Get the user name object
 * 
 * @param a the User
 * @return char* 
 */
char* get_user_name(User a);

/**
 * @brief Load user
 * 
 * @param s pointer
 * @return User 
 */
User load_user(char *s);

/**
 * @brief judge whether 2 users are the same person
 * 
 * @param a User
 * @param b User
 * @return true : 2 users are the same
 * @return false : 2 users are different
 */
bool same(User a, User b);


/**
 * Appointment
 * len : occupy length of time slot
 * level : priority
 * number : the number of callee
 * ifconfirmed : status of the appointment
 */
typedef struct APPOINTMENT{
	Period p;
	int len, level, number;
	User caller, callee[callee_max_num];
	bool ifconfirmed;
}Appointment;

/**
 * @brief Init all
 * 
 * @param start_date the start date
 * @param end_date the end date
 * @param number_of_users the number of users
 * @param users_name users' names
 */
void init_all(long long start_date, long long end_date, int number_of_users, char users_name[][name_length]);

/**
 * @brief Set an appointment
 * 
 * @param levels appointment levels
 * @param caller_name appointment caller name
 * @param date_str appointment date
 * @param time_str appointment time
 * @param period_length the hours of the appointment
 * @param number_of_callees the number of callees
 * @param callee list of the callees
 * @return Appointment 
 */
Appointment load_ap(char *levels, char *caller_name, long long date_str, long long time_str, double period_length, int number_of_callees, char callee[][name_length]);

/**
 * @brief Set a private_time appointment
 * 
 * @param user_id the user_id
 * @param date_str date of value form(appointment date)
 * @param time_str time of value form(appointment time)
 * @param period_length private_time duration
 * @return Appointment 
 */
Appointment load_private_ap(int user_id, long long date_str, long long time_str, double period_length);

/**
 * @brief Judge whether a conflict exist between 2 appointments
 * 
 * @param a Appointment
 * @param b the other appointmnet
 * @return true : conflict
 * @return false : no conflict
 */
bool ap_conflict(Appointment a, Appointment b);

/**
 * @brief Set the status of the appointment
 * 
 * @param a the appointment
 * @param flags 
 *          -true : set to true
 *          -false : set to false
 * @return Appointment 
 */
Appointment set_if_confirmed(Appointment a, bool flags);

/**
 * @brief Judge whether the user is a participant of the appointment
 * 
 * @param a the appointment
 * @param b the user
 * @return true : the user is a participant
 * @return false : the user is not a participant
 */
bool contains(Appointment a, User b);

/**
 * @brief print appointment
 * 
 * @param a the appointment
 */
void ap_print(Appointment a);
char* get_level_name(int a);



#else

#endif
