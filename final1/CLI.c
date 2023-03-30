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

// #define DEBUG
#define IPC

pt_t priv_t_entry;
pm_t pgg_entry;

/**
 * @brief user_id = user container position + 1
*/
static int id2index(int x) {
    return x - 1;
}

/**
 * @brief convert a str to a date
 * 
 * @param s : a integer of type long long
 * @return date_t : a date
 * @note The "str" here is not a real "string", but a "longlong" integer.
 * 
 * @par Sample
 * @code 
 *      date_t a;
 *      long long s = 20131224;
 *      a = str_to_date(s);
 *      // a.year == 2013; a.month == 12; a.day == 24;
 * @endcode
 */
static date_t str_to_date(long long s){
    date_t ret;
    ret.str = s;
    ret.year = s / 10000;// obtain year
    ret.month = (s / 100) % 100;// obtain month
    ret.day = s % 100;// obtain day
    return ret;
}

/**
 * @brief used when four scheduling algorithms are okey to use. 
*/
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
    if (ok_2) {
        printf("Round Robine scheduling is okey to use!\n");
        res += 4;
    }
    else printf("can not use Round Robine scheduling!\n");
    bool ok_3 = ipc_schd_insert_query(3, sch);
    if (ok_3) {
        printf("Big Meeting First scheduling is okey to use!\n");
        res += 8;
    }
    else printf("can not use Big Meeting First scheduling!\n");
    return res;
}
/**
 * @brief when user choose to stick to current schedule, program will use available scheduling algorithms for users.
 * @param sch current schedule
*/
static void use_available_scheduling(schd_t* sch) {
     ipc_schd_insert(0, sch);
     ipc_schd_insert(1, sch);
     ipc_schd_insert(2, sch);
     ipc_schd_insert(3, sch);
}

/**
 * @brief reschedule operation
 * @param sch_to_resch schedule to be rescheduled.
*/
static void reschedule_op(schd_t* sch_to_resch) {
    schd_t reschedule = re_schd(*sch_to_resch);
    if (reschedule.type == -1) {
        printf("Can not find a suitable time to help you reschedule your schedule!\n");
    } else {
        printf("The suggested new schedule is from %d.%02d.%02d %02d:%02d to %d.%02d.%02d %02d:%02d\n",
            reschedule.start_time.date.year, reschedule.start_time.date.month, reschedule.start_time.date.day, reschedule.start_time.hour, reschedule.start_time.minute,
            reschedule.end_time.date.year, reschedule.end_time.date.month, reschedule.end_time.date.day, reschedule.end_time.hour, reschedule.end_time.minute);
    }
}

/**
 * @brief arrange schedule, first check num of scheduling algorithms can use, 
 * if could four types of scheduling is available, then insert the schedule with these four algorithms.
 * if there is less than four, then ask user to reschedule or to stick to current schedule. 
 * if user chooses stick to current schedule, use the available algorithms if any. 
 * @param sch schedule just loaded. 
*/
static void arrange_schd(schd_t* sch) {
    int available_bitmap = num_scheduling_available(sch);
    if (available_bitmap == 15) insert_four_schd(sch);
    else {
        int option;
        printf("1. reschedule your current schedule, and your current schedule will be added to reject list.\n"); /** TODO: 当前的放入reject list要提示用户*/
        printf("2. continue with current schedule.\n");
        scanf("%d", &option);
        if (option == 1) {
            switch_to_reject_mode(sch);
            reschedule_op(sch);
            use_available_scheduling(sch); // automatically add the schedule to the reject list.
        }
        else if (option == 2) {
            use_available_scheduling(sch);
        }
    }
}

/**
 * @details the following is an example
 * . John, you have 999 appointments.
 * Date Start End Type People 
 * =========================================================================
*/
static void print_appointment_schd_heading(const char *username, int n_app) {
    printf("%7s, you have %d appintments.\n", username, n_app);
    printf("%-13s%-8s%-8s%-15s%-15s\n", "Date", "Start", "End", "Type", "People");
    printf("============================================================\n");
}

static void construct_op_type(char* op_c, schd_t* sch) {
    if (sch->type == 4) strcpy(op_c, "privateTime");
    else if (sch->type == 3) strcpy(op_c, "projectMeeting");
    else if (sch->type == 2) strcpy(op_c, "groupStudy");
    else strcpy(op_c, "gathering");
}

/**
 * @param which_op 0 -> FCFS 1 -> Priority 2 -> Round Robine 3 -> Big Meeting First 4 -> All
*/
static void help_calender_print(cmd_t *in, int which_op) {
    for (int i = 0; i < in->num_user; i++) { // iterate all users and find according appointment related to specific scheduling algorithms.
        printf("[Help Cal] %s\n", in->user_container[i].name);
        int this_n_app = ipc_schd_print(which_op, in->user_container[i].id); // 0 -> FCFS
        print_appointment_schd_heading(in->user_container[i].name, this_n_app);
        for (int j = 0; j < this_n_app; j++) { 
            schd_t i_app_tmp = schd_buffer[j];
            tm_t i_start_time = i_app_tmp.start_time;
            tm_t i_end_time = i_app_tmp.end_time;
            date_t i_start_date = i_start_time.date;
            char op_char[MAX_OPEARTOR_CHAR];
            construct_op_type(op_char, &i_app_tmp);
            printf("%d-%02d-%02d%3s%02d:%02d%3s%02d:%02d%3s%-15s", i_start_date.year, i_start_date.month, i_start_date.day, "", i_start_time.hour, i_start_time.minute, "", i_end_time.hour, i_end_time.minute, "", op_char);
            int n_callee = i_app_tmp.callee_num;
            for (int k = 0; k < n_callee; k++) {
                int id_callee = i_app_tmp.callee[k];
                for (int q = 0; q < in->num_user; q++) {
                    // in->user_container[q].id != in->user_container[j].id is for excluding mary from People list
                    /**
                     * e.g., john is the projectMeeting initiator
                     *    john, you have 1 appintments.
                     * Date         Start   End     Type           People
                     * ============================================================
                     * 2023-04-02   19:00   21:00   projectMeeting paul mary
                     * 
                     * mary, you have 1 appintments.
                     * Date         Start   End     Type           People
                     * ============================================================
                     * 2023-04-02   19:00   21:00   projectMeeting paul john (not mary here)
                    */
                    if (in->user_container[q].id == id_callee 
                        && in->user_container[i].id != id_callee) {
                        printf("%s ",in->user_container[q].name);
                    }
                }
            }
            // printf("[Help Cal] caller_id : %d\n", i_app_tmp.caller);
            // printf("[Help Cal] caller : %s\n", in->user_container[i_app_tmp.caller -1].name);
            // print initiator if the initiator is not i and i_app_tmp.type != 4 (privateTime)
            if (i_app_tmp.caller != in->user_container[i].id && i_app_tmp.type != 4) {
                printf("%s ", in->user_container[id2index(i_app_tmp.caller)].name);
            }
            printf("\n");
        }   
    }
}
/**
 * @param which_op 0 -> FCFS 1 -> Priority 2 -> Round Robine 3 -> Big Meeting First 4 -> All
*/
static void calendar_print(cmd_t* in, int which_op) {
    if (which_op != 4) help_calender_print(in, which_op);
    else {
        help_calender_print(in, 0);
        help_calender_print(in, 1);
        help_calender_print(in, 2);
        help_calender_print(in, 3);
    }
}

int run(cmd_t* in) {
    ipc_launch_schd(in->start_date,in->end_date,in->num_user);
    char buffer[BUFFER_SIZE];
    int op_id = 0; // appointment id, each appointment has a unique id.
    while (true) {
        printf("Please enter appointment:\n");
        int apm_len = read(STDIN_FILENO, buffer, BUFFER_SIZE);
        if (apm_len <= 1) {
            printf("EOF or invalid input error!\n");
            continue;
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
            int valid_private = private_time_handler(buffer + len_op, tmp, in);
            if (valid_private == -1) {
                printf("Input Error exists in privateTime command.\n");
                continue;
            }
#ifdef DEBUG
            printf("after private time handler\n");
#endif
#ifdef IPC
            schd_t tmp_schedule = load_schd(op_id, tmp->caller, 0, NULL, 4, tmp->starting_day_time, tmp->duration);
            arrange_schd(&tmp_schedule);
#endif
            /** TODO: free(tmp) ??? */
            printf("-> [Recorded]\n");
        }
        else if (strcmp(op, "projectMeeting") == 0) {
            pm_t * tmp = (pm_t*)&pgg_entry;
            int valid_pg = project_group_gather_handler(buffer + len_op, tmp, in);
            if (valid_pg == -1) {
                printf("Input Error exists in projectMeeting command.\n");
                continue;
            }
#ifdef IPC
            schd_t tmp_schedule = load_schd(op_id, tmp->caller, tmp->num_callee, tmp->callee, 3, tmp->starting_day_time, tmp->duration);
            arrange_schd(&tmp_schedule);
#endif
            printf("-> [Recorded]\n");
        }
        else if (strcmp(op, "groupStudy") == 0) {
            pm_t * tmp = (pm_t*)&pgg_entry;
            project_group_gather_handler(buffer + len_op, tmp, in);
#ifdef IPC
            schd_t tmp_schedule = load_schd(op_id, tmp->caller, tmp->num_callee, tmp->callee, 2, tmp->starting_day_time, tmp->duration);
            arrange_schd(&tmp_schedule);
#endif
            printf("-> [Recorded]\n");
        }
        else if (strcmp(op, "gathering") == 0) {
            pm_t * tmp = (pm_t*)&pgg_entry;
            project_group_gather_handler(buffer + len_op, tmp, in);
#ifdef IPC      
            schd_t tmp_schedule = load_schd(op_id, tmp->caller, tmp->num_callee, tmp->callee, 1, tmp->starting_day_time, tmp->duration);
            arrange_schd(&tmp_schedule);
#endif
            printf("-> [Recorded]\n");
        }
        else if (strcmp(op, "printSchd") == 0) {
            if (buffer[len_op] == 'A') { // printSchd ALL
                date_t st_date = str_to_date(in->start_date);
                date_t en_date = str_to_date(in->end_date);
                printf("Period: %d-%02d-%02d to %d-%02d-%02d\n", st_date.year, st_date.month, st_date.day, en_date.year, en_date.month, en_date.day);
                printf("Algorithm used: FCFS, PRIORITY, ROUND ROBINE, BIG MEETING FIRST\n\n");
                printf("***Appointment Schedule***\n\n");
                calendar_print(in, 4);
            }
            else if (buffer[len_op] == 'F') { // printSchd FCFS
                date_t st_date = str_to_date(in->start_date);
                date_t en_date = str_to_date(in->end_date);
                printf("Period: %d-%02d-%02d to %d-%02d-%02d\n", st_date.year, st_date.month, st_date.day, en_date.year, en_date.month, en_date.day);
                printf("Algorithm used: FCFS\n\n");
                printf("***Appointment Schedule***\n\n");
                calendar_print(in, 0);
            }
            else if (buffer[len_op] == 'P') { // printSchd PRIORITY
                date_t st_date = str_to_date(in->start_date);
                date_t en_date = str_to_date(in->end_date);
                printf("Period: %d-%02d-%02d to %d-%02d-%02d\n", st_date.year, st_date.month, st_date.day, en_date.year, en_date.month, en_date.day);
                printf("Algorithm used: PRIORITY\n\n");
                printf("***Appointment Schedule***\n\n");
                calendar_print(in, 1);
            }
            else if (buffer[len_op] == 'R') { // printSchd ROUND ROBINE
                date_t st_date = str_to_date(in->start_date);
                date_t en_date = str_to_date(in->end_date);
                printf("Period: %d-%02d-%02d to %d-%02d-%02d\n", st_date.year, st_date.month, st_date.day, en_date.year, en_date.month, en_date.day);
                printf("Algorithm used: ROUND ROBINE\n\n");
                printf("***Appointment Schedule***\n\n");
                calendar_print(in, 2);
            }
            else if (buffer[len_op] == 'B') { // printSchd BIG MEETING FIRST
                date_t st_date = str_to_date(in->start_date);
                date_t en_date = str_to_date(in->end_date);
                printf("Period: %d-%02d-%02d to %d-%02d-%02d\n", st_date.year, st_date.month, st_date.day, en_date.year, en_date.month, en_date.day);
                printf("Algorithm used: BIG MEETING FIRST\n\n");
                printf("***Appointment Schedule***\n\n");
                calendar_print(in, 3);
            }
        }
        else if (strcmp(op, "endProgram") == 0) {
#ifdef IPC
            ipc_shutdown_schd();
#endif
            printf("-> Bye!\n");
            break;
        }
    }
    return 0;
}