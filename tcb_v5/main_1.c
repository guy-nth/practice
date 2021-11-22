#include <stdio.h>
#include "tcb.ctrl.h"
#include "task.h"

int main(void){
    // キュー
    q_ready = make_queue();
    // タスクに割り当てる関数
    int (*tasks[])() = {TaskA, TaskB, TaskC};

    // idとタスクの割り当ておよびリスト構造の作成
    for(int i = 0; i < (sizeof(tasks) / sizeof(tasks[0])); i++){
        enqueue(q_ready, make_tcb(i, tasks[i]));
    }

    // log
    printf("(id: n, state: x)\nn: 0, 1, 2, ...\nx: 1 = READY, 2 = RUN, 3 = WAIT\n");

    // タスクの実行(タスクを2順させる(タスクを6回実行))
    for(int i = 0; i < (sizeof(tasks) / sizeof(tasks[0]))*2; i++){
        run_task();
    }

    return 0;
}