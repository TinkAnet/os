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
#include"appointment.h"

#define DEBUG
#define IPC

static void insert_four_schd(schd_t* sch) {
    ipc_schd_insert(0, sch);
    ipc_schd_insert(1, sch);
    ipc_schd_insert(2, sch);
    ipc_schd_insert(3, sch);
}

/**
 * @brief return how many scheduling algorithms this schedule could use. 
 * @param sch schedule to be tested
 * @return bitmap 0000: no scheduling algorithm available, xxx1: FCFS is available, xx1x: Priority is available, x1xx: Round Robine is available, 1xxx: Big Meeting First is available.
*/
static int num_scheduling_available(schd_t* sch) {
    int res = 0;
    bool ok_0 = ipc_schd_insert_query(0, sch);
    if (ok_0) {
        printf("FCFS scheduling is okey to use!\n"); 
        res += 1;
    }
    else printf("can not use FCFS scheduling!\n");
    bool ok_1 = ipc_schd_insert_query(1, sch);
    if (ok_1) {
        printf("Priority scheduling is okey to use!\n");
        res += 2;
    }
    else printf("can not use Priority scheduling!\n");
    bool ok_2 = ipc_schd_insert_query(2, sch);
    if (ok_1) {
        printf("Round Robine scheduling is okey to use!\n");
        res += 4;
    }
    else printf("can not use Round Robine scheduling!\n");
    bool ok_3 = ipc_schd_insert_query(3, sch);
    if (ok_1) {
        printf("Big Meeting First scheduling is okey to use!\n");
        res += 8;
    }
    else printf("can not use Big Meeting First scheduling!\n");
    return res;
}
/**
 * @brief when user choose to continue with current schedule, program will use available schedule for users. 
*/
static void use_available_scheduling(schd_t* sch, int available_bitmap) {
    if ((available_bitmap & 1) == 1) ipc_schd_insert(0, sch);
    if ((available_bitmap & 2) == 2) ipc_schd_insert(1, sch);
    if ((available_bitmap & 4) == 4) ipc_schd_insert(2, sch);
    if ((available_bitmap & 8) == 8) ipc_schd_insert(3, sch);
}
/**
 * @brief reschedule operation
*/
static void reschedule_op(schd_t* sch_to_resch) {
    schd_t reschedule = re_schd(*sch_to_resch);
    if (reschedule.type == -1) {
        printf("Can not find a suitable time to help you reschedule your schedule!\n");
    } else {
        printf("The new schedule is from %d.%02d.%02d %02d:%02d to %d.%02d.%02d %02d:%02d\n",
            reschedule.start_time.date.year, reschedule.start_time.date.month, reschedule.start_time.date.day, reschedule.start_time.hour, reschedule.start_time.minute,
            reschedule.end_time.date.year, reschedule.end_time.date.month, reschedule.end_time.date.day, reschedule.end_time.hour, reschedule.end_time.minute);
        printf("Do you want to use this new schedule? 1->yes,0->no!\n");
        int option;
        scanf("%d", &option);
        if (option == 1) {
            insert_four_schd(&reschedule);
        }
        else {
            switch_to_reject_mode(&reschedule);
        }
    }
}
/**
 * 
 * @param ap_id appointment id
 * @param tmp_pgg project meeting, group study, gathering entry
 * @param type 1 -> privateTime 2 -> projectMeeting 3 -> groupStudy 4 -> gathering
*/
static void arrange_schd(schd_t* sch) {
    int available_bitmap = num_scheduling_available(sch);
    if (available_bitmap == 15) insert_four_schd(sch);
    else {
        int option;
        printf("1. reschedule your current schedule.\n");
        printf("2. continue with current schedule.\n");
        scanf("%d", &option);
        if (option == 1) {
            reschedule_op(sch);
        }
        else if (option == 2) {
            use_available_scheduling(sch, available_bitmap);
        }
    }
}

int run(cmd_t* in) {
    // init child process
    for (int i = 0; i < in->num_user; i++) {
#ifdef IPC
        ipc_start_schd_process(in->user_container[i].id, in->start_date, in->end_date, in->num_user);
#endif
    }
    char buffer[BUFFER_SIZE];
    int op_id = 0; // appointment id, each appointment has a unique id.
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
        op_id++;
#ifdef DEBUG
        printf("op : %s with len %lu\n", op, strlen(op));
#endif
        if (strcmp(op, "privateTime") == 0) {
            pt_t * tmp = (pt_t *)&priv_t_entry;
            private_time_handler(buffer + len_op, tmp, in);
#ifdef DEBUG
            printf("after private time handler\n");
#endif

            schd_t tmp_schedule = load_schd(op_id, tmp->caller, 0, NULL, 1, tmp->starting_day_time, tmp->duration);
            arrange_schd(&tmp_schedule);
            printf("-> [Recorded]\n");
        }
        else if (strcmp(op, "projectMeeting") == 0) {
            pm_t * tmp = (pm_t*)&project_m_entry;
            project_group_gather_handler(buffer + len_op, tmp, in);
            printf("out of project meeting!\n");
            printf("-> [Recorded]\n");
        }
#ifdef TEST
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