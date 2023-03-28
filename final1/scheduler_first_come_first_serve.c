#include "scheduler_first_come_first_serve.h"

void FCFS_schder_start(){

}

bool FCFS_schder_schd(schd_t s){
    s.priv = 1;
    schder_schd(s);
}
