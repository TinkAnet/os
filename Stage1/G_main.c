#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "G_AP.h"

int test(){
    char OPTION;
    scanf("%c", &OPTION);
    if(OPTION == 'd'){
        long long start_date, end_date;
        int number_of_users;
        char user_name[20][name_length];
        scanf("%lld %lld %d", &start_date, &end_date, &number_of_users);
        for(int i = 0; i < number_of_users; i++){
            scanf("%s", user_name[i]);
        }
        init_all(start_date, end_date, number_of_users, user_name);

        while(1){
            int op;
            scanf("%d", &op);
            if(op == 1){
                char level[level_name_length];
                char caller[name_length];
                char callee[callee_max_num][name_length];
                long long date, hm;
                int number;
                double period_time;
                scanf("%s", level);
                scanf("%s", caller);
                scanf("%lld", &date);
                scanf("%lld", &hm);
                scanf("%lf", &period_time);
                scanf("%d", &number);
                for(int i = 0; i < number; i++) scanf("%s", callee[i]);
                Appointment tmp = load_ap(level, caller, date, hm, period_time, number, callee);
                printf("Gen: ");
                ap_print(tmp);
                printf("\n");
            }
        }
    }
}
