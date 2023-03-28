#include "scheduler_first_come_first_serve.h"
#include "ipc_schd.h"

void FCFS_schder_insert(schd_t s){
    s.priv = 0;
    schder_insert(s);
}

bool FCFS_schder_insert_query(schd_t s){
    s.priv = 0;
    return schder_insert_query(s);
}

