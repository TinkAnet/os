#ifndef READ_GUARD
#define READ_GUARD

#define BUFFER_SIZE 255
#define MAX_OPEARTOR_CHAR 15 // which is the len of projectMeeting
#define DATE_LEN 9
#define START_TIME_LEN 5
#define DURATION_LEN 4

typedef enum OPEARTOR_TYPE {
    PRIVATE_TIME,   // 0
    PROJECT_MEETING,// 1
    GROUP_STUDY,    // 2
    GATHERING,      // 3
} op_t;

typedef struct PRIVATE_TIME_OP {
    op_t op;
    char* caller_name;
    int date; // read by atoi
    int starting_time; // 
    double duration; // 
} pt_t;

pt_t priv_t_entry;
void private_time_handler(const char *input, pt_t *res);

#endif