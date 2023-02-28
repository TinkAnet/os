
#ifndef _USER_H
#define _USER_H
#define name_length 15

struct User{
    int uid;
};

void init_user(int number, char s[][name_length]);
void user_print(User a);
User load_user(char *s);
bool same(User a, User b);

#endif
