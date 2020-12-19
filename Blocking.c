// Vasudev Singhal
// 2019126
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct my_semaphore
{
    pthread_mutex_t mutex;
    pthread_cond_t wait;
    int value;
};

struct my_semaphore initalize_semaphore (int value, struct my_semaphore *sema){
    sema = (struct my_semaphore *)malloc(sizeof(struct my_semaphore));
    sema->value = value;
    pthread_mutex_init(&(sema->mutex),NULL);
    pthread_cond_init(&(sema->wait), NULL);
    return *sema;
}

void signal (struct my_semaphore *sema){
    pthread_mutex_lock(&(sema->mutex));
    sema->value++;
    if(sema->value <= 0) {
            pthread_cond_signal(&(sema->wait));
    }
    pthread_mutex_unlock(&(sema->mutex));
}

void wait (struct my_semaphore *sema){
    pthread_mutex_lock(&(sema->mutex));
    sema->value--;
    if(sema->value < 0) {
        pthread_cond_wait(&(sema->wait),&(sema->mutex));
    }
    pthread_mutex_unlock(&(sema->mutex));
}


#define k 20


struct my_semaphore forks[k];
pthread_t philosopheres[k];
struct my_semaphore bowl1;
struct my_semaphore bowl2;

void* philospher(int sema);
void thread_create();
void thread_join();

int main(){

    bowl1= initalize_semaphore(1, &bowl1);
    bowl2 = initalize_semaphore(1, &bowl2);
    for (int i = 0; i < k; i++)
    {
        forks[i]=initalize_semaphore(1, &forks[i]);
    }
	
    return 0;	
}



void thread_create(){
    for(int i=0; i<k; i++)
    {
        pthread_create(&philosopheres[i], NULL,(void *)philospher, (int *)i);
    }	

}

void thread_join(){
    for(int i=0; i<k; i++)
    {
        pthread_join(philosopheres[i],NULL);
    }
}

void* philospher(int s) 
{ 
    while (1)
    {
        int i =s;

        printf("Philosopher %d is thinking\n", i + 1);
        struct my_semaphore left = forks[i];
        struct my_semaphore right = forks[(i+1)%k];

        if(i==k-1){
            wait(&right);
            wait(&left);
        }
        else{
             wait(&left);
             wait(&right);

        }
        
        wait(&bowl1);
        wait(&bowl2);

        printf("Philosopher %d eats using forks %d and %d.\n",i+1, i+1, (i+1)%k +1);
        
        sleep(2);

        printf("Philosopher %d finished eating\n", i+1);

        signal(&bowl1);
        signal(&bowl2);
        

        if(i==k-1){
            signal(&left);
            signal(&right);
        }
        else{
            signal(&right);
            signal(&left);

        }

    }

} 

