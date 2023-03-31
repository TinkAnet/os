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


int schd_pipe_list[SCHD_NUM*2][2];

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
    printf("[Debug - Schd] Schd %d received %d bytes with op=%d from pid=%d (pid=%d)\n",
        cur_schd_id, n, instruction.op, instruction.s.sender_pid, getpid()
        );
#endif

    if (n == -1) {
        printf("\nFatal Error: schd pipe is not avaiable (pid=%d)\n", getpid());

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
    printf("[Debug - Schd] App %d received %d bytes with op=%d from pid=%d (pid=%d)\n",
        getpid(), n, instruction.op, instruction.s.sender_pid, getpid()
        );
#endif

    if (n == -1) {
        printf("\nFatal Error: schd pipe is not avaiable (pid=%d)\n", getpid());

        exit(EXIT_FAILURE);
    }

    return true;
}


/**
 * @brief put instruction into pipe
 * 
 */
static bool client_write_instruction() {
    instruction.s.sender_pid = getpid();
    int n = write(schd_pipe_list[cur_schd_id * 2+1][1], 
                &instruction, 
                sizeof(schd_instruction_t));

#ifdef DEBUG
    printf("[Debug - Schd] Schd %d sent %d bytes with op=%d (pid=%d)\n",
        cur_schd_id, n, instruction.op, getpid()
        );
#endif

    if (n == -1) {
        printf("\nFatal Error: schd pipe is not avaiable (pid=%d)\n", getpid());

        exit(EXIT_FAILURE);
    }

    return true;
}

/**
 * @brief put instruction into pipe
 * 
 */
static bool server_write_instruction() {
    instruction.s.sender_pid = getpid();


#ifdef DEBUG
    printf("[Debug - Schd] App %d PRE-sent with op=%d (pid=%d)\n",
        getpid(), instruction.op, getpid()
        );
#endif

    int n = write(schd_pipe_list[cur_schd_id * 2][1], 
                &instruction, 
                sizeof(schd_instruction_t));

#ifdef DEBUG
    printf("[Debug - Schd] App %d sent %d bytes with op=%d (pid=%d)\n",
        getpid(), n, instruction.op, getpid()
        );
#endif

    if (n == -1) {
        printf("\nFatal Error: schd pipe is not avaiable (pid=%d)\n", getpid());

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
    FCFS_schder_insert_query, FCFS_schder_insert_query, FCFS_schder_insert_query, FCFS_schder_insert_query
    // FCFS_schder_insert_query, PRF_schder_insert_query, RR_schder_insert_query, BMF_schder_insert_query
};
static schd_insert_fn_t schd_impl_insert_list[SCHD_NUM] = {
    // TODO: replace with the actual one
    FCFS_schder_insert, FCFS_schder_insert, FCFS_schder_insert, FCFS_schder_insert
    // FCFS_schder_insert, PRF_schder_insert, RR_schder_insert, BMF_schder_insert
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
    #ifdef DEBUG
    printf("[Debug - schd - Print] retrieve schd_buffer[0]: date=%lld type=%d\n" , schd_buffer[0].start_time.date.str, schd_buffer[0].type);
    #endif
    instruction.op = 6;
    instruction.s.id = n;
    client_write_instruction();
    for (int i = 0; i < n; ++i) {
        write(schd_pipe_list[cur_schd_id * 2+1][1], 
            &schd_buffer[i], sizeof(schd_t));
    }
#ifdef DEBUG
    printf("[Debug - Schd] Schd %d sent %d schd_t in Print\n", cur_schd_id, n);
#endif
}

static void schd_main() {
    while (client_read_instruction()) {
        switch (instruction.op) {
        case 0: return; break;// shutdown
        case 1: schd_insert_query_callback(); break;
        case 3: schd_insert_callback(); break;
        case 5: schd_print_callback(); break;
        }
    }
}

void ipc_start_schd_process(int schder_id, long long start_day, long long end_day, int people_num) {
    bool flag = true;
    
    // prepare pipe
    flag &= (pipe(schd_pipe_list[schder_id*2])==0);
    flag &= (pipe(schd_pipe_list[schder_id*2 + 1])==0);

    if (!flag) {
        printf("Fatal Error: Schd pipe() failed (pid=%d)\n", getpid());
        exit(EXIT_FAILURE);
    }

    // start
    int c_pid = fork();
    // failed
    if (c_pid < 0) {
        puts("\nFatal Error: fork() failed!\n");
        exit(EXIT_FAILURE);
    }
    // server
    if (c_pid != 0) {
        close(schd_pipe_list[schder_id*2][0]);
        close(schd_pipe_list[schder_id*2 + 1][1]);
        return ;
    }
    // child
    if (c_pid == 0) {
        #ifdef DEBUG
        printf("[Debug - Schd] Schd %d created, running on pid=%d\n", schder_id, getpid());
        #endif

        close(schd_pipe_list[schder_id*2][1]);
        close(schd_pipe_list[schder_id*2 + 1][0]);
        
        cur_schd_id = schder_id;
        ipc_launch_user(people_num, start_day, end_day, people_num);
        schd_main();
        ipc_shutdown_user(people_num);
        #ifdef DEBUG
        printf("[Debug - Schd] Schd %d terminated, running on pid=%d\n", schder_id, getpid());
        #endif

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
    #ifdef DEBUG
    printf("[DEBUG SCHD STOP] schder_id=%d terminate pid=%d\n", schder_id, getpid());
    #endif
    close(schd_pipe_list[schder_id*2][1]);
    close(schd_pipe_list[schder_id*2 + 1][0]);
}

void ipc_launch_schd(long long start_day, long long end_day, int people_num) {
    for (int i = 0; i < SCHD_NUM; ++i) {
        ipc_start_schd_process(i, start_day, end_day, people_num);
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
    #ifdef DEBUG
    printf("[INSERT QUERY] SENT to %d\n", cur_schd_id);
    #endif

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
        read(schd_pipe_list[schder_id*2 + 1][0],
            &schd_buffer[i],
            sizeof(schd_t)
            );
    }
#ifdef DEBUG
    printf("[Debug - Schd] Main %d received %d schd_t in print\n", 
            getpid(), instruction.s.id);
    printf("[Debug - schd] Main retrieve schd_buffer[0]: date=%lld type=%d\n" , schd_buffer[0].start_time.date.str, schd_buffer[0].type);
    fflush(stdout);
#endif
    return instruction.s.id;

}