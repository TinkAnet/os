#ifndef IPC_H
#define IPC_H

/**
 * User meta data struct
*/
typedef struct user_meta_data
{
    int num; // number of appintment
};

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
    char *people;   // people description length
};


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
int private_time(char* user_name, int event_date, int event_time, double event_duration);

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

#endif