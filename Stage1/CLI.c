#include<stdint.h>
#include<stdlib.h>
#include<rinput.h>

int main(int argc, char *argv[]) {
    // argc = 7
    // end of argv is argc-1
    int start_date = atoi(argv[1]);
    int end_date = atoi(argv[2]);
    int num_of_users = argc - 3;
    int dif = 3;
    char* name_list[num_of_users];
    for (int i = 0; i < num_of_users; i++) {
        name_list[i] = argv[i+3];
    }
    
}