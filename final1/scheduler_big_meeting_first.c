#include "scheduler_big_meeting_first.h"
#include "ipc_schd.h"

const double alpha = 0.25; // One more people will increase 0.25 priority for an appointment

void BMF_schder_insert(schd_t s);

bool BMF_schder_insert_query(schd_t s){
    s.priv = (s.callee_num+s.caller) * alpha + s.type;
}
