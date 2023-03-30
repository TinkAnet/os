#ifndef CLI_GUARD
#define CLI_GUARD

#include<stdint.h>
#include<stdlib.h>
#include"input_process.h"

// int bucket[MAX_APPOINTMENT_NUM];
// void init_bucket() {
//     for (int i = 0; i < MAX_APPOINTMENT_NUM; i++) {
//         bucket[i] = -1;
//     }
// }

// /**
//  * @brief appointment id to four types of command
//  * privateTime -> 4
//  * projectMeeting -> 3
//  * groupStudy -> 2
//  * gathering -> 1
// */
// int appid2optype(int appid) {
//     return bucket[appid];
// }


/**
 * @brief after format checking of the command line arguments, run the APO directly.
 * @param cmd_in valid command line arguments
*/
int run(cmd_t* cmd_in);
#endif