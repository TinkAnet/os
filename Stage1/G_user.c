#include "G_user.h"

char current_user_name[name_length];
int current_user_id;
int number_of_appointment = 0;
Appointment appointment_list[max_appointment_num];

user_meta_data retrieve_num;
user_appointment_data retrieve_ap_list[max_appointment_num];

int add_private_time(int event_date, int event_time, double event_duration){
     appointment_list[number_of_appointment++] = load_private_ap(current_user_id, (long long)event_date, (long long)event_time, event_duration);
     return 0;
}

int retrieve_my_appointment(){
    retrieve_num = number_of_appointment;
    for(int i = 0; i < retrieve_num; i++){
        retrieve_ap_list[i].date = appointment_list[i].p.start_time.d.str;
        retrieve_ap_list[i].start_time = appointment_list[i].p.start_time.str;
        retrieve_ap_list[i].end_time = appointment_list[i].p.end_time.str;
        retrieve_ap_list[i].type = {0};
        strcpy(retrieve_ap_list[i].type, get_level_name(appointment_list[i].level));

        retrieve_ap_list[i].people = {0};

        sprintf(retrieve_ap_list[i].people, "%s", get_user_name(appointment_list[i].caller));

        for(int j = 0; j < appointment_list[i].number; j++)
            sprintf(retrieve_ap_list[i].people, "%s %s", retrieve_ap_list[i].people, get_user_name(appointment_list[i].caller[j]));
        retrieve_ap_list[i].people_len = strlen(retrieve_ap_list[i].people);
    }
    return 0;
}

int set_current_user(char* name){
    int tmp = strlen(name);
    for(int i = 0; i < tmp; i++) current_user_name[i] = name[i];
    current_user_id = load_user(current_user_name);
    return 0;
}
