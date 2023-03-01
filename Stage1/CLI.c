#include<stdint.h>
#include<stdlib.h>
#include "rinput.h"
#include "ipc.h"
#include<stdio.h>
// #include<rinput.h>
// #include<ipc.h>
#include<stdbool.h>
#include<string.h>
// #define DEBUG

pt_t priv_t_entry;

int main(int argc, char *argv[]) {
    // argc = 7
    // end of argv is argc-1
    int start_date = atoi(argv[1]);
    int end_date = atoi(argv[2]);
    int num_of_users = argc - 3;
    int dif = 3;
    char* name_list[num_of_users];
    for (int i = 0; i < num_of_users; i++) {
        name_list[i] = argv[i+3];
    }
    init_child_process(start_date, end_date, num_of_users, name_list);
    char buffer[BUFFER_SIZE];
    while (true) {
        printf("Please enter appointment:\n");
        fgets(buffer, BUFFER_SIZE, stdin);
        int n = strlen(buffer);
#ifdef DEBUG
        printf("buffer : %s", buffer);
#endif
        char op[MAX_OPEARTOR_CHAR];
        int len_op = 0;
        for (int i = 0; i < n; i++) {
            if (buffer[i] != ' ') {
                op[len_op++] = buffer[i];
            }
            else if(buffer[i] == ' ') {
                op[len_op++] = '\0';
                break;
            }
        }
#ifdef DEBUG
        printf("reach here!\n");
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
            for (int i = 0; i < n; i++) {
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
                for (int j = 0; j < num_of_users; j++) {
                    user_meta_data meta;
                    user_appointment_data *list;
                    retrieve_user_appointment(name_list[j], &meta, &list);
                    printf("  %s, you have %d appointments.\n", name_list[j], meta.num);
                    printf("Date           Start   End      Type               People\n");
                    
                    printf("============================================================================\n");
                    for (int i = 0; i < meta.num; i++) {
                        if (list[i].people_len == 0) {
                            printf("%lld       %d    %d     %s      %c\n", list[i].date, list[i].start_time, list[i].end_time, list[i].type, '-');
                        }
                        else {
                            printf("%lld       %d    %d     %s      ", list[i].date, list[i].start_time, list[i].end_time, list[i].type);
                            for (int k = 0; k < list[i].people_len; k++) {   
                                printf("%s ", name_list[list[i].people[k]]);
                            }
                            printf("\n");
                        }
                    }
                    printf("                              - End of %s's Schedual -                      \n", name_list[j]);
                    printf("============================================================================\n");
                    free (list);
                }
            }
            
            // printf("-> [Exported file: Ggg_01_FCFS.txt]\n");
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
}
