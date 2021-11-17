#include <stdio.h>
#include <stdlib.h>

#define READY 1
#define RUN 2
#define WAIT 3

struct TCB{
    int id;
    int state;
    struct TCB *next;
    int (*task)();
};

struct Queue{
    struct TCB *front;
    struct TCB *back;
};

// Readyキュー, Waitキュー, カレントタスク
struct Queue *q_ready;
struct Queue *q_wait;
struct TCB *current_task;

struct TCB *make_tcb(int id, int (*task)()){
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
    キューのfrontとbackをNULL埋めする(初期化)
    */
    queue->front = NULL;
    queue->back = NULL;
}

struct Queue *make_queue(void){
    /*
    ReadyキューとWaitキューを作成する
    */
    // キューを作成するのに必要なメモリ領域を確保
    q_ready = (struct Queue*)malloc(sizeof(struct Queue));
    q_wait = (struct Queue*)malloc(sizeof(struct Queue));
    // メンバ変数を初期化
    init_queue(q_ready);
    init_queue(q_wait);
    // 作成したReadyキューのみを渡す(返す)
    return q_ready;
}

struct TCB *dequeue(struct Queue *queue){
    /*
    デキュー処理
    queue.frontの入れ替えを行う(そしてfrontにあった要素を取り出す)
    */
    // 先頭要素を保持
    struct TCB *tmp = queue->front;
    // 先頭要素がNULLならキューにTCBは無い
    if(tmp == NULL){
        return NULL;
    }
    // frontを付け替え
    queue->front = queue->front->next;
    // 先頭要素のnextをNULL埋め
    tmp->next = NULL;
    // 先頭要素の状態切替え
    if(queue == q_ready){
        // Readyキューから取り出すならRUN
        tmp->state = RUN;
    }
    // デキュー後の先頭要素がNULLならキューにTCBは無い
    if(queue->front == NULL){
        queue->back = NULL;
    }
    // 先頭要素を渡す(返す)
    return tmp;
}

void enqueue(struct Queue *queue, struct TCB *tcb){
    /*
    エンキュー処理
    queue.backの付け替えを行う
    */
    // backがNULLならqueueにTCBは無い
    if(queue->back == NULL){
        // エンキューするTCBが始めての要素
        queue->back = tcb;
        queue->front = tcb;
    }
    else{
        // 既にTCBが存在する
        // 現在の後尾に新しい後尾を繋げる
        queue->back->next = tcb;
        // backを付け替え
        queue->back = tcb;
    }
    // 後尾要素のnextをNULL埋め
    tcb->next = NULL;
    // 状態切替え
    if(queue == q_ready){
        // Readyキューに挿入するならREADY
        tcb->state = READY;
    }
    else if(queue == q_wait){
        // Waitキューに挿入するならWAIT
        tcb->state = WAIT;
    }
}

void print_state(struct TCB *tcb){
    /*
    タスクのidとstateを再帰的に表示する
    「/」はNULL(つまりTCBがない)を表す
    */
    if(tcb != NULL){
        printf("(id: %d, state: %d) -> ", tcb->id, tcb->state);
        print_state(tcb->next);
    }
    else{
        // 末尾を「/」にする(NULLを表現)
        printf("/\n");
    }
}

void run_task(void){
    /*
    Readyキューからタスクを1つ取り出して実行する
    */
    // log
    printf("-----\n");
    printf("ready\t");
    print_state(q_ready->front);
    printf("wait\t");
    print_state(q_wait->front);
    // キューからタスクを取り出す(取り出したタスクはRUN状態になる)
    struct TCB *tmp = dequeue(q_ready);
    // 有効なタスクであれば実行する
    if(tmp->task != NULL){
        current_task = tmp;
        tmp->task();
    }
    // enqueueする前にタスクの状態を確認する
    if(tmp->state == RUN){
        // RUN状態ならReadyキューへ
        enqueue(q_ready, tmp);
    }
    else if(tmp->state == WAIT){
        // WAIT状態ならスリープ中なのでWaitキューへ
        enqueue(q_wait, tmp);
    }
    // log
    printf("ready\t");
    print_state(q_ready->front);
    printf("wait\t");
    print_state(q_wait->front);
}

// セマフォとわかるように名前を付ける
typedef int sem_t;

int sem_wait(sem_t *sem){
    /*
    セマフォのデクリメント
    semが0の場合は呼び出し元をブロックする
    */
    if(*sem == 0){
        current_task->state = WAIT;
        return -1;
    }
    else{
        (*sem)--;
    }
    return 0;
}

int sem_post(sem_t *sem){
    /*
    セマフォのインクリメント
    Waitキューで待機しているタスクがあれば起こす(Readyキューに挿入する)
    */
    (*sem)++;
    struct TCB *tmp = dequeue(q_wait);
    if(tmp != NULL){
        enqueue(q_ready, tmp);
    }
    return 0;
}