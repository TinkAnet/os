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
#include"CLI.h"

// #define DEBUG
#define TER // print at terminal

pt_t priv_t_entry;
pm_t pgg_entry;
int sequence_number;
int op_id;
int caller_request_accepted;
int user_accept_count[MAX_CALLEE_NUM + 1];
int accepted_act_people;
int user_time_slots[MAX_CALLEE_NUM + 1];

/**
 * @brief user container position = user_id - 1
*/
static int id2index(int x) {
    return x - 1;
}
/**
 * @brief user_id = user container position + 1
*/
static int index2id(int x) {
    return x + 1;
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
 * @brief convert a str to time
 * 
 * @param[in] s: input str 
 * @return tm_t : a time
 * @note The "str" here is not a real "string", but a "longlong" integer.
 * 
 * @par Sample
 * @code
 *      long long s = 201312241930;
 *      tm_t a = str_to_date(s);
 * @endcode
 */
static tm_t str_to_time(long long s){
    tm_t ret;
    ret.str = s;
    ret.date = str_to_date(s/10000);//obtain date
    ret.hour = (s / 100) % 100;//obtain hour
    ret.minute = s % 100;//obtain minute
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
        printf("1. reschedule your current schedule, and your current schedule will be added to reject list.\n");
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
static void print_appointment_schd_heading(const char *username, int n_app, FILE* fd) {
    fprintf(fd, "%7s, you have %d appintments.\n", username, n_app);
    fprintf(fd, "%-13s%-8s%-8s%-15s%-15s\n", "Date", "Start", "End", "Type", "People");
    fprintf(fd, "============================================================\n");
#ifdef TER
    printf("%7s, you have %d appintments.\n", username, n_app);
    printf("%-13s%-8s%-8s%-15s%-15s\n", "Date", "Start", "End", "Type", "People");
    printf("============================================================\n");
#endif
}

/**
 * @brief construct appointment operation content for op_c according to sch->type. 4 -> privateTime 3 -> projectMeeting 2 -> groupStudy 1 -> gathering
*/
static void construct_op_type(char* op_c, schd_t* sch) {
    if (sch->type == 4) strcpy(op_c, "privateTime");
    else if (sch->type == 3) strcpy(op_c, "projectMeeting");
    else if (sch->type == 2) strcpy(op_c, "groupStudy");
    else strcpy(op_c, "gathering");
}

/**
 * @brief print all rejected appointments.
 * @param which_op 0 -> FCFS 1 -> Priority 2 -> Round Robine 3 -> Big Meeting First
*/
static void print_rejected_list(int which_op, FILE* fd, cmd_t *in) {
    int n_reject = ipc_schd_print(which_op, 0);
#ifdef DEBUG
    printf("n_reject = %d\n", n_reject);
#endif
    fprintf(fd, "***Rejected List***\n");
    fprintf(fd, "Altogether there are %d appointments rejected.\n", n_reject);
    fprintf(fd, "============================================================\n");
    char activ_buffer[MAX_OPEARTOR_CHAR];
    for (int i = 0; i < n_reject; i++) {
        memset(activ_buffer, 0, sizeof(activ_buffer));
        schd_t i_app_tmp = schd_buffer[i];
        int act_type = i_app_tmp.type;
        tm_t i_start_time = i_app_tmp.start_time;
        date_t i_start_date = i_start_time.date;
        if (act_type == 4) {
            sprintf(activ_buffer, "privateTime");
            fprintf(fd, "%d. %s %s %lld %lld %-4.1f\n", i + 1, activ_buffer, in->user_container[id2index(i_app_tmp.caller)].name, i_start_date.str, i_start_time.str, i_app_tmp.len);
        } else {
            if (act_type == 3) {
                sprintf(activ_buffer, "projectMeeting");
            } else if (act_type == 2) {
                sprintf(activ_buffer, "groupStudy");
            } else if (act_type == 1) {
                sprintf(activ_buffer, "gathering");
            }
            fprintf(fd, "%d. %s %s %lld %02d%02d %-4.1f ", i + 1, activ_buffer, in->user_container[id2index(i_app_tmp.caller)].name, i_start_date.str, str_to_time(i_start_time.str).hour, str_to_time(i_start_time.str).minute, i_app_tmp.len);
        #ifdef DEBUG
            printf("[Print Reject]: %d\n", i_app_tmp.callee_num);
        #endif
            for (int j = 0; j < i_app_tmp.callee_num; j++) {
                fprintf(fd, "%s ", in->user_container[id2index(i_app_tmp.callee[j])].name);
            }
            fprintf(fd, "\n");
        }
    }
}

/**
 * @brief print performance matric under specific scheduling algorithms. 
*/
static void print_performance(FILE* fd, cmd_t* in) {
    fprintf(fd, "\n\n*** Performance ***\n");
    fprintf(fd, "The total number of people participating in activities under the current scheduling algorithm: %d\n", accepted_act_people);
    fprintf(fd, "Total Number of Requests Received: %d\n", op_id);
    #ifdef DEBUG
    printf("caller_request_accepted = %d op_id = %d\n", caller_request_accepted, op_id);
    #endif
    fprintf(fd, "Total Number of Requests Accepted: %d (%.1f%%)\n", caller_request_accepted, ((double) caller_request_accepted / (double) op_id) * 100);
    fprintf(fd, "Total Number of Requests Rejected: %d (%.1f%%)\n\n\n", op_id - caller_request_accepted, ((double)(op_id - caller_request_accepted) / (double) op_id) * 100);
    
    fprintf(fd, "Number of Requests Accepted by Individual: \n\n");
    for (int i = 0; i < in->num_user; i++) {
        fprintf(fd, "%15s%20s- %d\n", in->user_container[i].name, "", user_accept_count[in->user_container[i].id]);
    }
    fprintf(fd, "\n\n\n");
    fprintf(fd, "Utilization of Time Slot:\n\n");
    #ifdef DEBUG
    int n = sizeof(user_time_slots) / sizeof(user_time_slots[0]);
    for (int i = 0; i < n; i++) {
        printf("%d ", user_time_slots[i]);
    }
    printf("\n");
    #endif
    for (int i = 0; i < in->num_user; i++) {
        fprintf(fd, "%15s%20s- %4.1f%%\n", in->user_container[i].name, "", (double) user_time_slots[in->user_container[i].id] / (double) total_slot_num * 100);
    }
    fprintf(fd, "\n\n");
}

/**
 * @param which_op 0 -> FCFS 1 -> Priority 2 -> Round Robine 3 -> Big Meeting First
*/
static void help_calender_print(cmd_t *in, int which_op, FILE* fd) {
    caller_request_accepted = 0;
    accepted_act_people = 0;
    memset(user_accept_count, 0, sizeof(user_accept_count));
    memset(user_time_slots, 0, sizeof(user_time_slots));
    for (int i = 0; i < in->num_user; i++) { // iterate all users
    #ifdef DEBUG
        printf("[Help Cal] %s\n", in->user_container[i].name);
    #endif
        int this_n_app = ipc_schd_print(which_op, in->user_container[i].id); // find corresponding appointment related to specific scheduling algorithms (which_op).
        print_appointment_schd_heading(in->user_container[i].name, this_n_app, fd);
        for (int j = 0; j < this_n_app; j++) {
            schd_t i_app_tmp = schd_buffer[j];
            tm_t i_start_time = i_app_tmp.start_time;
            tm_t i_end_time = i_app_tmp.end_time;
            date_t i_start_date = i_start_time.date;
            if (i_app_tmp.caller == in->user_container[i].id) {
                caller_request_accepted++;
                accepted_act_people += (i_app_tmp.callee_num + 1);
            }
            user_accept_count[in->user_container[i].id]++;
            user_time_slots[index2id(i)] += i_app_tmp.end_slot - i_app_tmp.start_slot + 1;
            char op_char[MAX_OPEARTOR_CHAR];
            construct_op_type(op_char, &i_app_tmp);
        #ifdef TER
            printf("%d-%02d-%02d%3s%02d:%02d%3s%02d:%02d%3s%-15s", i_start_date.year, i_start_date.month, i_start_date.day, "", i_start_time.hour, i_start_time.minute, "", i_end_time.hour, i_end_time.minute, "", op_char);
        #endif
            fprintf(fd, "%d-%02d-%02d%3s%02d:%02d%3s%02d:%02d%3s%-15s", i_start_date.year, i_start_date.month, i_start_date.day, "", i_start_time.hour, i_start_time.minute, "", i_end_time.hour, i_end_time.minute, "", op_char);
            int n_callee = i_app_tmp.callee_num;
            
            /** caller是他自己, 就开一个user_accpt_count数组, 如果是他自己 user_account_cnt[id] ++ = 成功发起, user_time_slots[] += endslot - starslot + 1, total_slot_num  */
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
                    #ifdef TER
                        printf("%s ",in->user_container[q].name);
                    #endif
                        fprintf(fd, "%s ",in->user_container[q].name);
                    }
                }
            }
            // print initiator if the initiator is not i and i_app_tmp.type != 4 (privateTime)
            if (i_app_tmp.caller != in->user_container[i].id && i_app_tmp.type != 4) {
            #ifdef TER
                printf("%s ", in->user_container[id2index(i_app_tmp.caller)].name);
            #endif
                fprintf(fd, "%s ", in->user_container[id2index(i_app_tmp.caller)].name);
            }
            #ifdef TER
            printf("\n");
            #endif
            fprintf(fd, "\n");
        }
    }
    fprintf(fd, "\n");

    print_rejected_list(which_op, fd, in);
    print_performance(fd, in);
}

/**
 * @param which_op 0 -> FCFS 1 -> Priority 2 -> Round Robine 3 -> Big Meeting First 4 -> All
 * @param fd file to store the print output
*/
static void calendar_print(cmd_t* in, int which_op, FILE* fd) {
    if (which_op != 4) help_calender_print(in, which_op, fd);
    else {
        fprintf(fd, "\nFCFS: \n\n");
        help_calender_print(in, 0, fd);
        fprintf(fd, "\nPriority: \n\n");
        help_calender_print(in, 1, fd);
        fprintf(fd, "\nRound Robine: \n\n");
        help_calender_print(in, 2, fd);
        fprintf(fd, "\nBig Meeting First \n\n");
        help_calender_print(in, 3, fd);
    }
}

/**
 * @brief for printing the init header
 * @details init header example:
 * Period: 2023-04-01 to 2023-04-30
 * Algorithm used: FCFS (variable part)
 * ***Appointment Schedule***
*/
static void init_header_calender_print(cmd_t* in, FILE* infilep, int which_op) {
        date_t st_date = str_to_date(in->start_date);
        date_t en_date = str_to_date(in->end_date);
    #ifdef TER
        printf("Period: %d-%02d-%02d to %d-%02d-%02d\n", st_date.year, st_date.month, st_date.day, en_date.year, en_date.month, en_date.day);
        if (which_op == 4) {
            printf("Algorithm used: FCFS, PRIORITY, ROUND ROBINE, BIG MEETING FIRST\n\n");
        } else if (which_op == 3) {
            printf("Algorithm used: BIG MEETING FIRST\n\n");
        }
        else if (which_op == 2) {
            printf("Algorithm used: ROUND ROBINE\n\n");
        }
        else if (which_op == 1) {
            printf("Algorithm used: PRIORITY\n\n");
        }
        else if (which_op == 0) {
            printf("Algorithm used: FCFS\n\n");
        }
        printf("***Appointment Schedule***\n\n");
    #endif
        fprintf(infilep, "Period: %d-%02d-%02d to %d-%02d-%02d\n", st_date.year, st_date.month, st_date.day, en_date.year, en_date.month, en_date.day);
        if (which_op == 4) {
            fprintf(infilep, "Algorithm used: FCFS, PRIORITY, ROUND ROBINE, BIG MEETING FIRST\n\n");
        } else if (which_op == 3) {
            fprintf(infilep, "Algorithm used: BIG MEETING FIRST\n\n");
        }
        else if (which_op == 2) {
            fprintf(infilep, "Algorithm used: ROUND ROBINE\n\n");
        }
        else if (which_op == 1) {
            fprintf(infilep, "Algorithm used: PRIORITY\n\n");
        }
        else if (which_op == 0) {
            fprintf(infilep, "Algorithm used: FCFS\n\n");
        }
        fprintf(infilep, "***Appointment Schedule***\n\n");
        calendar_print(in, which_op, infilep); // 4 -> ALL
}

int run(cmd_t* in) {
    sequence_number = 0; // init the sequence number
    init_appointment(in->start_date,in->end_date,in->num_user);
    ipc_launch_schd(in->start_date, in->end_date,in->num_user);
    char buffer[BUFFER_SIZE];
    op_id = 0; // appointment id, each appointment has a unique id, once we receive an appointment, let op_id += 1
    FILE* fd_all_req = NULL;
    fd_all_req = fopen("All_Requests.dat", "a");
    if (fd_all_req == NULL) {
        printf("Can't open All_Requests.dat\n");
    }
    while (true) {
        printf("Please enter appointment:\n");
        int apm_len = read(STDIN_FILENO, buffer, BUFFER_SIZE);
        if (apm_len <= 1) {
            printf("EOF or invalid input error!\n");
            continue;
        }
        buffer[--apm_len] = 0; // remove newline character
        fprintf(fd_all_req, "%s\n", buffer);
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
            int valid_private = private_time_handler(buffer + len_op, tmp, in);
            if (valid_private == -1) {
                printf("Input Error exists in privateTime command.\n");
                continue;
            }
            op_id++;
#ifdef DEBUG
            printf("after private time handler\n");
#endif
            schd_t tmp_schedule = load_schd(op_id, tmp->caller, 0, NULL, 4, tmp->starting_day_time, tmp->duration);
            arrange_schd(&tmp_schedule);
            printf("-> [Recorded]\n");
        }
        else if (strcmp(op, "projectMeeting") == 0) {
            pm_t * tmp = (pm_t*)&pgg_entry;
            int valid_pg = project_group_gather_handler(buffer + len_op, tmp, in);
            if (valid_pg == -1) {
                printf("Input Error exists in projectMeeting command.\n");
                continue;
            }
            op_id++;
            schd_t tmp_schedule = load_schd(op_id, tmp->caller, tmp->num_callee, tmp->callee, 3, tmp->starting_day_time, tmp->duration);
            arrange_schd(&tmp_schedule);
            printf("-> [Recorded]\n");
        }
        else if (strcmp(op, "groupStudy") == 0) {
            pm_t * tmp = (pm_t*)&pgg_entry;
            int valid_pg = project_group_gather_handler(buffer + len_op, tmp, in);
            if (valid_pg == -1) {
                printf("Input Error exists in groupStudy command.\n");
                continue;
            }
            op_id++;
            schd_t tmp_schedule = load_schd(op_id, tmp->caller, tmp->num_callee, tmp->callee, 2, tmp->starting_day_time, tmp->duration);
            arrange_schd(&tmp_schedule);
            printf("-> [Recorded]\n");
        }
        else if (strcmp(op, "gathering") == 0) {
            pm_t * tmp = (pm_t*)&pgg_entry;
            int valid_pg = project_group_gather_handler(buffer + len_op, tmp, in);
            if (valid_pg == -1) {
                printf("Input Error exists in gathering command.\n");
                continue;
            }
            op_id++;
            schd_t tmp_schedule = load_schd(op_id, tmp->caller, tmp->num_callee, tmp->callee, 1, tmp->starting_day_time, tmp->duration);
            arrange_schd(&tmp_schedule);
            printf("-> [Recorded]\n");
        }
        else if (strcmp(op, "printSchd") == 0) {
            sequence_number += 1;
            char file_name_buffer[30];
            memset(file_name_buffer, 0, sizeof(file_name_buffer));
            FILE* infilep = NULL;
            if (buffer[len_op] == 'A') { // printSchd ALL
                sprintf(file_name_buffer, "Ggg_%02d_ALL.txt", sequence_number);
                infilep = fopen(file_name_buffer, "a");
                if (infilep == NULL) {
                    printf("Error in opening file!\n");
                    continue;
                }
                init_header_calender_print(in, infilep, 4); // 4 -> ALL
            }
            else if (buffer[len_op] == 'F') { // printSchd FCFS
                sprintf(file_name_buffer, "Ggg_%02d_FCFS.txt", sequence_number);
                infilep = fopen(file_name_buffer, "a");
                if (infilep == NULL) {
                    printf("Error in opening file!\n");
                    continue;
                }
                init_header_calender_print(in, infilep, 0); // 0 -> FCFS
            }
            else if (buffer[len_op] == 'P') { // printSchd PRIORITY
                sprintf(file_name_buffer, "Ggg_%02d_PRIORITY.txt", sequence_number);
                infilep = fopen(file_name_buffer, "a");
                if (infilep == NULL) {
                    printf("Error in opening file!\n");
                    continue;
                }
                init_header_calender_print(in, infilep, 1); // 1 -> PRIORITY
            }
            else if (buffer[len_op] == 'R') { // printSchd ROUND ROBINE
                sprintf(file_name_buffer, "Ggg_%02d_ROUND_ROBINE.txt", sequence_number);
                infilep = fopen(file_name_buffer, "a");
                if (infilep == NULL) {
                    printf("Error in opening file!\n");
                    continue;
                }
                init_header_calender_print(in, infilep, 2); // 2 -> Round Robine
            }
            else if (buffer[len_op] == 'B') { // printSchd BIG MEETING FIRST
                sprintf(file_name_buffer, "Ggg_%02d_BIG_MEETING_FIRST.txt", sequence_number);
                infilep = fopen(file_name_buffer, "a");
                if (infilep == NULL) {
                    printf("Error in opening file!\n");
                    continue;
                }
                init_header_calender_print(in, infilep, 3); // 3 -> BIG MEETING FIRST
            }
            printf("-> [Exported file: %s]\n", file_name_buffer); // print -> [Exported file: Ggg_99_ALL.txt]
            fclose(infilep); // close the file descriptor.
        }
        else if (strcmp(op, "endProgram") == 0) {
            fclose(fd_all_req);
            ipc_shutdown_schd();
            printf("-> Bye!\n");
            break;
        }
    }
    return 0;
}