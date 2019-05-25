/* Contador de números primos
 *
 * Este programa recebera uma serie de caracteres representando números em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela a
 * quantidade de números primos que recebeu e, após, encerrar.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>


#define ativo 1
#define livre 0

typedef struct {
  unsigned long value;
  int key;
} thread_args;

pthread_mutex_t trava;
pthread_t threads[4];
int worker_status[4];
int countWorkers=0;
int quantidadePrimos=0;

bool validaPrimo(unsigned long n);

void *worker(void *arg) {
  thread_args *info = (thread_args *)arg;
  pthread_mutex_lock(&trava);
  if(validaPrimo(info->value))
    quantidadePrimos += 1;
  worker_status[info->key] = livre;
  countWorkers--;
  pthread_mutex_unlock(&trava);

  free(arg);

  return NULL;
}

int main() {
  unsigned long n;
  int i;
  while(scanf("%lu", &n)!= EOF) {
    while(countWorkers>=4);
    thread_args *args;
    args = (thread_args *)malloc(sizeof(thread_args));
    int i=0;


    pthread_mutex_lock(&trava);
    args->value = n;
    while(worker_status[i]==ativo) i++;
    args->key = i;
    worker_status[i] = ativo;
    pthread_create(&(threads[i]), NULL, worker, (void *) args);
    pthread_mutex_unlock(&trava);
    countWorkers++;


  }
  //Esperando o termino de todas as threads
  for(i=0 ; i<4 ; i++) {
    if(worker_status[i]==ativo) {
      pthread_join(threads[i], NULL);
    }
  }
  printf("%d\n", quantidadePrimos);
  return 0;
}




bool validaPrimo( unsigned long  n){
  if(n==1){
    return false;
  }
  bool validou=true;
  unsigned long i =n-1;
  while(validou){
    if(i<=1){
      break;
    }
    if(n%i==0){
      validou= false;
    }
    i=i-1;
  }
  return validou;
}
