#include "ipc_user.h"
#include "appointment.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int user_pipe_list[MAX_USER_NUM*2][2];
schd_t schd_buffer[MAX_APPOINTMENT_NUM];

typedef struct {
    /**
     * @brief operation code
     * @details
     *   0 -> server use, shutdown
     *   1 -> server use, ipc_user_insert_query
     *   2 -> client use, ipc_user_insert_query reply
     *   3 -> server use, ipc_user_insert
     *   4 -> client use, ipc_user_insert reply
     *   5 -> server use, ipc_user_delete_query
     *   6 -> client use, ipc_user_delete_query reply
     *       (the return number is stored at `s`)
     *   7 -> server use, ipc_user_delete
     *   8 -> client use, ipc_user_delete reply
     *   9 -> server use, ipc_user_print
     *   10 -> client use, ipc_user_print reply
     *       (the return number is stored at `s`)
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
static bool client_read_instruction() {
    int n;

CLIENT_READ: 
    n = read(user_pipe_list[cur_user_id * 2][0], 
                &instruction, 
                sizeof(user_instruction_t));
    
    if (n == 0) {
        goto CLIENT_READ;
    }

#ifdef DEBUG
    printf("[Debug - User] Client %d received %d bytes with op=%d from pid=%d (pid=%d)\n",
        cur_user_id, n, instruction.op, instruction.s.sender_pid, getpid()
        );
#endif

    if (n == -1) {
        printf("\nFatal Error: user pipe is not avaiable (pid=%d)\n", getpid());
        exit(EXIT_FAILURE);
    }

    return true;
}

/**
 * @brief load instruction from pipe
 * 
 */
static bool server_read_instruction() {
    int n = read(user_pipe_list[cur_user_id * 2 + 1][0], 
                &instruction, 
                sizeof(user_instruction_t));

#ifdef DEBUG
    printf("[Debug - User] Server %d received %d bytes with op=%d from pid=%d (pid=%d)\n",
        getpid(), n, instruction.op, instruction.s.sender_pid, getpid()
        );
#endif

    if (n == -1) {
        printf("\nFatal Error: user pipe is not avaiable (pid=%d)\n", getpid());

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
    int n = write(user_pipe_list[cur_user_id * 2+1][1], 
                &instruction, 
                sizeof(user_instruction_t));

#ifdef DEBUG
    printf("[Debug - User] Client %d sent %d bytes with op=%d\n",
        cur_user_id, n, instruction.op
        );
#endif

    if (n == -1) {
        printf("\nFatal Error: user pipe is not avaiable (pid=%d)\n", getpid());
        
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
    int n = write(user_pipe_list[cur_user_id * 2][1], 
                &instruction, 
                sizeof(user_instruction_t));

#ifdef DEBUG
    printf("[Debug - User] Server %d sent %d bytes with op=%d\n",
        getpid(), n, instruction.op
        );
#endif

    if (n == -1) {
        printf("\nFatal Error: user pipe is not avaiable (pid=%d)\n", getpid());
        
        exit(EXIT_FAILURE);
    }

    return true;
}

// =================== Start Implementation ==========================

static void user_print_callback() {
    int n = user_print_schd(schd_buffer);
    #ifdef DEBUG
    printf("[Debug - User - Print] retrieve schd_buffer[0]: date=%lld type=%d\n" , schd_buffer[0].start_time.date.str, schd_buffer[0].type);
    #endif
    instruction.op = 10;
    instruction.s.id = n;
    client_write_instruction();
    for (int i = 0; i < n; ++i) {
        write(user_pipe_list[cur_user_id * 2+1][1], 
            &schd_buffer[i], sizeof(schd_t));
    }
#ifdef DEBUG
    printf("[Debug - User] Client %d sent %d schd_t in Print\n", cur_user_id, n);
#endif
}

static void user_delete_callback() {
    user_delete_schd(instruction.s.id);
    instruction.op = 8;
    client_write_instruction();
}

static void user_delete_query_callback() {
    int n = user_delete_query(instruction.s, schd_buffer);
    instruction.op = 6;
    instruction.s.id = n;
    client_write_instruction();
    for (int i = 0; i < n; ++i) {
        write(user_pipe_list[cur_user_id * 2+1][1], 
            &schd_buffer[i], sizeof(schd_t));
    }
#ifdef DEBUG
    printf("[Debug - User] Client %d sent %d schd_t in Delete Query\n", cur_user_id, n);
#endif
}

static void user_insert_callback() {
    #ifdef DEBUG
    printf("[Debug - User - Insert] perform insert: date=%lld type=%d\n" , instruction.s.start_time.date.str, instruction.s.type);
    #endif
    user_insert_schd(instruction.s);
    instruction.op = 4;
    client_write_instruction();
}

static void user_insert_query_callback() {
    bool res = user_query_schd(instruction.s);
    instruction.op = 2;
    instruction.s.id = res;
    client_write_instruction();
}

static void user_main() {
    while (client_read_instruction()) {
        switch (instruction.op) {
        case 0: return; break; // shutdown
        case 1: user_insert_query_callback(); break;
        case 3: user_insert_callback(); break;
        case 5: user_delete_query_callback(); break;
        case 7: user_delete_callback(); break;
        case 9: user_print_callback(); break;
        }
    }
}

void ipc_start_user_process(int user_id, long long start_day, long long end_day, int people_num) {
    bool flag = true;
    // prepare pipe
    flag &= (pipe(user_pipe_list[user_id*2]) == 0);
    flag &= (pipe(user_pipe_list[user_id*2 + 1]) == 0);

    if (!flag) {
        printf("Fatal Error: User pipe() failed (pid=%d)\n", getpid());
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
#ifdef DEBUG
        printf("[Debug - User] Client %d created, running on pid=%d\n", user_id, c_pid);
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
        #ifdef DEBUG
        printf("[Debug - User] User %d terminated, running on pid=%d\n", user_id, getpid());
        #endif

        // collect resources
        close(user_pipe_list[user_id*2][0]);
        close(user_pipe_list[user_id*2 + 1][1]);
        // exit
        exit(0);
    }
}

void ipc_stop_user_process(int user_id) {
    cur_user_id = user_id;
    instruction.op = 0;
    server_write_instruction();
    // collect user process
    wait(NULL);
    // close pipe
    #ifdef DEBUG
        printf("[DEBUG USER STOP] user_id=%d terminate pid=%d\n", user_id,  getpid());
    #endif
    close(user_pipe_list[user_id*2][1]);
    close(user_pipe_list[user_id*2 + 1][0]);
}

void ipc_launch_user(int tot_user_number, long long start_day, long long end_day, int people_num) {
    for (int i = 0; i <= tot_user_number; ++i) {
        ipc_start_user_process(i, start_day, 
            end_day, people_num);
    }
}

void ipc_shutdown_user(int tot_user_number) {
    for (int i = 0; i <= tot_user_number; ++i) {
        ipc_stop_user_process(i);
    }
}

bool ipc_user_insert_query(int user_id, schd_t *s) {
    // prepare
    cur_user_id = user_id;
    instruction.op = 1;
    instruction.s = *s;
    // send
    server_write_instruction();
    
    // receive
    server_read_instruction();
    return instruction.s.id;
}

void ipc_user_insert(int user_id, schd_t *s) {
    // prepare
    cur_user_id = user_id;
    instruction.op = 3;
    instruction.s = *s;
    // send
    server_write_instruction();

    // receive
    server_read_instruction();
    return ;
}

int ipc_user_delete_query(int user_id, schd_t *s) {
    // prepare
    cur_user_id = user_id;
    instruction.op = 5;
    instruction.s = *s;
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
    printf("[Debug - User] Server %d received %d schd_t in Delete Query\n", 
            getpid(), instruction.s.id);

#endif
    return instruction.s.id;
}

void ipc_user_delete(int user_id, int schd_id) {
    // prepare
    cur_user_id = user_id;
    instruction.op = 7;
    instruction.s.id = schd_id;
    server_write_instruction();

    // receive
    server_read_instruction();
    return;
}

int ipc_user_print(int user_id) {
    // prepare
    cur_user_id = user_id;
    instruction.op = 9;
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
    printf("[Debug - User] Server %d received %d schd_t in print\n", 
            getpid(), instruction.s.id);

#endif
    return instruction.s.id;
}
