#include <stdio.h>
#include "tcb_ctrl.h"

// セマフォ
sem_t mutex;

int TaskA(void){
    // プログラムカウンタ
    static int cnt = 0;
    // システムコールの返り値
    int ret;

    // まず始めに実行されるTaskAでセマフォを初期化
    if(cnt == 0){
        ret = sem_init(&mutex, 1);
    }
    // cntの値によって処理を変える
    if(cnt%2 == 0){
        // 0, 2, 4, ...: セマフォの獲得
        ret = sem_wait(&mutex);
    }
    else{
        // 1, 3, 5, ...: セマフォの解放
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

    // cntの値によって処理を変える
    if(cnt%2 == 0){
        // 0, 2, 4, ...: セマフォの獲得
        ret = sem_wait(&mutex);
    }
    else{
        // 1, 3, 5, ...: セマフォの解放
        ret = sem_post(&mutex);
    }

    // sem_waitまたはsem_postを完了できた場合のみ
    if(ret == 0){
        printf("TaskC (cnt = %d)\n", cnt);
        cnt++;
    }
    return ret;
}