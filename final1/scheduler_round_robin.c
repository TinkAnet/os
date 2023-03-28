#include "scheduler_round_robin.h"
#include "ipc_schd.h"

const double beta = 0.15; // More appointments a user have, the less priority will be assigned to his new appointment.

int user_cnt[MAX_USER_NUM];

void RR_schder_insert(schd_t s);

bool RR_schder_insert_query(schd_t s);