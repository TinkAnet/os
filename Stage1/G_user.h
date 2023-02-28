
#ifndef _USER_H
#define _USER_H
#define name_length 15
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "G_Debug.h"

typedef struct USER{
    int uid;
}User;

void init_user(int number, char s[][name_length]);
void user_print(User a);
User load_user(char *s);
bool same(User a, User b);

#endif
