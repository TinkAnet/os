#include "scheduler_round_robin.h"
#include "ipc_schd.h"

const double beta = 0.15; // More appointments a user have, the less priority will be assigned to his new appointment.

int user_cnt[MAX_USER_NUM];

void RR_schder_insert(schd_t s){
    schder_delete_query(s);
    for(int i = 0; i < cnt; i++){
        if(if_rejected[schd_list[i].id]) continue;;
        if_rejected[schd_list[i].id] = 1;
        ipc_user_insert(0, &schd_list[i]);
    }
    schder_delete();
    ipc_user_insert(s.caller, &s);
    for(int i = 0; i < s.callee_num; i++)
        ipc_user_insert(s.callee[i], &s);

}

bool RR_schder_insert_query(schd_t s){
    if(s.caller == 0)
        s.priv = 0;
    else{


    }
    
}