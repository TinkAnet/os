#include "appointment.c"
#include "stdio.h"

int main(){
    //str_to_date()
    printf("Test: str_to_date()\n");
    printf("Defined in line 160\n");
    printf("test case1 : str == 20131224\n");
    long long str = 20131224;
    date_t date = str_to_date(str);
    if(date.str==20131224 && date.year==2013 && date.month==12 && date.day == 24){
        printf("test case1 is correct!\n");
    }else{
        printf("test case1 is wrong! Double check function str_to_date()\n");
    }

    printf("test case2 : str == 20230305\n");
    str = 20230309;
    date = str_to_date(str);
    if(date.str==20230309 && date.year==2023 && date.month==3 && date.day == 9){
        printf("test case2 is correct!\n");
    }else{
        printf("test case2 is wrong! Double check function str_to_date()\n");
    }
    printf("\n");


    //date_add_day()
    printf("Test: date_add_day()\n");
    printf("Defined in line 206\n");
    printf("test case1 : date = {20230331,2023,3,31}\n");
    date.str = 20230331;
    date.year = 2023;
    date.month = 3;
    date.day = 31;
    date_t new = date_add_day(date);
    if(new.year==2023 && new.month == 4 && new.day==1){
        printf("test case1 is correct!\n");
    }else{
        printf("test case1 wrong! Double check function date_add_date()\n");
    }

    printf("test case2 : date = {20231231,2023,12,31}\n");
    date.str = 20231231;
    date.year = 2023;
    date.month = 12;
    date.day = 31;
    new = date_add_day(date);
    if(new.year==2024 && new.month == 1 && new.day==1){
        printf("test case2 is correct!\n");
    }else{
        printf("test case2 wrong! Double check function date_add_date()\n");
    }
    printf("\n");


    //date_to_str()
    printf("Test: date_to_str()\n");
    printf("Defined in line 142\n");
    printf("test case1 : date = {20230331,2023,3,31}\n");
    date.str = 20230331;
    date.year = 2023;
    date.month = 3;
    date.day = 31;
    str = date_to_str(date);
    if(str == 20230331){
        printf("test case1 is correct!\n");
    }else{
        printf("test case1 is wrong! Double check function date_to_str()\n");
    }
    printf("\n");



    //str_to_time
    printf("Test: str_to_time()\n");
    printf("Defined in line 263\n");
    printf("test case1 : str = 202303311800\n");
    tm_t current_time;
    str = 202303311800;
    current_time = str_to_time(str);
    if(current_time.hour==18 && current_time.minute==0 && current_time.date.str == 20230331){
        printf("test case1 is correct!\n");
    }else{
        printf("test case1 is wrong!\n");
    }

    printf("test case2 : str = 202312241931\n");
    str = 202312241931;
    current_time = str_to_time(str);
    if(current_time.hour==19 && current_time.minute==31 && current_time.date.str == 20231224){
        printf("test case1 is correct!\n");
    }else{
        printf("test case1 is wrong!\n");
    }
    printf("\n");

    //time_to_str
    printf("Test: time_to_str()\n");
    printf("Defined in line 279\n");
    printf("test case1 : current_time = {2023,12,24,19,31}\n");
    str = 202312241931;
    current_time = str_to_time(str);
    long long temp_str = time_to_str(current_time);
    if(temp_str==str){
        printf("test case1 is correct!\n");
    }else{
        printf("test case1 is wrong!\n");
    }
    printf("\n");

    //time_add_hm()
    printf("Test: time_add_hm()\n");
    printf("Defined in line 237\n");
    printf("test case1 : current_time = {2023,12,24,19,31}\n");
    tm_t newtime;
    str = 2023122411931;
    current_time = str_to_time(str);
    newtime = time_add_hm(current_time,2,29);
    if(newtime.hour == 22 && newtime.minute==0 && newtime.str == 202312242200){
        printf("test case1 is correct!\n");
    }else{
        printf("test case1 is wrong!\n");
    }

    printf("test case1 : current_time = {2023,3,31,22,36}\n");
    str = 202303312236;
    current_time = str_to_time(str);
    newtime = time_add_hm(current_time,1,30);
    if(newtime.hour == 0 && newtime.minute==6 && newtime.str == 202304010006){
        printf("test case1 is correct!\n");
    }else{
        printf("test case1 is wrong!\n");
    }

    printf("\n");

    //call init_appointment()
    long long start_date = 20230301;
    long long end_date = 20230331;
    int people_num = 8;
    init_appointment(start_date,end_date,people_num);

    //time_to_slot()
    printf("Test: time_to_slot()\n");
    printf("Defined in line 290\n");
    printf("test case1 : current_time = {2023,3,1,19,31}\n");
    str = 202303011931;
    current_time = str_to_time(str);
    int slot_number = time_to_slot(current_time);
    if(slot_number == 2){
        printf("test case1 is correct!\n");
    }else{
        printf("slot number is %d, test case 1 is wrong!\n",slot_number);
    }

    printf("test case2 : current_time = {2023,3,2,20,30}\n");
    str = 202303022030;
    current_time = str_to_time(str);
    slot_number = time_to_slot(current_time);
    if(slot_number == 8){
        printf("test case1 is correct!\n");
    }else{
        printf("slot number is %d, test case 1 is wrong!\n",slot_number);
    }
    printf("\n");




    //load_schd()
    printf("Test: time_add_hm()\n");
    printf("Defined in line 237\n");
    int callee[5];
    for(int i =0;i<5;i++){
        callee[i] = i+2;
    }
    schd_t temp_schd1 = load_schd(1,1,5,callee,1,202303021800,2.0);
    schd_t temp_schd2 = load_schd(2,1,5,callee,3,202303021900,2.0);
    schd_t temp_schd3 = load_schd(3,1,5,callee,2,202303031900,2.0);
    schd_t temp_schd4 = load_schd(4,1,5,callee,1,202303012000,1.5);
    
    printf("test case1\n");
    if(temp_schd1.start_time.str == 202303021800 && temp_schd1.type == 1){
        printf("test case1  is correct.\n");
    }else{
        printf("test case1 is wrong.\n");
    }

    printf("test case2\n");
    if(temp_schd1.start_time.str == 202303021900 && temp_schd1.type == 1){
        printf("test case2  is correct.\n");
    }else{
        printf("test case2 is wrong.\n");
    }
    printf("\n");


    // if_schd_conflict()
    printf("Test: if_schd_conflict()\n");
    printf("Defined in line 346\n");
    printf("test case1 : conflict\n");
    if(if_schd_conflict(&temp_schd1,&temp_schd2)){
        printf("test case1 is correct.\n");
    }else{
        printf("test case1 is wrong.\n");
    }
    printf("test case2 : not conflict\n");
    if(!if_schd_conflict(&temp_schd1,&temp_schd3)){
        printf("test case1 is correct.\n");
    }else{
        printf("test case1 is wrong.\n");
    }
    printf("\n");    



    // call user_insert_schd()
    user_insert_schd(temp_schd1);
    user_insert_schd(temp_schd3);
    user_insert_schd(temp_schd4);

    // user_query_schd(); with all the priv == 0;
    printf("Test: user_query_schd()\n");
    printf("Defined in line 59\n");
    printf("test case1 : cannot insert; with all the priv == 0\n");
    if(!user_query_schd(temp_schd2)){
        printf("test case1 is correct.\n");
    }else{
        printf("test case1 is wrong.\n");
    }

    printf("test case2 : can insert; with all the priv == 0\n");
    schd_t temp_schd5 = load_schd(5,1,5,callee,1,202303052000,1.5);
    if(user_query_schd(temp_schd5)){
        printf("test case2 is correct.\n");
    }else{
        printf("test case2 is wrong.\n");
    }

    printf("test case3 : can insert; with different priv\n");
    temp_schd2.priv = 3;
    if(user_query_schd(temp_schd2)){
        printf("test case3 is correct.\n");
    }else{
        printf("test case3 is wrong.\n");
    }

    printf("test case4 : cannot insert; with different priv\n");
    schd_t temp_schd6 = load_schd(5,1,5,callee,1,202303031900,1.5);
    temp_schd3.priv = temp_schd3.type;
    temp_schd6.priv = temp_schd6.type;
    if(!user_query_schd(temp_schd6)){
        printf("test case4 is correct.\n");
    }else{
        printf("test case4 is wrong.\n");
    }
    printf("\n");


    //user_delete_query()
    schd_t *out;
    int number_dele;
    printf("Test: user_delete_query()\n");
    printf("Defined in line 86\n");
    printf("test case1\n");
    temp_schd1.priv = temp_schd1.type;
    temp_schd2.priv = temp_schd2.type;
    temp_schd3.priv = temp_schd3.type;
    temp_schd4.priv = temp_schd4.type;
    temp_schd5.priv = temp_schd5.type;
    temp_schd6.priv = temp_schd6.type;


    number_dele = user_delete_query(temp_schd2,out);
    if(number_dele==1){
        if(out[0].id == temp_schd1.id){
            printf("test case1 is correct.\n");
        }
    }else{
        printf("test case1 is wrong.\n");
    }

    printf("test case2\n");
    number_dele = user_delete_query(temp_schd6,out);
    if(number_dele==0){
        printf("test case2 is correct.\n");
    }else{
        printf("test case2 is wrong.\n");
    }


    printf("\n"); 
    printf("\n"); 
    printf("Tests on structures are finished.");
    return 0;
}