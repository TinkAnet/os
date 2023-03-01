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

typedef struct USER{
    int uid;
}User;

void init_user(int number, char s[][name_length]);
void user_print(User a);
char* get_user_name(User a);
User load_user(char *s);
bool same(User a, User b);



typedef struct APPOINTMENT{
	Period p;
	int len, level, number;
	User caller, callee[callee_max_num];
	bool ifconfirmed;
}Appointment;

void init_all(long long start_date, long long end_date, int number_of_users, char users_name[][name_length]);
Appointment load_ap(char *levels, char *caller_name, long long date_str, long long time_str, double period_length, int number_of_callees, char callee[][name_length]);
Appointment load_private_ap(int user_id, long long date_str, long long time_str, double period_length);
bool ap_conflict(Appointment a, Appointment b);
Appointment set_if_confirmed(Appointment a, bool flags);
bool contains(Appointment a, User b);
void ap_print(Appointment a);
char* get_level_name(int a);



#else

#endif
