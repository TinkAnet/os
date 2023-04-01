#include "scheduler_big_meeting_first.h"
#include "ipc_user.h"

static const double alpha = 0.25; // One more people will increase 0.25 priority for an appointment

void BMF_schder_insert(schd_t s){
    s.priv = s.callee_num * alpha + s.type;
    schder_insert(s);
}

bool BMF_schder_insert_query(schd_t s){
    if(s.type == 0) s.priv = 0;
    else s.priv = s.callee_num * alpha + s.type;
    return schder_insert_query(s);
}
