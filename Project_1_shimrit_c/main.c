
#include "pattern_matching.c"
#include "malloc.h"
int main() {
    pm_t* pm_db = (pm_t*) malloc(sizeof(pm_t));
    pm_init(pm_db);
//    unsigned char str[7][7] = {"E", "BE", "BD", "BCD", "CDBCAA", "BCAA", "BEDE"};
    const unsigned char str1[] = "abc";
    const unsigned char str4[] = "aa";
    const unsigned char str2[] = "ababc";
    const unsigned  char str3[] = "dbaa";
    pm_addstring(pm_db, "abc", 4);
    pm_addstring(pm_db, str4, 3);
    pm_addstring(pm_db, str2, 6);
    pm_addstring(pm_db, str3, 5);


    unsigned char* T = "EBEDBCBCAABCD";
    //unsigned char str5[] = "dbaaabc";
//    pm_fsm_search(pm_db.zerostate, str5, (char*)str5);
    pm_makeFSM(pm_db);
    pm_fsm_search(pm_db->zerostate, T, 15);

    return 0;
}
