#include <stdio.h>
#include <stdlib.h>

#define READY 1
#define RUN 2

struct TCB{
    int id;
    int state;
    struct TCB *next;
    void (*task)();
};

struct Queue{
    struct TCB *front;
    struct TCB *back;
};

struct TCB *make_tcb(int id, void (*task)()){
    /*
    構造体TCBを作成する
    */
    // TCBを作成するのに必要なメモリ領域を確保
    struct TCB *tcb = (struct TCB*)malloc(sizeof(struct TCB));
    // malloc()のエラー処理
    if(tcb == NULL){
        perror("make_tcb - malloc()");
    }
    // id, state, next, taskを設定
    tcb->id = id;
    tcb->state = READY;
    tcb->next = NULL;
    tcb->task = task;
    // 作成したTCBを渡す(返す)
    return tcb;
}

void init_queue(struct Queue *queue){
    /*
    キューのfrontとbackをNULL埋めする
    */
    queue->front = NULL;
    queue->back = NULL;
}

struct Queue *make_queue(void){
    /*
    構造体Queueを作成する
    */
    // Queueを作成するのに必要なメモリ領域を確保
    struct Queue *queue = (struct Queue*)malloc(sizeof(struct Queue));
    // メンバ変数を初期化
    init_queue(queue);
    // 作成したQueueを渡す(返す)
    return queue;
}

struct TCB *dequeue(struct Queue *queue){
    /*
    デキュー処理(frontにある要素を取り出す)
    Queue.frontの入れ替えを行う
    */
    // 先頭要素を保持
    struct TCB *tmp = queue->front;
    // frontを付け替え
    queue->front = queue->front->next;
    // 先頭要素のnextをNULL埋め
    tmp->next = NULL;
    // 先頭要素の状態切替え
    tmp->state = RUN;
    // 先頭要素を渡す(返す)
    return tmp;
}

void enqueue(struct Queue *queue, struct TCB *tcb){
    /*
    エンキュー処理(backに要素を入れる)
    Queue.backの付け替えを行う
    */
    // 現在の後尾に新しい後尾を繋げる
    queue->back->next = tcb;
    // backを付け替え
    queue->back = tcb;
    // 後尾要素のnextをNULL埋め
    tcb->next = NULL;
    // 状態切替え
    tcb->state = READY;
}

void insert_tcb(struct Queue *queue, struct TCB *tcb){
    /*
    TCBの連結リストの後尾に新しいTCBを挿入する
    この関数を呼び出す前に引数queueは初期化しておくこと
    */
    // 連結リストの後尾に新しい要素を追加する
    if(queue->front == NULL){
        // front未設定
        queue->front = tcb;
    }
    else{
        // frontが設定されているのであれば、backが存在する
        queue->back->next = tcb;
    }
    // backを更新
    queue->back = tcb;
}

void print_state(struct TCB *tcb){
    /*
    タスクのidとstateを再帰的に表示する
    「-/」はNULL(つまり後に繋がるTCBがない)を表す
    */
    if(tcb != NULL){
        printf("(id: %d, state: %d) -> ", tcb->id, tcb->state);
        print_state(tcb->next);
    }
    else{
        // 末尾の「>」(半角スペース含む)を消して「-/」にする(NULLを表現)
        printf("\b\b/\n");
    }
}

void run_task(struct Queue *queue){
    /*
    キューからタスクを1つ取り出して実行する
    */
    // log
    printf("-----\n");
    // キューからタスクを取り出す(取り出したタスクはRUN状態になる)
    struct TCB *tmp = dequeue(queue);
    // タスクの状態を表示(log)
    // print_state(tmp);
    // print_state(queue->front);
    // 有効なタスクであれば実行
    if(tmp->task != NULL){
        tmp->task();
    }
    // 実行したタスクをキューの後尾へ
    enqueue(queue, tmp);
    // タスクの状態を表示(log)
    // print_state(queue->front);
}