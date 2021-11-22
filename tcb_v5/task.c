#include <stdio.h>
#include "tcb.ctrl.h"

// セマフォ
sem_t sem_1;
sem_t sem_2;
sem_t sem_3;

int TaskA(void){
    // プログラムカウンタ
    static int cnt = 0;
    // システムコールの返り値
    int ret;

    // まず始めに実行されるTaskAでセマフォを初期化
    if(cnt == 0){
        ret = sem_init(&sem_1, 1);
    }
    // cntの値によって処理を変える
    if(cnt%2 == 0){
        // 0, 2, 4, ...: セマフォの獲得
        ret = sem_wait(&sem_1);
    }
    else{
        // 1, 3, 5, ...: セマフォの解放
        ret = sem_post(&sem_1);
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
        ret = sem_wait(&sem_1);
    }
    else{
        // 1, 3, 5, ...: セマフォの解放
        ret = sem_post(&sem_1);
    }

    // sem_waitまたはsem_postを完了できた場合のみ
    if(ret == 0){
        printf("TaskC (cnt = %d)\n", cnt);
        cnt++;
    }
    return ret;
}

int TaskX(void){
    // プログラムカウンタ
    static int cnt = 0;
    // システムコールの返り値
    int ret;

    // まず始めにsem_2を使用するTaskAでsem_2を初期化
    if(cnt == 0){
        ret = sem_init(&sem_2, 1);
    }
    ret = sem_wait(&sem_2);

    // sem_waitまたはsem_postを完了できた場合のみ
    if(ret == 0){
        printf("TaskA (cnt = %d)\n", cnt);
        cnt++;
    }
    return ret;
}

int TaskY(void){
    // プログラムカウンタ
    static int cnt = 0;
    // システムコールの返り値
    int ret;

    // まず始めにsem_3を使用するTaskBでsem_3を初期化
    if(cnt == 0){
        ret = sem_init(&sem_3, 1);
    }
    ret = sem_wait(&sem_3);

    // sem_waitまたはsem_postを完了できた場合のみ
    if(ret == 0){
        printf("TaskB (cnt = %d)\n", cnt);
        cnt++;
    }
    return ret;
}

int TaskZ(void){
    // プログラムカウンタ
    static int cnt = 0;
    // システムコールの返り値
    int ret;

    ret = sem_wait(&sem_2);

    // sem_waitまたはsem_postを完了できた場合のみ
    if(ret == 0){
        printf("TaskC (cnt = %d)\n", cnt);
        cnt++;
    }
    return ret;
}