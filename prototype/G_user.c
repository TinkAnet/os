#include "G_user.h"

/**
 * Init & Set needed objects
 * 
 */
char current_user_name[name_length];
int current_user_id;
int number_of_appointment = 0;
Appointment appointment_list[max_appointment_num];

user_meta_data retrieve_num;
user_appointment_data retrieve_ap_list[max_appointment_num];

/**
 * add an appointment of private time in the appointmnet list
 */
int add_private_time(int event_date, int event_time, double event_duration){
     appointment_list[number_of_appointment++] = load_private_ap(current_user_id, (long long)event_date, (long long)event_time, event_duration);
     return 0;
}

/**
 * retrieve the appointment
 *  
 */
int retrieve_my_appointment(){
    retrieve_num.num = number_of_appointment;
    for(int i = 0; i < retrieve_num.num; i++){
        retrieve_ap_list[i].date = appointment_list[i].p.start_time.d.str;
        retrieve_ap_list[i].start_time = appointment_list[i].p.start_time.str%10000;
        retrieve_ap_list[i].end_time = appointment_list[i].p.end_time.str%10000;
        retrieve_ap_list[i].type[0] = 0;
        strcpy(retrieve_ap_list[i].type, get_level_name(appointment_list[i].level));

        retrieve_ap_list[i].people_len = appointment_list[i].number;
        for(int j = 0; j < retrieve_ap_list[i].people_len; j++){
            retrieve_ap_list[i].people[j] = appointment_list[i].callee[j].uid;
        }

    }
    return 0;
}

/**
 * @brief Set the current user object
 * 
 * @param name pointer to user name
 * @return int 
 */
int set_current_user(char* name){
    int tmp = strlen(name);
    for(int i = 0; i < tmp; i++) current_user_name[i] = name[i];
    current_user_id = load_user(current_user_name).uid;
    return 0;
}
