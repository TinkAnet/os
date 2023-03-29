#include"command_op.h"
#include"common.h"
#include"input_process.h"
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define DEBUG

int private_time_handler (const char *input, pt_t *res, cmd_t *inst) {
    int n = strlen(input);
    char name_str[MAX_USER_NAME_LEN];
    char date_str[DATE_LEN + START_TIME_LEN]; // later will be used to store starting date and time.
    char time_str[START_TIME_LEN];
    char duration_str[DURATION_LEN];
    
    int name_len = 0;
    int date_len = 0;
    int time_len = 0;
    int duration_len = 0;
    
    char c;
    int state = 0; // read privateTime caller
    for (int i = 0; i < n; i++) {
        c = input[i];
        printf("c = %c\n", c);
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
            if (!isnumber(c)) {
                printf("invalid starting date input\n");
                return -1;
            }
        }
        else if (state == 4) {
            time_str[time_len++] = c;
            if (!isnumber(c)) {
                printf("invalid starting time input\n");
                return -1;
            }
        }
        else if (state == 6) {
            duration_str[duration_len++] = c;
        }
    }
    name_str[name_len++] = 0;
    date_str[date_len++] = 0;
    time_str[time_len++] = 0;
    duration_str[duration_len++] = 0;
    res->caller = name_to_int(name_str, inst);
    strcat(date_str, time_str);
#ifdef DEBUG
    printf("starting date and time = %s\n", date_str);
#endif
    res->starting_day_time = atoll(date_str);
#ifdef DEBUG
    printf("res->starting_day_time %lld\n", res->starting_day_time);
#endif
    res->duration = atof(duration_str);
#ifdef DEBUG
    printf("res->duration = %lf\n", res->duration);
#endif
    return 0;
}
