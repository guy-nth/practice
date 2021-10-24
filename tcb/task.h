#ifndef _TASK_H_
#define _TASK_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

extern int SW_STATUS;

void led_ctrl(void);
void sw_check(void);

#endif