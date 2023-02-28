#ifndef _AP_H
#define _AP_H
#define callee_max_num 10
#define level_max 5
#define level_name_length 20
#include "G_time.h"
#include "G_user.h"
#include <math.h>

struct Appointment{
	Period p;
	int len, level, number;
	User caller, callee[callee_max_num];
	bool ifconfirmed;
};

void init_all(long long start_date, long long end_date, int number_of_users, char users_name[][name_length]);
Appointment load_ap(char *levels, char *caller_name, long long date_str, long long time_str, double period_length, int number_of_callees, char callee[][name_length]);
bool ap_conflict(Appointment a, Appointment b);
Appointment set_if_confirmed(Appointment a, bool flags);
bool contains(Appointment a, User b);
void ap_print(Appointment a);

#else

#endif
