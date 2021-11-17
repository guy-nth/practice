gcc -c tcb_ctrl.c
gcc -c task_1.c
gcc -c main_1.c
gcc -o sim_1 main_1.o task_1.o tcb_ctrl.o
gcc -c task_2.c
gcc -c main_2.c
gcc -o sim_2 main_2.o task_2.o tcb_ctrl.o
del main_1.o task_1.o main_2.o task_2.o tcb_ctrl.o