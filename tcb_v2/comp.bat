gcc -c tcb_ctrl.c
gcc -c task.c
gcc -c main.c
gcc -o sim main.o tcb_ctrl.o task.o