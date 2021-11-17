#include <stdio.h>
#include "tcb_ctrl.h"

// セマフォ
sem_t mutex_1;
sem_t mutex_2;

int TaskA(void){
    // プログラムカウンタ
    static int cnt = 0;
    // システムコールの返り値
    int ret;

    // まず始めにmutex_1を使用するTaskAでmutex_1を初期化
    if(cnt == 0){
        ret = sem_init(&mutex_1, 1);
    }
    ret = sem_wait(&mutex_1);

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
    // システムコールの返り値
    int ret;

    // まず始めにmutex_2を使用するTaskBでmutex_2を初期化
    if(cnt == 0){
        ret = sem_init(&mutex_2, 1);
    }
    ret = sem_wait(&mutex_2);

    // sem_waitまたはsem_postを完了できた場合のみ
    if(ret == 0){
        printf("TaskB (cnt = %d)\n", cnt);
        cnt++;
    }
    return ret;
}

int TaskC(void){
    // プログラムカウンタ
    static int cnt = 0;
    // システムコールの返り値
    int ret;

    ret = sem_wait(&mutex_1);

    // sem_waitまたはsem_postを完了できた場合のみ
    if(ret == 0){
        printf("TaskC (cnt = %d)\n", cnt);
        cnt++;
    }
    return ret;
}