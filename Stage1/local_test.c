// #include <stdio.h>
// #include <string.h>

// #define MAX_OPEARTOR_CHAR 14 // which is the len of projectMeeting
// #define DATE_LEN 9 // len is 8
// #define START_TIME_LEN 5 // len is 4 
// #define DURATION_LEN 4 // len is 3
// #define name_length 15

// void read_app(const char *input) {
//     int n = strlen(input);
//     char c;
    
//     char op_str[MAX_OPEARTOR_CHAR] = {'\0'}; // strlen(op_str) = 0
//     char user_str[name_length] = {'\0'};
//     char date_str[DATE_LEN] = {'\0'};
//     char start_str[START_TIME_LEN] = {'\0'};
//     char duration_str[DURATION_LEN] = {'\0'};

//     int op_len = 0;
//     int user_len = 0;
//     int date_len = 0;
//     int start_len = 0;
//     int duration_len = 0;
//     int state = 0;
    
//     int reach_space = 0;
//     for (int i = 0; i < n; i++) {
//         c = input[i];
//         if (state == 0 && c != ' ') {
//             state = 1; // read operator
//         }
//         else if (state == 1 && c == ' ') {
//             state = 2; // reach 1th space
//             reach_space++;
//             op_str[op_len++] = '\0';
//             continue;
//         }
//         else if (state == 2 && c != ' ' && reach_space == 1) {
//             state = 3; // time to read users' string
//         }
//         else if (state == 3 && c == ' ') {
//             state = 4; // reach 2th space
//             reach_space++;
//             user_str[user_len++] = '\0';
//             continue;
//         }
//         else if (state == 4 && c != ' ' && reach_space == 2) {
//             state = 5; // read date
//         }
//         else if (state == 5 && c == ' ') {
//             state = 6; // reach 3th space
//             reach_space++;
//             date_str[date_len++] = '\0';
//             continue;
//         }
//         else if (state == 6 && c != ' ' && reach_space == 3) {
//             state = 7; // read starting time
//         }
//         else if (state == 7 && c == ' ') {
//             state = 8; // reach 4th space
//             reach_space++;
//             start_str[start_len++] = '\0';
//             continue;
//         }
//         else if (state == 8 && c != ' ' && reach_space == 4) {
//             state = 9; // read duration
//         }

//         if (state == 1) {
//             op_str[op_len++] = c;
//             continue;
//         } else if (state == 3) {
//             user_str[user_len++] = c;
//             continue;
//         }
//         else if (state == 5) {
//             date_str[date_len++] = c;
//             continue;
//         }
//         else if (state == 7) {
//             start_str[start_len++] = c;
//             continue;
//         }
//         else if (state == 9) {
//             duration_str[duration_len++] = c; // TODO: append '\0'
//             continue;
//         }
//     }
//     printf("op_str -> %s\n", op_str);
//     printf("user_str -> %s\n", user_str);
//     printf("date_str -> %s\n", date_str);
//     printf("start_str -> %s\n", start_str);
//     printf("duration_str -> %s\n", duration_str);
// }

// int main(int argc, char *argv[]) {
//     char* a = "privateTime paul 20230401 1800 2.0";
//     read_app(a);
// }

