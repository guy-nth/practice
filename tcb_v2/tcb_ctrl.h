#ifndef _TCB_CTRL_H_
#define _TCB_CTRL_H_

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
typedef int sem_t;

struct TCB *make_tcb(int id, int (*task)());
void init_queue(struct Queue *queue);
struct Queue *make_queue(void);
struct TCB *dequeue(struct Queue *queue);
void enqueue(struct Queue *queue, struct TCB *tcb);
void print_state(struct TCB *tcb);
void run_task(void);
int sem_wait(sem_t *sem);
int sem_post(sem_t *sem);

#endif