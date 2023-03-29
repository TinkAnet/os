/**
 * @file appointment.h
 * @author Group 08
 * @brief The header file of appointment.c, defines needed structures.
 * @version Final 1
 * @date 2023-03-28
 * 
 * 
 */
#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#define le(a, b) (a.str < b.str)
#define eq(a, b) (a.str == b.str)
#define ge(a, b) (a.str > b.str)

#include <stdbool.h>
#include "common.h"
#include "math.h"
#include <stdio.h>

extern int schd_cnt;
extern int total_user_num;
extern bool if_rejected[MAX_APPOINTMENT_NUM];

typedef struct DATE{
    long long str;
    int year, month, day;
}date_t;

typedef struct TIME{
    long long str;
    date_t date;
    int hour, minute;
}tm_t;


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

extern schd_t schd_list[MAX_APPOINTMENT_NUM];


void switch_to_reject_mode(schd_t *a);

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

int schder_delete_query(schd_t s);

void schder_delete();

int schder_print();

bool schder_schd(schd_t s);

// For Main Process

schd_t re_schd(schd_t s);

#endif
