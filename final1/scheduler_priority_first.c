#include "scheduler_priority_first.h"

void PRF_schder_start(){

}

bool PRF_schder_schd(schd_t s){
    s.priv = s.type;
    schder_schd(s);
}
