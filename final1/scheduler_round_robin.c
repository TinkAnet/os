#include "scheduler_round_robin.h"
#include "ipc_schd.h"

const double beta = 1; // More appointments a user have, the less priority will be assigned to his new appointment.
const double alpha = 0.5
int user_cnt[MAX_USER_NUM], success_cnt;

void RR_schder_insert(schd_t s){
    s.priv = user_cnt[s.caller] - (success_cnt+0.0) / total_user_num;
    if(s.priv <= beta) s.priv = 0;
    else s.priv = -(s.priv-beta) * alpha;
    
    schder_delete_query(s);
    for(int i = 0; i < cnt; i++){
        if(if_rejected[schd_list[i].id]) continue;;
        if_rejected[schd_list[i].id] = true;
        user_cnt[schd_list[i].caller]--;
        success_cnt--;
        ipc_user_insert(0, &schd_list[i]);
    }
    schder_delete();
    user_cnt[s.caller]++;
    success_cnt++;
    ipc_user_insert(s.caller, &s);
    for(int i = 0; i < s.callee_num; i++)
        ipc_user_insert(s.callee[i], &s);
}

bool RR_schder_insert_query(schd_t s){
    if(s.caller == 0)
        s.priv = -INF;
    else{
        s.priv = user_cnt[s.caller] - (success_cnt+0.0) / total_user_num;
        if(s.priv <= beta) s.priv = 0;
        else s.priv = -(s.priv-beta) * alpha;
    }
    ipc_schd_insert_query(s.caller, &s);
    for(int i = 0; i < s.callee_num; i++)
        ipc_schd_insert_query(s.callee[i], &s);
}