#include "scheduler_big_meeting_first.h"

const double alpha = 0.25; // One more people will increase 0.25 priority for an appointment

void BMF_schder_start(){
    
}

bool BMF_schder_schd(schd_t s){
    s.priv = (s.callee_num+s.caller) * alpha + s.type;
    schder_schd(s);
}
