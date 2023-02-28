#include "G_user.h"

int num_of_user = 0;
char user_name[11][name_length] = {0};

void init_user(int number, char s[][name_length]){
    num_of_user = number;
    for(int i = 0; i < number; i++){
        for(int j = 0; s[i][j]; j++)
            user[i][j] = s[i][j];
        if(user[i][0] < 'a') user[i][0] = user[i][0] - 'a' + 'A';
    }
}

User load_user(char *s){
    if(s[0] < 'a') s[0] = s[0] - 'a' + 'A';
    User ret;
    ret.uid = -1;
    for(int i = 0; i < num_of_user; i++){
        bool ifsame = true;
        for(int j = 0; s[j] || user_name[i][j]; j++){
            if(s[j] != user_name[i][j]){
                ifsame = false;
                break;
            }
        }
        if(ifsame){
            ret.uid = i;
            break;
        }
    }
    return ret;
}

bool same(User a, User b){
    return a.uid == b.yid;
}

void user_print(User a){
    printf("%s(%d)", user_name[a.uid], a.uid);
}
