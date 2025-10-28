#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>  // Para ftok() e chaves IPC
#include <sys/shm.h>  // Para shmget() e shmat()
#include <unistd.h>     // Para fork()
#include <sys/types.h>  // Para pid_t
#include <sys/wait.h>

#include <semaphore.h> // Para sem_open, sem_wait, sem_post, etc.
#include <fcntl.h>     // Para as flags O_CREAT

int main(){
    key_t key;
    const char *pathname = "semaforo.c";
    int proj_id = 'B';

    key = ftok(pathname, proj_id);

    if (key == -1) { perror("ftok"); exit(1); }

    int shmid;
    int size = sizeof(int);

    if ((shmid = shmget(key, size, IPC_CREAT | 0666)) == -1) {
        perror("shmget failed");
        exit(1);
    }


    int *contador_compartilhado = (int *) shmat(shmid, NULL, 0);
    if (contador_compartilhado == (int *) -1) {
        perror("shmat failed");
        exit(1);
    }

    *contador_compartilhado = 0;

    //sem_open("/semaforo", O_CREAT, 0644, 1);
    pid_t pid_filho = fork();
    if(pid_filho == -1){
        perror("Deu tudo errado no fork!");
    }else if (pid_filho == 0){
        printf("----Temos o processo filho!----\n");
        for (int i = 0; i < 100000; i++){
            (*contador_compartilhado)++;
        }
        printf("O valor do contador é: %d\n", *contador_compartilhado);
    }else{
        printf("\n---------Temos o processo pai!--------\n");
        for (int i = 0; i < 100000; i++){
            (*contador_compartilhado)++;
        }
        wait(NULL);
        printf("O valor do contador é: %d\n", *contador_compartilhado);

        if (shmdt(contador_compartilhado) == -1) {
            perror("shmdt failed");
            exit(1);
        }
    }

    return 0;

    /** foi observado que, ao debugar o código existe um grande
     * risco de condição de corrida, onde ambos os processos
     * (pai e filho) tentam acessar e modificar o valor do
     * contador ao mesmo tempo, resultando em um valor final
     * inconsistente e inesperado.
     * 
     * para resolver esse problema, é necessário implementar
     * semáforos para garantir que apenas um processo possa
     * acessar e modificar o contador compartilhado de cada vez.
     */
}