#include<stdint.h>
#include<stdlib.h>
#include "rinput.h"
#include "ipc.h"
#include<stdio.h>
// #include<rinput.h>
// #include<ipc.h>
#include<stdbool.h>
#include<string.h>
#include <unistd.h>
#include <string.h>
#include"input_process.h"

// #define DEBUG

pt_t priv_t_entry;
int run(cmd_t* in) {
    
    init_child_process(in->start_date, in->end_date, in->num_user, in->users);
    char buffer[BUFFER_SIZE];
    while (true) {
        printf("Please enter appointment:\n");
        int apm_len = read(STDIN_FILENO, buffer, BUFFER_SIZE);
        if (apm_len <= 1) {
            
            /** TODO: EOF or error or invalid input*/
            
        }
        buffer[--apm_len] = 0; // remove newline character
        
#ifdef DEBUG
        printf("buffer : %s", buffer);
#endif
        char op[MAX_OPEARTOR_CHAR];
        int len_op = 0;
        for (int i = 0; i < apm_len; i++) {
            if (buffer[i] != ' ') {
                op[len_op++] = buffer[i];
            }
            else if(buffer[i] == ' ') {
                op[len_op++] = '\0';
                break;
            }
        }
        if (op[len_op] != '\0') op[len_op++] = '\0';
#ifdef DEBUG
        printf("reach here!\n");
        printf("op : %s with len %lu\n", op, strlen(op));
#endif
        if (strcmp(op, "privateTime") == 0) {
            pt_t * tmp = (pt_t *)&priv_t_entry;
            private_time_handler(buffer, tmp);
#ifdef DEBUG
            printf("after private time handler\n");
#endif
            private_time(tmp->caller_name, tmp->date, tmp->starting_time, tmp->duration);
            printf("-> [Recorded]\n");
            // return 0;
        }
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
        /**
         * 如何调用int retrieve_user_appointment(char* user_name, user_meta_data *meta, user_appointment_data **list);
{
    user_meta_data meta; // 一个人有多少appointment
    user_appointment_data *list;
    retrieve_user_appointment(user, &meta, &list);

    meta.num;
    for (int i = 0; i < meta.num; ++i) {
        printf("");
        list[i].date;
    }
}
        */
    }
    return 0;
}