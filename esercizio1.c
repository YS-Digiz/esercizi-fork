#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/times.h>
#include <errno.h>

int main () {
    int somma=0;
    int dati[12] = {3, 7, 2, 9, 4, 6, 8, 1, 5, 10, 2, 7};
    
    for (int i = 0; i<3; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("errore nel fork \n");
            exit(-1);
        }
        if (pid == 0) {
            int somma_parziale = 0;
            for (int j = i*4; j<i*4+3; j++ ) {
                somma_parziale += dati[j];
            }
            printf("[figlio] %d %d, somma parziale = %d \n", i+1, getpid(), somma_parziale);
            exit(somma_parziale);
        } else {
            int status; //ritorno del figlio
            wait(&status);
            if (WIFEXITED(status) == -1) {
                printf("errore");
                exit(-1);
            }
            int numero = WEXITSTATUS(status); //converte status in numero
            somma+=numero;
        }
    }
    
    printf("somma finale %d \n",somma);
    return 0;
}
