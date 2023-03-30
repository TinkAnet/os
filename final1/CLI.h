#ifndef CLI_GUARD
#define CLI_GUARD

#include<stdint.h>
#include<stdlib.h>
#include"input_process.h"

/**
 * @brief after format checking of the command line arguments, run the APO directly.
 * @param cmd_in valid command line arguments
*/
int run(cmd_t* cmd_in);
#endif