# Projeto: Conceito de Semáforos (Semaphore)

Descrição
- Dois exemplos em C que ilustram condição de corrida e como semáforos podem resolver o problema.
  - [main.c](main.c): exemplo simples sem sincronização, usando variável global `contador`.
  - [semaforo.c](semaforo.c): uso de memória compartilhada e semáforo POSIX nomeado para sincronizar pai e filho.

Arquivos principais
- [main.c](main.c) — demonstra condição de corrida. Funções/variáveis relevantes: [`main`](main.c), [`contador`](main.c).  
- [semaforo.c](semaforo.c) — implementa sincronização com semáforo e memória compartilhada. Funções/variáveis relevantes: [`main`](semaforo.c), [`contador_compartilhado`](semaforo.c), [`semaforo`](semaforo.c).  
- [.vscode/tasks.json](.vscode/tasks.json) — tarefa de build usando gcc-13.  
- [.vscode/settings.json](.vscode/settings.json)

Como compilar
- Usando a tarefa do VSCode (menu Run/Build) ou diretamente no terminal:
  - Para compilar main.c:
    $ /usr/bin/gcc-13 -g main.c -o main
  - Para compilar semaforo.c:
    $ /usr/bin/gcc-13 -g semaforo.c -o semaforo -lrt -pthread

Observações sobre flags
- sem_open e sem_unlink podem requerer link com librt/pthread em algumas distribuições; se houver erro de link, adicionar `-lrt -pthread`.

Como executar
- Exemplo sem sincronização:
  $ ./main
- Exemplo sincronizado (memória compartilhada + semáforo):
  $ ./semaforo

Resultado esperado
- Em [main.c](main.c) (sem sincronização), o valor final do contador pode será apenas 100000, sem qualquer interação ou memória compartilhada entre os processos pai e filho.
- Em [semaforo.c](semaforo.c), com semáforo POSIX nomeado, espera-se valor final consistente igual a 200000 (100000 incrementos pelo pai + 100000 pelo filho).

Boas práticas e limpeza
- [semaforo.c](semaforo.c) faz unlink do semáforo com `sem_unlink` e remove a memória compartilhada com `shmctl(..., IPC_RMID, ...)`. Assegure execução completa para evitar recursos órfãos.
- Se um semáforo nomeado ficar pendente, remova manualmente ou adapte o nome do semáforo para evitar conflitos.

Notas de depuração
- Depurar com breakpoints pode alterar o comportamento temporal e mascarar condições de corrida; testar execuções sem debugger para medir comportamento real.

Licença
- Uso educacional / experimental.
