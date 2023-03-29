#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include <unistd.h>
#include <string.h>
#include"input_process.h"
#include"ipc_user.h"
#include"ipc_schd.h"
#include"command_op.h"

#define DEBUG

int run(cmd_t* in) {
    // init child process
    for (int i = 0; i < in->num_user; i++) {
        // ipc_start_schd_process(in->user_container[i].id, in->start_date, in->end_date, in->num_user);
    }
    char buffer[BUFFER_SIZE];
    while (true) {
        printf("Please enter appointment:\n");
        int apm_len = read(STDIN_FILENO, buffer, BUFFER_SIZE);
        if (apm_len <= 1) {
            printf("EOF or invalid input error!\n");
            return -1;
        }
        buffer[--apm_len] = 0; // remove newline character
#ifdef DEBUG
        printf("buffer : %s\n", buffer);
#endif
        char op[MAX_OPEARTOR_CHAR];
        int len_op = 0;
        for (int i = 0; i < apm_len; i++) {
            if (buffer[i] == ' ') break;
            op[len_op++] = buffer[i];
        }
        op[len_op++] = 0;
        
#ifdef DEBUG
        printf("op : %s with len %lu\n", op, strlen(op));
#endif
        if (strcmp(op, "privateTime") == 0) {
            pt_t * tmp = (pt_t *)&priv_t_entry;
            private_time_handler(buffer + len_op, tmp, in);
#ifdef DEBUG
            printf("after private time handler\n");
#endif
            // private_time(tmp->caller_name, tmp->date, tmp->starting_time, tmp->duration);
            printf("-> [Recorded]\n");
            // return 0;
        }
#ifdef TEST
        else if (strcmp(op, "projectMeeting") == 0) {
            printf("-> [Recorded]\n");
        }
        else if (strcmp(op, "groupStudy") == 0) {
            printf("-> [Recorded]\n");
        }
        else if (strcmp(op, "gathering") == 0) {
            printf("-> [Recorded]\n");
        } else if (strcmp(op, "printSchd") == 0) {
            bool reach = false;
            bool is_all = false;
            for (int i = 0; i < apm_len; i++) {
                if (buffer[i] == ' ') {
                    reach = true;
                }
                else if (buffer[i] != ' ' && reach) {
                    if (buffer[i] == 'A') is_all = true;
                }
            }
            if (is_all) {
                printf("Period: 2023-04-01 to 2023-04-30\n");
                printf("Algorithm used: FCFS\n");
                printf("***Appointment Schedule***\n\n");
                for (int j = 0; j < in->num_user; j++) {
                    user_meta_data meta;
                    user_appointment_data *list;
                    retrieve_user_appointment(in->users[j], &meta, &list);
                    printf("  %s, you have %d appointments.\n", in->users[j], meta.num);
                    printf("Date           Start   End      Type               People\n");
                    
                    printf("============================================================================\n");
                    for (int i = 0; i < meta.num; i++) {
                        if (list[i].people_len == 0) {
                            printf("%lld       %d    %d     %s      %c\n", list[i].date, list[i].start_time, list[i].end_time, list[i].type, '-');
                        }
                        else {
                            printf("%lld       %d    %d     %s      ", list[i].date, list[i].start_time, list[i].end_time, list[i].type);
                            for (int k = 0; k < list[i].people_len; k++) {   
                                printf("%s ", in->users[list[i].people[k]]);
                            }
                            printf("\n");
                        }
                    }
                    printf("                              - End of %s's Schedual -                      \n", in->users[j]);
                    printf("============================================================================\n");
                    free (list);
                }
            }
            
            // printf("-> [Exported file: Ggg_01_FCFS.txt]\n");
        }
        else { // TODO: (strcmp(op, "endProgram") == 0)
            shutdown_child_process();
            printf("-> Bye!\n");
            break;
        }
#endif
    }
    return 0;
}