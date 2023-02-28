#include<rinput.h>
#include <stdio.h>
#include <stdlib.h>

app_t read_app(const char *input) {
    int n = strlen(input);
    char c;
    char op_str[MAX_OPEARTOR_CHAR] = {'\0'}; // strlen(op_str) = 0
    int op_len = 0;
    int state = 0;
    int cnt_space = 0;
    for (int i = 0; i < n; i++) {
<<<<<<< Updated upstream

=======
        c = input[i];
        if (state == 0 && c != ' ') {
            state = 1; // start to read
        }
        else if (state == 1 && c == ' ') {
            state = 2;
            cnt_space++;
        }
        else if (state == 2 && c == ' ') {
            state = 3; // ill state
            // TODO: illegal input
            // return app_t(NULL,NULL,NULL,NULL);
        }
        else if (state == 2 && c != ' ') {
            state = 1;
        }
    }
    
    state = 0;
    int reach_space = 0;
    for (int i = 0; i < n; i++) {
        c = input[i];
        if (state == 0 && c != ' ') {
            state = 1;
        }
        else if (state == 1 && c == ' ') {
            state = 2; // reach space
            reach_space++;
            continue;
        }
        else if (state == 2 && c != ' ' && reach_space == 1) {
            state = 3; // time to read users' string
        }
        
        if (state == 1) {
            op_str[op_len++] = c;
            continue;
        } else if (state == 3) {
            
        }
        
>>>>>>> Stashed changes
    }
}
