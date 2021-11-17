#include <stdio.h>
#include "tcb_ctrl.h"

// セマフォ
sem_t mutex = 1;

int TaskA(void){
    // プログラムカウンタ
    static int cnt = 0;
    // システムコールの返り値
    int ret;

    // 0を含む偶数ならセマフォを獲得しにいく, 奇数ならセマフォを解放する
    if(cnt%2 == 0){
        // 0, 2, 4, 6, ...
        ret = sem_wait(&mutex);
    }
    else{
        // 1, 3, 5, 7, ...
        ret = sem_post(&mutex);
    }

    // sem_waitまたはsem_postを完了できた場合のみ
    if(ret == 0){
        printf("TaskA (cnt = %d)\n", cnt);
        cnt++;
    }
    return ret;
}

int TaskB(void){
    // プログラムカウンタ
    static int cnt = 0;

    printf("TaskB (cnt = %d)\n", cnt);
    cnt++;
    return 0;
}

int TaskC(void){
    // プログラムカウンタ
    static int cnt = 0;
    // システムコールの返り値
    int ret;

    // 0を含む偶数ならセマフォを獲得しにいく, 奇数ならセマフォを解放する
    if(cnt%2 == 0){
        // 0, 2, 4, 6, ...
        ret = sem_wait(&mutex);
    }
    else{
        // 1, 3, 5, 7, ...
        ret = sem_post(&mutex);
    }

    // sem_waitまたはsem_postを完了できた場合のみ
    if(ret == 0){
        printf("TaskC (cnt = %d)\n", cnt);
        cnt++;
    }
    return ret;
}