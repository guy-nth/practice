#include <stdio.h>
#include "tcb_ctrl.h"
#include "task.h"

int main(void){
    // キュー
    struct Queue *tcb_ready_queue = make_queue();
    // タスクに割り当てる関数
    void (*tasks[])() = {sw_check, led_ctrl};

    // idとタスクの割り当ておよびリスト構造の作成
    for(int i = 0; i < (sizeof(tasks) / sizeof(tasks[0])); i++){
        insert_tcb(tcb_ready_queue, make_tcb(i, tasks[i]));
    }

    // log
    // printf("(id: n, state: x)\nid: n; 0, 1, 2, ...\nstate: x; 1 = READY, 2 = RUN\n");

    // 初期化
    SW_STATUS = 0;

    // タスクの実行(タスクを2順させる)
    for(int i = 0; i < (sizeof(tasks) / sizeof(tasks[0]))*2; i++){
        run_task(tcb_ready_queue);
    }

    return 0;
}