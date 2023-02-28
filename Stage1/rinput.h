#ifndef READ_GUARD
#define READ_GUARD


#define MAX_OPEARTOR_CHAR 14 // which is the len of projectMeeting

typedef enum OPEARTOR_TYPE {
    PRIVATE_TIME,   // 0
    PROJECT_MEETING,// 1
    GROUP_STUDY,    // 2
    GATHERING,      // 3
} op_t;

typedef struct APPOINTMENT_ENTRY {
    char* name;
    op_t op;
    int name_len;
    int even_time; // read by atoi
    double even_d; // 
} app_t;

app_t read_app(const char *input);

#endif