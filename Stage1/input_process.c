// #include<rinput.h>
#include"rinput.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "G_AP.h"

// #define DEBUG
/**
 * Use to parse private time string. 
*/
void private_time_handler (const char *input, pt_t *res) {
    int n = strlen(input);
    char c;
    
    char op_str[MAX_OPEARTOR_CHAR] = {'\0'}; // strlen(op_str) = 0
    char user_str[name_length] = {'\0'};
    char date_str[DATE_LEN] = {'\0'};
    char start_str[START_TIME_LEN] = {'\0'};
    char duration_str[DURATION_LEN] = {'\0'};

    int op_len = 0;
    int user_len = 0;
    int date_len = 0;
    int start_len = 0;
    int duration_len = 0;
    int state = 0;
    
    int reach_space = 0;
    for (int i = 0; i < n; i++) {
        c = input[i];
        if (state == 0 && c != ' ') {
            state = 1; // read operator
        }
        else if (state == 1 && c == ' ') {
            state = 2; // reach 1th space
            reach_space++;
            op_str[op_len++] = '\0';
            continue;
        }
        else if (state == 2 && c != ' ' && reach_space == 1) {
            state = 3; // time to read users' string
        }
        else if (state == 3 && c == ' ') {
            state = 4; // reach 2th space
            reach_space++;
            user_str[user_len++] = '\0';
            continue;
        }
        else if (state == 4 && c != ' ' && reach_space == 2) {
            state = 5; // read date
        }
        else if (state == 5 && c == ' ') {
            state = 6; // reach 3th space
            reach_space++;
            date_str[date_len++] = '\0';
            continue;
        }
        else if (state == 6 && c != ' ' && reach_space == 3) {
            state = 7; // read starting time
        }
        else if (state == 7 && c == ' ') {
            state = 8; // reach 4th space
            reach_space++;
            start_str[start_len++] = '\0';
            continue;
        }
        else if (state == 8 && c != ' ' && reach_space == 4) {
            state = 9; // read duration
        }
        else if (state == 9) {
            int isnotend = 0;
            isnotend = isnotend || c == '0';
            isnotend = isnotend || c == '1';
            isnotend = isnotend || c == '2';
            isnotend = isnotend || c == '3';
            isnotend = isnotend || c == '4';
            isnotend = isnotend || c == '5';
            isnotend = isnotend || c == '6';
            isnotend = isnotend || c == '7';
            isnotend = isnotend || c == '8';
            isnotend = isnotend || c == '9';
            isnotend = isnotend || c == '.';
            if (!isnotend) {
                duration_str[duration_len++] = '\0';
                break;
            }
        }

        if (state == 1) {
            op_str[op_len++] = c;
            continue;
        } else if (state == 3) {
            user_str[user_len++] = c;
            continue;
        }
        else if (state == 5) {
            date_str[date_len++] = c;
            continue;
        }
        else if (state == 7) {
            start_str[start_len++] = c;
            continue;
        }
        else if (state == 9) {
            duration_str[duration_len++] = c;
            continue;
        }
    }
    res->op = PRIVATE_TIME;
    res->caller_name = user_str;
    res->starting_time = atoi(start_str);
    res->date = atoi(date_str);
    res->duration = atof(duration_str);
#ifdef DEBUG
    printf("op_str -> %s\n", op_str);
    printf("user_str -> %s\n", user_str);
    printf("date_str -> %s\n", date_str);
    printf("start_str -> %s\n", start_str);
    printf("duration_str -> %s\n", duration_str);
#endif
}