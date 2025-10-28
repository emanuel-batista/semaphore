#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // Para fork()
#include <sys/types.h>  // Para pid_t
#include <sys/wait.h>   // Para wait() - boa prática!

int contador = 0;

int main() {
    printf("CONCEITO DE SEMÁFOROS\n");

    pid_t pid_filho = fork();



    if(pid_filho == -1){
        perror("Não foi possível retornar o processo filho!");
    }else if (pid_filho == 0)
    {
        printf("----Temos o processo filho!----\n");
        for (int i = 0; i < 100000; i++)
        {
            contador++;
        }
        printf("O valor do contador é: %d", contador);

    }else{
        printf("\n---------Temos o processo pai!--------\n");
        for (int i = 0; i < 100000; i++)
        {
            contador++;
        }
        wait(NULL);
        printf("O valor do contador é: %d", contador);
    }
    

    return 0;
}