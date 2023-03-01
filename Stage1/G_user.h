#ifndef _USER_H
#define _USER_H
#define max_appointment_num 10000
#include "G_AP.h"
#include <string.h>


typedef struct user_meta_data
{
    int num; // number of appintment
}user_meta_data;

typedef struct user_appointment_data
{
    long long date; // appointment date
    int start_time; // appointment start time
    int end_time;   // end time
    char type[32];  // type description
    int people_len; // people description length
    char *people;   // people description length
}user_appointment_data;

extern user_meta_data retrieve_num;
extern user_appointment_data retrieve_ap_list[max_appointment_num];

int set_current_user(char* name);

int add_private_time(int event_date, int event_time, double event_duration);

int retrieve_my_appointment();

#endif
