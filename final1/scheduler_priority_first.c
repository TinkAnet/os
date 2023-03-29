#include "scheduler_priority_first.h"
#include "ipc_user.h"

void PRF_schder_insert(schd_t s){
    s.priv = s.type;
    schder_insert(s);
}

bool PRF_schder_insert_query(schd_t s){
    if(s.caller == 0) s.priv = 0;
    else{
        s.priv = s.type;
    }
    return schder_insert_query(s);
}
