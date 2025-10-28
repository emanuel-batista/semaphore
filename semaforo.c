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

    sem_t *semaforo; // Declaração
    const char *nome_semaforo = "/meu_semaforo";

    semaforo = sem_open(nome_semaforo, O_CREAT, 0644, 1);
    if (semaforo == SEM_FAILED) {
        perror("sem_open failed");
        exit(1);
    }

    pid_t pid_filho = fork();
    if(pid_filho == -1){
        perror("Deu tudo errado no fork!");
    }else if (pid_filho == 0){
        printf("----Temos o processo filho!----\n");
        for (int i = 0; i < 100000; i++){
            sem_wait(semaforo);
            (*contador_compartilhado)++;
            sem_post(semaforo);
        }
        printf("O valor do contador é: %d\n", *contador_compartilhado);
        if (shmdt(contador_compartilhado) == -1) {
            perror("shmdt failed");
            exit(1);
        }

        sem_close(semaforo);
    }else{
        printf("\n---------Temos o processo pai!--------\n");
        for (int i = 0; i < 100000; i++){
            sem_wait(semaforo);
            (*contador_compartilhado)++;
            sem_post(semaforo);
        }
        wait(NULL);
        printf("O valor do contador é: %d\n", *contador_compartilhado);

        if (shmdt(contador_compartilhado) == -1) {
            perror("shmdt failed");
            exit(1);
        }

        sem_unlink(nome_semaforo);
        sem_close(semaforo);
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl failed");
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

     /**
      * Com a implementação dos semáforos, o código agora garante
      * que apenas um processo (pai ou filho) possa acessar e
      * modificar o contador compartilhado de cada vez. Isso
      * elimina a condição de corrida e assegura que o valor
      * final do contador seja consistente e esperado (200000).
      */
}