#include"command_op.h"
#include"common.h"
#include"input_process.h"
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

pt_t priv_t_entry;
pm_t pgg_entry;


// #define DEBUG
int private_time_handler (const char *input, pt_t *res, cmd_t *inst) {
    int n = strlen(input);
    char name_str[MAX_USER_NAME_LEN];
    char date_str[DATE_LEN + START_TIME_LEN]; // later will be used to store starting date and time.
    char time_str[START_TIME_LEN];
    char duration_str[DURATION_LEN];
    
    // init array to 0 so that after reading, '\0' is automatically appended to the end. 
    memset(name_str, 0, sizeof(name_str));
    memset(date_str, 0, sizeof(date_str));
    memset(time_str, 0, sizeof(time_str));
    memset(duration_str, 0, sizeof(duration_str));
    
    int name_len = 0;
    int date_len = 0;
    int time_len = 0;
    int duration_len = 0;
    
    char c;
    int state = 0; // read privateTime caller
    for (int i = 0; i < n; i++) {
        c = input[i];
#ifdef DEBUG
        printf("c = %c\n", c);
#endif
        if (state == 0 && c == ' ') {
            state = 1; // space transition state
            continue;
        }
        else if (state == 1 && c != ' ') {
            state = 2; // read starting day
        }
        else if (state == 2 && c == ' ') {
            state = 3; // space transition state
            continue;
        }
        else if (state == 3 && c != ' ') {
            state = 4; // read starting time
        }
        else if (state == 4 && c == ' ') {
            state = 5; // space transition state
            continue;
        }
        else if (state == 5 && c != ' ') {
            state = 6; // read duration
        }

        if (state == 0) {
            name_str[name_len++] = c;
            if (!isalpha(c)) {
                printf("invalid caller name input\n");
                return -1;
            }
        }
        else if (state == 2) {
            date_str[date_len++] = c;
            if (!isalnum(c)) {
                printf("invalid starting date input\n");
                return -1;
            }
        }
        else if (state == 4) {
            time_str[time_len++] = c;
            if (!isalnum(c)) {
                printf("invalid starting time input\n");
                return -1;
            }
        }
        else if (state == 6) {
            duration_str[duration_len++] = c;
        }
    }
    int caller_id = name_to_int(name_str, inst);
    if (!is_existing_user(caller_id, inst)) {
        printf("caller name does not exist in system!\n");
        return -1;
    }
    res->caller = caller_id;
    strcat(date_str, time_str);
#ifdef DEBUG
    printf("starting date and time = %s\n", date_str);
#endif
    res->starting_day_time = atoll(date_str);
    /** TODO: time interval checking. */
#ifdef DEBUG
    printf("res->starting_day_time %lld\n", res->starting_day_time);
#endif
    res->duration = atof(duration_str);
#ifdef DEBUG
    printf("res->duration = %lf\n", res->duration);
#endif
    return 0;
}

int project_group_gather_handler(const char *input, pm_t *res, cmd_t *inst) {
    int n = strlen(input);
    char name_str[MAX_USER_NAME_LEN];
    char date_str[DATE_LEN + START_TIME_LEN]; // later will be used to store starting date and time.
    char time_str[START_TIME_LEN];
    char duration_str[DURATION_LEN];
    char callee_str[MAX_CALLEE_NUM][MAX_USER_NAME_LEN];
    
    // init array to 0 so that after reading, '\0' is automatically appended to the end. 
    memset(name_str, 0, sizeof(name_str));
    memset(date_str, 0, sizeof(date_str));
    memset(time_str, 0, sizeof(time_str));
    memset(duration_str, 0, sizeof(duration_str));
    memset(callee_str, 0, sizeof(callee_str));
    

    int name_len = 0;
    int date_len = 0;
    int time_len = 0;
    int duration_len = 0;
    int callee_ptr = 0; // index for callee_str[callee_ptr], starting from 0
    int callee_len = 0;

    char c;
    int state = 0; // read projectMeeting caller
    for (int i = 0; i < n; i++) {
        c = input[i];
#ifdef DEBUG
        printf("c = %c\n", c);
#endif
        if (state == 0 && c == ' ') {
            state = 1; // space transition state
            continue;
        }
        else if (state == 1 && c != ' ') {
            state = 2; // read starting day
        }
        else if (state == 2 && c == ' ') {
            state = 3; // space transition state
            continue;
        }
        else if (state == 3 && c != ' ') {
            state = 4; // read starting time
        }
        else if (state == 4 && c == ' ') {
            state = 5; // space transition state
            continue;
        }
        else if (state == 5 && c != ' ') {
            state = 6; // read duration
        }
        else if (state == 6 && c == ' ') {
            state = 7; // space transition state
            continue;
        }
        else if (state == 7 && c != ' ') {
            state = 8; // read callee name
        }
        else if (state == 8 && c == ' ') {
            state = 9; // space transition state
            callee_len = 0; // reset offset
            callee_ptr++;
            continue;
        }
        else if (state == 9 && c != ' ') {
            state = 8; // read callee name
        }
        
        if (state == 0) {
            name_str[name_len++] = c;
            if (!isalpha(c)) {
                printf("invalid caller name input\n");
                return -1;
            }
        }
        else if (state == 2) {
            date_str[date_len++] = c;
            if (!isalnum(c)) {
                printf("invalid starting date input\n");
                return -1;
            }
        }
        else if (state == 4) {
            time_str[time_len++] = c;
            if (!isalnum(c)) {
                printf("invalid starting time input\n");
                return -1;
            }
        }
        else if (state == 6) {
            duration_str[duration_len++] = c;
        }
        else if (state == 8) {
            callee_str[callee_ptr][callee_len++] = c;
            if (!isalpha(c)) {
                printf("invalid callee name input\n");
                return -1;
            }
        }
    }
    int caller_id = name_to_int(name_str, inst);
    if (!is_existing_user(caller_id, inst)) {
        printf("caller name does not exist in system!\n");
        return -1;
    }
    res->caller = caller_id;
    strcat(date_str, time_str);
#ifdef DEBUG
    printf("starting date and time = %s\n", date_str);
#endif
    res->starting_day_time = atoll(date_str);
    /** TODO: time interval checking. */
#ifdef DEBUG
    printf("res->starting_day_time %lld\n", res->starting_day_time);
#endif
    res->duration = atof(duration_str);
#ifdef DEBUG
    printf("res->duration = %lf\n", res->duration);
#endif
    res->num_callee = callee_ptr + 1;
#ifdef DEBUG
    printf("res->num_callee = %d\n", res->num_callee);
#endif
    for (int i = 0; i <= callee_ptr; i++) {
        int callee_id = name_to_int(callee_str[i], inst);
        if (!is_existing_user(callee_id, inst)) {
            printf("callee name does not exist in system!\n");
            return -1;
        }
        res->callee[i] = callee_id;
    }
#ifdef DEBUG
    printf("callee_ptr = %d\n", callee_ptr);
    for (int i = 0; i <= callee_ptr; i++) {
        printf("callee: %s\n", callee_str[i]);
    }
#endif
    return 0;
}