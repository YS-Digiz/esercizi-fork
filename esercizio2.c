/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/times.h>
#include <errno.h>

int main()
{
    int fd1 [2];
    int fd2 [2];
    
    if(pipe(fd1) == -1) {
        perror("errore");
        exit(-1);
    }
    if(pipe(fd2) == -1) {
        perror("errore");
        exit(-1);
    }
    
    int num = -1;
    while (num<0) {
        printf("quanti numeri vuoi inserire? \n");
        scanf("%d", &num);
    }

    int numeri[num];
    for (int i = 0; i<num; i++) {
        printf("inserisci il numero %d\n", i+1);
        scanf("%d", &numeri[i]);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("errore fork");
        exit(-1);
    }
    
    if(pid == 0) {
        close (fd1[1]); //1 scrittura
        close (fd2[0]); //0 lettura
        
        int nuovo[num];
        int somma =0;
        int max = -99999999;
        int min = 99999999;
        read(fd1[0], nuovo, sizeof(nuovo));
        for(int i= 0; i<num; i++) {
            somma+=nuovo[i];
            
            if(nuovo [i] > max) {
                max = nuovo[i];
            }
            
            if(nuovo[i]< min) {
                min = nuovo[i];
            }
        }
        
        write(fd2[1], &somma, sizeof(somma));
        write(fd2[1], &max, sizeof(max));
        write(fd2[1], &min, sizeof(min));
        
        close (fd1[0]);
        close (fd2[1]);
        
        exit(0);
        
    } else {
        close (fd1[0]);
        close (fd2[1]);
        
        write(fd1[1], numeri, sizeof(numeri));
        
        
        int somma, min, max;
        read(fd2[0], &somma, sizeof(somma));
        read(fd2[0], &max, sizeof(max));
        read(fd2[0], &min, sizeof(min));
        
        wait(NULL);
        
        printf("somma = %d\n, max= %d\n, min= %d\n", somma,max,min);
        
        close (fd1[1]);
        close (fd2[0]);
    }

    return 0;
}
