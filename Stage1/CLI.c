#include<stdint.h>
#include<stdlib.h>
#include"rinput.h"
#include<stdio.h>
// #include<rinput.h>
// #include<ipc.h>
#include<stdbool.h>
#include<string.h>
// #define DEBUG

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
    // init_child_process(start_date, end_date, num_of_users, name_list);
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
        if (strcmp(op, "privateTime") == 0) {
            pt_t * tmp = (pt_t *)&priv_t_entry;
            private_time_handler(buffer, tmp);
            
            // private_time(tmp->caller_name, tmp->date, tmp->starting_time, tmp->duration);
            printf("-> [Recorded]\n");
            return 0;
        }
        else if (strcmp(op, "projectMeeting") == 0) {
            
        }
        else if (strcmp(op, "groupStudy") == 0) {
            
        }
        else if (strcmp(op, "gathering") == 0) {
            
        } else {
            
        }
        printf("-> [Recorded]\n");
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
