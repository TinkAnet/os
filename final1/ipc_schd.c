#include "ipc_schd.h"
#include "common.h"
#include "ipc_user.h"
#include "appointment.h"
#include "scheduler_first_come_first_serve.h"
#include "scheduler_big_meeting_first.h"
#include "scheduler_priority_first.h"
#include "scheduler_round_robin.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>


int schd_pipe_list[SCHD_NUM][2];

typedef struct {
    /**
     * @brief operation code
     * @details
        0 -> server use, shutdown
        1 -> server use, ipc_schd_insert_query
        2 -> client use, ipc_schd_insert_query reply
        3 -> server use, ipc_schd_insert
        4 -> client use, ipc_schd_insert reply
        5 -> server use, ipc_schd_print
        6 -> client use, ipc_schd_print reply
            (the return number is stored at `s`)
     */
    int op;
    schd_t s;
}schd_instruction_t;

static schd_instruction_t instruction;
static int cur_schd_id;

/**
 * @brief load instruction from pipe
 * 
 */
static bool client_read_instruction() {
    int n = read(schd_pipe_list[cur_schd_id * 2][0], 
                &instruction, 
                sizeof(schd_instruction_t));

#ifdef DEBUG
    printf("[Debug] Schd %d received %d bytes with op=%d\n",
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
 * @brief load instruction from pipe
 * 
 */
static bool server_read_instruction() {
    int n = read(schd_pipe_list[cur_schd_id * 2 + 1][0], 
                &instruction, 
                sizeof(schd_instruction_t));

#ifdef DEBUG
    printf("[Debug] App %d received %d bytes with op=%d\n",
        getpid(), n, instruction.op
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
static bool client_write_instruction() {
    int n = write(schd_pipe_list[cur_schd_id * 2+1][1], 
                &instruction, 
                sizeof(schd_instruction_t));

#ifdef DEBUG
    printf("[Debug] Schd %d sent %d bytes with op=%d\n",
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
static bool server_write_instruction() {
    int n = write(schd_pipe_list[cur_schd_id * 2][1], 
                &instruction, 
                sizeof(schd_instruction_t));

#ifdef DEBUG
    printf("[Debug] App %d sent %d bytes with op=%d\n",
        getpid(), n, instruction.op
        );
#endif

    if (n == -1) {
        puts("\nFatal Error: pipe is not avaiable\n");
        exit(EXIT_FAILURE);
    }

    return true;
}

// =================== Start Implementation ==========================
typedef bool (*schd_insert_query_fn_t)(schd_t a);
typedef void (*schd_insert_fn_t)(schd_t a);
typedef int (*schd_print_fn_t)();

static schd_insert_query_fn_t schd_impl_insert_query_list[SCHD_NUM] = {
    // TODO: replace with the actual one
    FCFS_schder_insert_query, PRF_schder_insert_query, BMF_schder_insert_query, RR_schder_insert_query
};
static schd_insert_fn_t schd_impl_insert_list[SCHD_NUM] = {
    // TODO: replace with the actual one
    FCFS_schder_insert, PRF_schder_insert, BMF_schder_insert, RR_schder_insert
};

static void schd_insert_query_callback() {
    bool res = schd_impl_insert_query_list[cur_schd_id](instruction.s);
    instruction.op = 2;
    instruction.s.id = res;
    client_write_instruction();
}

static void schd_insert_callback() {
    schd_impl_insert_list[cur_schd_id](instruction.s);
    instruction.op = 4;
    client_write_instruction();
}

static void schd_print_callback() {
    int n = ipc_user_print(instruction.s.id);
    instruction.op = 6;
    instruction.s.id = n;
    client_write_instruction();
    for (int i = 0; i < n; ++i) {
        write(user_pipe_list[cur_schd_id * 2+1][1], 
            &schd_buffer[i], sizeof(schd_t));
    }
#ifdef DEBUG
    printf("[Debug] Schd %d sent %d schd_t in Print\n", cur_user_id, n);
#endif
}

static void schd_main() {
    while (client_read_instruction()) {
        switch (instruction.op) {
        case 0: return; // shutdown
        case 1: schd_insert_query_callback(); break;
        case 3: schd_insert_callback(); break;
        case 5: schd_print_callback(); break;
        }
    }
}

void ipc_start_schd_process(int schder_id) {
        // prepare pipe
    pipe(schd_pipe_list[schder_id*2]);
    pipe(schd_pipe_list[schder_id*2 + 1]);
    // start
    int c_pid = fork();
    // failed
    if (c_pid < 0) {
        puts("\nFatal Error: fork() failed!\n");
        exit(EXIT_FAILURE);
    }
    // server
    if (c_pid != 0) {
#ifdef DEBUG
        printf("[Debug] Schd %d created, running on pid=%d\n", c_pid);
#endif
        close(schd_pipe_list[schder_id*2][0]);
        close(schd_pipe_list[schder_id*2 + 1][1]);
        return ;
    }
    // child
    if (c_pid == 0) {
        close(schd_pipe_list[schder_id*2][1]);
        close(schd_pipe_list[schder_id*2 + 1][0]);
        
        cur_schd_id = schder_id;
        schd_main();
        
        // collect resources
        close(schd_pipe_list[schder_id*2][0]);
        close(schd_pipe_list[schder_id*2 + 1][1]);
        // exit
        exit(0);
    }
}


void ipc_stop_schd_proces(int schder_id) {
    instruction.op = 0;
    server_write_instruction();
    // collect
    wait(NULL);
    // close pipe
    close(schd_pipe_list[schder_id*2][1]);
    close(schd_pipe_list[schder_id*2 + 1][0]);
}

void ipc_launch_schd() {
    for (int i = 0; i < SCHD_NUM; ++i) {
        ipc_start_schd_process(i);
    }
}

void ipc_shutdown_schd() {
    for (int i = 0; i < SCHD_NUM; ++i) {
        ipc_stop_schd_proces(i);
    }
}

bool ipc_schd_insert_query(int schder_id, schd_t *s) {
    // prepare
    cur_schd_id = schder_id;
    instruction.op = 1;
    instruction.s = *s;
    // send
    server_write_instruction();

    // receive
    server_read_instruction();
    return instruction.s.id;
}

void ipc_schd_insert(int schder_id, schd_t *s) {
    // prepare
    cur_schd_id = schder_id;
    instruction.op = 3;
    instruction.s = *s;
    // send
    server_write_instruction();

    // receive
    server_read_instruction();
    return ;
}

int ipc_schd_print(int schder_id, int user_id) {
    // prepare
    cur_schd_id = schder_id;
    instruction.op = 5;
    instruction.s.id = user_id;
    // send
    server_write_instruction();

    // receive
    server_read_instruction();
    for (int i = 0; i < instruction.s.id; ++i) {
        read(user_pipe_list[user_id*2 + 1][0],
            &schd_buffer[i],
            sizeof(schd_t)
            );
    }
#ifdef DEBUG
    printf("[Debug] Main %d received %d schd_t in print\n", 
            getpid(), instruction.s.id);

#endif
    return instruction.s.id;

}