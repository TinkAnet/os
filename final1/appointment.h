#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#define le(a, b) (a.str < b.str)
#define eq(a, b) (a.str == b.str)
#define ge(a, b) (a.str > b.str)

#include <stdbool.h>
#include "common.h"
#include "math.h"

extern int schd_cnt;
extern int total_user_num;
extern bool if_rejected[MAX_APPOINTMENT_NUM];
extern schd_t schd_list[MAX_APPOINTMENT_NUM];

typedef struct DATE{
    long long str;
    int year, month, day;
}date_t;

static int days(date_t a);
static date_t str_to_date(long long str);
static long long date_to_str(date_t d);
static date_t date_add_day(date_t a);

typedef struct TIME{
    long long str;
    date_t date;
    int hour, minute;
}tm_t;


static tm_t time_add_hm(tm_t a, int h, int m);
static tm_t str_to_time(long long s);
static long long time_to_str(tm_t a);
static int time_to_slot(tm_t a);
static tm_t slot_to_time(int s);

typedef struct APPOINTMENT{
	int id;
    int caller;
    int type;
    int callee_num;
    int callee[MAX_CALLEE_NUM];
    tm_t start_time, end_time;
    int start_slot, end_slot;
    bool if_user_choose_to_reject;
    double len, priv;
}schd_t;

void switch_to_reject_mode(schd_t *a);

static bool if_schd_conflict(const schd_t *a, const schd_t *b);

// abcd

// General

void init_appointment(long long start_day, long long end_day, int people_num);

schd_t load_schd(int id, int caller, int num_of_callee, int *callee, int type, long long start_time, double len);
 

// For user to operator

bool user_query_schd(schd_t a);

void user_insert_schd(schd_t a);

int user_delete_query(schd_t a, schd_t *out);

void user_delete_schd(int id);

int user_print_schd(schd_t *out);

// void user_start();

// For scheduler to call user to operate

// bool user_query(int id, const schd_t *s);

// bool user_delete(int id, const schd_t *s);

// bool user_insert(int id, const schd_t *s);

// void user_print(int id, int type, const schd_t *out);

// For scheduler to operating

bool schder_insert_query(schd_t s);

void schder_insert(schd_t s);

static int schder_delete_query(schd_t s);

static void schder_delete();

int schder_print();

bool schder_schd(schd_t s);

// For Main Process

schd_t re_schd(schd_t s);

#endif
