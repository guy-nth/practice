#include <stdio.h>
#include <stdlib.h>

#define READY 1
#define RUN 2
#define WAIT 3

// タスクコントロールブロック
struct TCB{
    int id;
    int state;
    struct TCB *next;
    int (*task)();
};

// キュー構造
struct Queue{
    struct TCB *front;
    struct TCB *back;
};

// セマフォ本体
typedef struct semaphore{
    unsigned int sem_v;
    struct Queue *q_wait;
} sem_t;

// セマフォリスト(Queueを利用したLIFOに近い構造を持つ)
struct SemList{
    sem_t *sem_p;
    struct SemList *next;
};

// Readyキュー
struct Queue *q_ready;
// カレントタスク
struct TCB *current_task;
// セマフォリスト
struct SemList *sem_list;

struct TCB *make_tcb(int id, int (*task)()){
    /*
    構造体TCBを作成する
    */
    // TCBを作成するのに必要なメモリ領域を確保
    struct TCB *tcb = (struct TCB*)malloc(sizeof(struct TCB));
    // mallocのエラー処理
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
    キュー構造を作成する
    */
    // キューを作成するのに必要なメモリ領域を確保
    struct Queue *queue = (struct Queue*)malloc(sizeof(struct Queue));
    // mallocのエラー処理
    if(queue == NULL){
        perror("make_queue - malloc()");
    }
    // メンバ変数を初期化
    init_queue(queue);
    // 作成したキューを渡す(返す)
    return queue;
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
    if(tcb->state == RUN){
        // RUN状態のタスクはReadyキューに挿入
        tcb->state = READY;
    }
    else if(tcb->state == WAIT){
        // WAIT状態のタスクはWAITキューに挿入
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

int sem_init(sem_t *sem, unsigned int val){
    /*
    セマフォの初期化
    semに初期値としてvalを格納する
    また、アイドルタスク用にセマフォリストを作成する
    */
    // メモリ領域を確保
    struct SemList *tmp = (struct SemList*)malloc(sizeof(struct SemList));
    sem->q_wait = make_queue();
    // mallocのエラー処理
    if(tmp == NULL){
        perror("sem_init - malloc()");
    }
    // 初期値の設定
    sem->sem_v = val;
    // セマフォリストの作成
    struct SemList *list_end = sem_list;
    // 新しく追加する要素にセマフォのアドレスを登録
    tmp->sem_p = sem;
    // 最後尾に追加するのでnextは存在しない
    tmp->next = NULL;
    // リスト構造の更新
    if(sem_list == NULL){
        // セマフォリストに何もない(初回)
        sem_list = tmp;
    }
    else{
        // リストの最後尾を探す
        while(list_end->next != NULL){
            list_end = list_end->next;
        }
        list_end->next = tmp;
    }
    return 0;
}

int sem_wait(sem_t *sem){
    /*
    セマフォのデクリメント
    semが0の場合は呼び出し元をブロックする
    */
    if(sem->sem_v == 0){
        // セマフォが0なのでセマフォ固有のWaitキューに移す
        current_task->state = WAIT;
        enqueue(sem->q_wait, current_task);
        return -1;
    }
    else{
        // セマフォが1以上なのでデクリメント
        (sem->sem_v)--;
    }
    return 0;
}

int sem_post(sem_t *sem){
    /*
    セマフォのインクリメント
    Waitキューで待機しているタスクがあれば起こす(Readyキューに挿入する)
    */
    printf("wait(sem = %d)\t", &sem->sem_v);
    print_state(sem->q_wait->front);
    // インクリメント
    (sem->sem_v)++;
    // セマフォ固有のWaitキューからタスクを1つ取り出す
    struct TCB *tmp = dequeue(sem->q_wait);
    if(tmp != NULL){
        // タスクが存在するならReadyキューに移す
        enqueue(q_ready, tmp);
    }
    return 0;
}

int idle_task(void){
    /*
    アイドルタスク
    Readyキューの中身がゼロになったときrun_task内で生成・実行される
    セマフォの数だけWaitキューにあるタスクをReadyキューへ移す
    */
    struct SemList *tmp;
    tmp = sem_list;
    while(tmp != NULL){
        // log
        printf("idle_task is running.\n");
        // セマフォ固有のWaitキューから1つ取り出す
        sem_post(tmp->sem_p);
        // 次のセマフォへ
        tmp = tmp->next;
    }
    return 0;
}

void run_task(void){
    /*
    Readyキューからタスクを1つ取り出して実行する
    */
    // log
    printf("-----\n");
    printf("ready\t\t\t");
    print_state(q_ready->front);
    // キューからタスクを取り出す(取り出したタスクはRUN状態になる)
    current_task = dequeue(q_ready);
    // タスクの実行
    if(current_task != NULL){
        // ユーザタスク
        current_task->task();
        // enqueueする前にタスクの状態を確認する
        // 状態がWAITならスルーする
        if(current_task->state == RUN){
            // RUN状態ならReadyキューへ
            enqueue(q_ready, current_task);
        }
    }
    else{
        // Readyキューにタスクが存在しない(エラー)のでアイドルタスクを呼ぶ
        // アイドルタスクはenqueueしないので使い捨て
        current_task = make_tcb(-1, idle_task);
        current_task->state = RUN;
        current_task->task();
    }
}