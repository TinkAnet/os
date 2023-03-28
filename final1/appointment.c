#include "appointment.h"
#include "ipc_user.h"

int days_of_holidays = 6;
date_t holidays[] = {{20230305, 2023, 3, 5}, {20230312, 2023, 3, 12}, {20230319, 2023, 3, 19}, {20230326, 2023, 3, 26}, {20230329, 2023, 3, 29}, {20230329, 2023, 3, 30}};

int cnt = 0;
schd_t schd_list[MAX_APPOINTMENT_NUM];
int day_num;
int user_num;
date_t st_day, ed_day;

void init_appointment(long long start_day, long long end_day, int people_num){
    st_day = str_to_date(start_day);
    ed_day = str_to_date(end_day);
    user_num = people_num;
    day_num = days(ed_day)+1;
}

bool user_query_schd(schd_t a){
    for(int i = 0; i < cnt; i++){
        if(if_schd_conflict(&a, &schd_list[i])){
            if(a.priv <= schd_list[i].priv) return false; // Cannot insert
        }
    }
    return true; // Able to insert
}



void user_insert_schd(schd_t a){
    schd_list[cnt++] = a;
}

int user_delete_query(schd_t a, schd_t *out){
    int p = 0;
    for(int i = 0; i < cnt; i++){
        if(if_schd_conflict(&schd_list[i], &a)){
            out[p++] = schd_list[i];
        }
    }
    return p;
}

void user_delete_schd(int id){
    for(int i = cnt-1; i >= 0; i--){
        if(schd_list[i].id == id){
            cnt--;
            for(int j = i; j < cnt; j++){
                schd_list[j] = schd_list[j+1];
            }
        }
    }
}

int user_print_schd(schd_t *out){
    for(int i = 0; i < cnt; i++){
        out[i] = schd_list[i];
    }
    return cnt;
}

static long long date_to_str(date_t d){
    return d.year * 10000ll + d.month * 100 + d.day;
}

static date_t str_to_date(long long s){
    date_t ret;
    ret.str = s;
    ret.year = s / 10000;// obtain year
    ret.month = (s / 100) % 100;// obtain month
    ret.day = s % 100;// obtain day
    return ret;
}

static int days(date_t a){
    int p = 0;
    for(date_t i = st_day; !eq(i, a); i = date_add_day(i)){
        p++;
        for(int j = 0; j < days_of_holidays; j++){
            if(eq(i, holidays[j])){
                p--;// exclude holiday
                break;
            }
        }
    }
    return p;
}

static date_t date_add_day(date_t a){
    if(a.month == 2 && a.day == 28){
        // leap year judgement
        if((a.year % 4 == 0 && a.year % 100 != 0) || a.year % 400 == 0) a.day = 29;
        else{ a.month = 3; a.day = 1; }
        return a;
    }

    a.day++;

    // days out of range,enter into next month/ next year
    int mth[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(a.day > mth[a.month]){
        a.day = 1;
        a.month++;
        if(a.month > 12){
            a.month = 1;
            a.year++;
        }
    }
    a.str = date_to_str(a);
    return a;
}
//
static tm_t time_add_hm(tm_t a, int h, int m){
    a.minute += m;
    h += a.minute / 60;
    a.minute %= 60;
    a.hour += h;
    while(a.hour >= 24){
        //if hour greater than 24, add a day
        a.hour -= 24;
        a.date = date_add_day(a.date);
    }
    a.str = time_to_str(a);
    return a;
}

static tm_t str_to_time(long long s){
    tm_t ret;
    ret.str = s;
    ret.date = str_to_date(s/10000);//obtain date
    ret.hour = (s / 100) % 100;//obtain hour
    ret.minute = s % 100;//obtain minute
    return ret;
}

static long long time_to_str(tm_t a){
    return date_to_str(a.date) * 10000 + a.hour * 100 + a.minute;
}

static int time_to_slot(tm_t a){
    for(int i = 0; i < days_of_holidays; i++){
        if(eq(a.date, holidays[i])){
            return -1;
        }
    }
    int base = days(a.date);
    return (base * (END_HOUR - START_HOUR) + (a.hour - START_HOUR+1)) * SLOT_PER_HOUR + a.minute/(60/SLOT_PER_HOUR);
}

static int time_minus_minute_to_slot(tm_t a){
    if(a.minute % (60/SLOT_PER_HOUR) == 0)
        return time_to_slot(a)-1;
    else return time_to_slot(a);
}

static tm_t slot_to_time(int s){
    tm_t ret;
    int d = s / (SLOT_PER_HOUR * (END_HOUR - START_HOUR));
    int r = s % (SLOT_PER_HOUR * (END_HOUR - START_HOUR));
    int p = d;

    for(ret.date = st_day; p > 0; ret.date = date_add_day(ret.date)){
        p--;
        for(int j = 0; j < days_of_holidays; j++){
            if(eq(ret.date, holidays[j])){
                p++;// exclude holiday
                break;
            }
        }
    }
    ret.hour = r / SLOT_PER_HOUR - 1 + START_HOUR;
    ret.minute = (r % SLOT_PER_HOUR) * (60 / SLOT_PER_HOUR);

    return ret;
}

//
static bool if_schd_conflict(const schd_t *a, const schd_t *b){
    return !((a->end_slot < b->start_slot) || (a->start_slot > b->end_slot));
}


schd_t load_schd(int id, int caller, int num_of_callee, int *callee, int type, long long start_time, double len){
    schd_t ret;
    ret.id = id;
    ret.caller = caller;
    ret.callee_num = num_of_callee;
    for(int i = 0; i < num_of_callee; i++)
        ret.callee[i] = callee[i];
    ret.type = type;
    ret.start_time = str_to_time(start_time);
    ret.len = len;
    int mm = ceil(len * 60);
    int hh = mm / 60;
    mm %= 60;
    ret.end_time = time_add_hm(ret.start_time, hh, mm);
    ret.start_slot = time_to_slot(ret.start_time);
    ret.end_slot = time_minus_minute_to_slot(ret.end_time);
    return ret;
}

schd_t re_schd(schd_t s){
    schd_t t;
    t.type = 0;
    t.priv = 0;
    int slot_len = ceil(s.len * SLOT_PER_HOUR);
    for(int i = 0; i < day_num; i++){

        int base = i * SLOT_PER_DAY;
        for(int j = 0; base+j+t.end_slot <= SLOT_PER_DAY; j++){

            t.start_slot = base+j+1;
            t.end_slot = base+j+slot_len;
            bool ok = 1;
            ok &= user_query(t.caller, t);
            for(int k = 0; k < s.callee_num && ok; k++){
                ok &= user_query(t.callee[k], t);
            }
            if(ok){
                t.start_time = slot_to_time(t.start_slot);
                t.end_time = slot_to_time(t.end_slot);
                int mm = 60 / SLOT_PER_HOUR;
                t.end_time = time_add_hm(t.end_time, mm / 60, mm % 60);
                return t;
            }
        }
    }
    t.type = -1;
    return t;
}


bool schder_insert_query(schd_t s){
    bool ok = true;
    ok &= user_query(s.caller, s);
    for(int i = 0; i < s.callee_num & ok; i++)
        ok &= user_query(s.callee[i], s);
    return ok;
}

void schder_insert(schd_t s){
    user_insert(s.caller, s);
    for(int i = 0; i < s.callee_num; i++)
        user_insert(s.callee[i], s);
}

bool schder_delete_query(schd_t s){
    cnt = 0;
    int m = user_delete_query(s, schd_buffer);
    for(int i = 0; i < m; i++){
        schd_list[i] = schd_buffer[i];
    }
    cnt = m;
    for(int i = 0; i < s.callee_num; i++){
        m = user_delete_query(s, schd_buffer);
        for(int j = 0; j < m; j++){
            schd_list[cnt+j] = schd_buffer[j];
        }
        cnt += m;
    }
}
void schder_delete() {
    for(int i = 0; i < cnt; i++){
        user_delete(schd_list[i].id);
    }
}

void schder_print(int id, schd_t *out){
    
}

bool schder_schd(schd_t s){
    bool ok = schder_insert_query(s);
    if(ok){
        user_delete(s.caller, s);
        for(int i = 0; i < s.callee_num; i++)
            user_delete(s.callee[i], s);

        schder_insert(s);
        return true;
    }
    else{
        //--------- re schd Part -----------
        return false;
    }
}
