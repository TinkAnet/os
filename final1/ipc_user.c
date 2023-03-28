#include "ipc_user.h"
#include "appointment.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int user_pipe_list[MAX_USER_NUM][2];
schd_t schd_buffer[MAX_APPOINTMENT_NUM];

typedef struct {
    /**
     * @brief operation code
     * @details
        0 -> parent use, shutdown
        1 -> parent use, ipc_user_insert_query
        2 -> client use, ipc_user_insert_query reply
        3 -> parent use, ipc_user_insert
        4 -> client use, ipc_user_insert reply
        5 -> parent use, ipc_user_delete_query
        6 -> client use, ipc_user_delete_query reply
            (the return number is stored at `s`)
        7 -> parent use, ipc_user_delete
        8 -> client use, ipc_user_delete reply
        9 -> parent use, ipc_user_print
        10 -> client use, ipc_user_print reply
            (the return number is stored at `s`)
     */
    int op;
    schd_t s;
}user_instruction_t;

static user_instruction_t instruction;
static int cur_user_id;

/**
 * @brief load instruction from pipe
 * 
 */
static bool read_instruction() {
    int n = read(user_pipe_list[cur_user_id * 2][0], 
                &instruction, 
                sizeof(user_instruction_t));

#ifdef DEBUG
    printf("[Debug] User %d received %d bytes with op=%d\n",
        cur_user_id, n, instruction.op
        );
#endif

    if (n == -1) {
        puts("\nFatal Error: pipe is not avaiable\n");
        exit(EXIT_FAILURE);
    }

    return true;
}

/**
 * @brief put instruction into pipe
 * 
 */
static bool write_instruction() {
    int n = write(user_pipe_list[cur_user_id * 2+1][1], 
                &instruction, 
                sizeof(user_instruction_t));

#ifdef DEBUG
    printf("[Debug] User %d sent %d bytes with op=%d\n",
        cur_user_id, n, instruction.op
        );
#endif

    if (n == -1) {
        puts("\nFatal Error: pipe is not avaiable\n");
        exit(EXIT_FAILURE);
    }

    return true;
}

static void user_print_callback() {
    int n = user_print_schd(schd_buffer);
    instruction.op = 10;
    instruction.s.id = n;
    write_instruction();
    for (int i = 0; i < n; ++i) {
        write(user_pipe_list[cur_user_id * 2+1][1], 
            &schd_buffer[i], sizeof(schd_t));
    }
#ifdef DEBUG
    printf("[Debug] User %d sent %d schd_t in Print\n", cur_user_id, n);
#endif
}

static void user_delete_callback() {
    user_delete_schd(instruction.s.id);
    instruction.op = 8;
    write_instruction();
}

static void user_delete_query_callback() {
    int n = user_delete_query(instruction.s, schd_buffer);
    instruction.op = 6;
    instruction.s.id = n;
    write_instruction();
    for (int i = 0; i < n; ++i) {
        write(user_pipe_list[cur_user_id * 2+1][1], 
            &schd_buffer[i], sizeof(schd_t));
    }
#ifdef DEBUG
    printf("[Debug] User %d sent %d schd_t in Delete Query\n", cur_user_id, n);
#endif
}

static void user_insert_callback() {
    user_insert_schd(instruction.s);
    instruction.op = 4;
    write_instruction();
}

static void user_insert_query_callback() {
    bool res = user_query_schd(instruction.s);
    instruction.op = 2;
    instruction.s.id = res;
    write_instruction();
}

static void user_main() {
    while (read_instruction()) {
        switch (instruction.op) {
        case 0: exit(0); break; // shutdown
        case 1: user_insert_query_callback(); break;
        case 3: user_insert_callback(); break;
        case 5: user_delete_query_callback(); break;
        case 7: user_delete_callback(); break;
        case 9: user_print_callback(); break;
        }
    }
}

void ipc_start_user_process(int user_id, long long start_day, long long end_day, int people_num) {
    // prepare pipe
    pipe(user_pipe_list[user_id*2]);
    pipe(user_pipe_list[user_id*2 + 1]);
    // start
    int c_pid = fork();
    // failed
    if (c_pid < 0) {
        puts("\nFatal Error: fork() failed!\n");
        exit(EXIT_FAILURE);
    }
    // parent
    if (c_pid != 0) {
#ifdef DEBUG
        printf("[Debug] User %d created, running on pid=%d\n", c_pid);
#endif
        close(user_pipe_list[user_id*2][0]);
        close(user_pipe_list[user_id*2 + 1][1]);
        return ;
    }
    // child
    if (c_pid == 0) {
        close(user_pipe_list[user_id*2][1]);
        close(user_pipe_list[user_id*2 + 1][0]);

        // prepare
        cur_user_id = user_id;
        init_appointment(start_day, end_day, people_num);
        // start
        user_main();
        // never reach
        exit(0);
    }
}

