#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"
#include <sys/wait.h>

#include "ipc.h"
#include "G_user.h"


/* For Interal Usage */

/**
 * Pipe List
 * 
 * For each pair in this list
 *  - 0: parent -> user
 *  - 1: user   -> parent
 * 
*/
static int num_user;
static int *pipe_list;
static char *user_name_list[];

/**
 * Request Payload & Response Code
 * 
 * Function id table
 * - 1: private time
 * - 2: retrieve user information
 * - 3: shutdown the child process
 * 
 * Response Code
 * - 1: success
 * - 2: failed
 * 
*/
static struct __request {
    int req_id; // request function id
}_req;

/**
 * Private Time Payload
 * 
*/
static struct __private_time_payload {
    int event_date; // event time 
    int event_time; // event time
    double event_duration; // event duration
}_private_time_req;


/**
 * Private Time Callback
 * 
*/
static void private_time_callback(int user_id) {
    // load
    int fd_in = pipe_list[(user_id<<1)];
    read(fd_in, &_private_time_req, sizeof(struct __private_time_payload));
    
    // execute
    _req.req_id = !add_private_time(_private_time_req.event_date, _private_time_req.event_time, _private_time_req.event_duration);

    // return
    int fd_out = pipe_list[(user_id<<1)^1];
    write(fd_out, &_req, sizeof(struct __request));
}

/**
 * retrieve_user Callback
 * 
*/
static void retrieve_user_callback(int user_id) {
    // prepare
    retrieve_my_appointment();
    
    // send
    int fd_out = pipe_list[(user_id<<1)^1];

    // send first erq
    _req.req_id = 1;
    write(fd_out, &_req, sizeof(struct __request));

    // send meta
    write(fd_out, &retrieve_num, sizeof(user_meta_data));
    // send payload
    for (int i = 0; i < retrieve_num.num; ++i) {
        write(fd_out, &retrieve_ap_list[i], sizeof(user_appointment_data));
    }
}

/**
 * System Shutdown Callback
*/
static void process_shutdown_callback(int user_id) {
    // close pipe
    for (int i = 0; i < (user_id << 1); ++i)
        close(pipe_list[i]);
    
    // exit the system;
    exit(0);
}

/**
 * User Call Back Function
 * 
 * @param user_id the current user_id for pipes
*/
static int user_callback(int user_id) {
    while (1) {
        // load the request
        read(pipe_list[user_id<<1], &_req, sizeof(struct __request));
        
        switch (_req.req_id)
        {
        case 1:
            private_time_callback(user_id);
            break;
        case 2:
            retrieve_user_callback(user_id);
            break;
        case 3:
            process_shutdown_callback(user_id);
            break;
        default:
            break;
        }
    }
}

/* For Interal Usage Ends */


/* For Exteral Usage */
int init_child_process(int start_date, int end_date, int number, char *user_name_list[]) {
    num_user = number;
    user_name_list = user_name_list;
    pipe_list = malloc(sizeof(int*) * number * 2);

    user_storage_init(start_date, end_date, number, user_name_list);
    for (int i = 0; i < number; ++i) {
        // init
        pipe(&pipe_list[i<<1]);
        int c_pid = fork();
        
        // child
        if (c_pid == 0) {
            // start the process
            user_callback(i);
        }

        // failed
        if (c_pid < 1) {
            exit(-1);
        }
    }
}

/**
 * Find User Id in the system
 * 
 * @param user_name user name
 * @return user id
 * 
*/
static int find_user_id(char* user_name) {
    for (int i = 0; i < num_user; ++i) {
        if (strcmp(user_name, user_name_list[i] == 0)) {
            return i;
        }
    }
}

int private_time(char* user_name, int event_date, int event_time, double event_duration) {
    // get user id
    int user_id = find_user_id(user_name);
    
    // send request
    int fd_out = pipe_list[user_id<<1];
    _req.req_id = 1;
    _private_time_req.event_date = event_date;
    _private_time_req.event_time = event_time;
    _private_time_req.event_duration = event_duration;
    write(fd_out, &_req, sizeof(struct __request));
    write(fd_out, &_private_time_req, sizeof(struct __private_time_payload));

    // wait for result
    int fd_in = pipe_list[(user_id<<1)^1];
    read(fd_in, &_req, sizeof(struct __request));

    return !(_req.req_id == 1);
}

int retrieve_user_appointment(char* user_name, user_meta_data *meta, user_appointment_data **list) {
    int user_id = find_user_id(user_name);
    
    // prepare
    int fd_out = pipe_list[user_id<<1];
    _req.req_id = 2;
    write(fd_out, &_req, sizeof(struct __request));

    // receive
    int fd_in = pipe_list[(user_id<<1)^1];
    read(fd_in, &_req, sizeof(struct __request));
    
    // if the request failed
    if (_req.req_id != 1) {
        return 1;
    }

    // parse
    read(fd_out, &_req, sizeof(struct __request));

    // receive meta
    read(fd_out, &retrieve_num, sizeof(user_meta_data));
    meta->num = retrieve_num.num;

    // receive appointment
    user_appointment_data *receive_list = malloc(sizeof(user_appointment_data) * meta->num);
    *list = receive_list;
    for (int i = 0; i < retrieve_num.num; ++i) {
        read(fd_out, &receive_list[i], sizeof(user_appointment_data));
    }

    return 0;
}

int shutdown_child_process() {
    _req.req_id = 3;
    for (int i = 0; i < num_user; ++i) {
        int fd_out = pipe_list[i<<1];
        // send close payload
        write(fd_out, &_req, sizeof(struct __request));
    }
    // close pipe on parent side
    for (int i = 0; i < (num_user << 1); ++i)
        close(pipe_list[i]);
}
/* For Exteral Usage Ends */
