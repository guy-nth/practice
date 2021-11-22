#ifndef TCB_CTRL_H
#define TCB_CTRL_H

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
typedef struct semaphore{
    unsigned int sem_v;
    struct Queue *q_wait;
} sem_t;
struct SemList{
    sem_t *sem_p;
    struct SemList *next;
};

extern struct Queue *q_ready;

struct TCB *make_tcb(int id, int (*task)());
void init_queue(struct Queue *queue);
struct Queue *make_queue(void);
struct TCB *dequeue(struct Queue *queue);
void enqueue(struct Queue *queue, struct TCB *tcb);
void print_state(struct TCB *tcb);
int sem_init(sem_t *sem, unsigned int val);
int sem_wait(sem_t *sem);
int sem_post(sem_t *sem);
int idle_task(void);
void run_task(void);

#endif