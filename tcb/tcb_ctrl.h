#ifndef _TCB_CTRL_H_
#define _TCB_CTRL_H_

#include <stdio.h>
#include <stdlib.h>

#define READY 1
#define RUN 2

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

struct TCB *make_tcb(int id, void (*task)());
void init_queue(struct Queue *queue);
struct Queue *make_queue(void);
struct TCB *dequeue(struct Queue *queue);
void enqueue(struct Queue *queue, struct TCB *tcb);
void insert_tcb(struct Queue *queue, struct TCB *tcb);
void print_state(struct TCB *tcb);
void run_task(struct Queue *queue);

#endif