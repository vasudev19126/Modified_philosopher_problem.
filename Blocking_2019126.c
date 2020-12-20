// Vasudev Singhal
// 2019126

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// no of philosopheres
#define k 20


// This is my_semaphore which includes mutex, wait and value
// Mutex is used for locking and unlocking of thread
// wait is used to sleep a thread
struct my_semaphore
{
    pthread_mutex_t mutex;
    pthread_cond_t wait;
    int value;
};

// this is semaphore for every fork
struct my_semaphore forks[k];
// this is threads for k philosopheres
pthread_t philosopheres[k];
// this is semaphore for bowl 1
struct my_semaphore bowl1;
// this is semaphore for bowl 2
struct my_semaphore bowl2;


// This function is used to intiaize semaphores
// it has two parameters value and sema
// sema is semaphore which has to initialize 
// and value is the inital value of semapore
struct my_semaphore initalize_semaphore (int value, struct my_semaphore *sema){
    // allocating memoery using malloc
    sema = (struct my_semaphore *)malloc(sizeof(struct my_semaphore));
    sema->value = value;
    // initializing mutex and wait with NULL
    pthread_mutex_init(&(sema->mutex),NULL);
    pthread_cond_init(&(sema->wait), NULL);
    return *sema;
}

// This function is used to up particular semaphore
void signal (struct my_semaphore *sema){
    // this is for locking the semaohore
    // this will not allow access to more than one thread
    pthread_mutex_lock(&(sema->mutex));

    // increasing the semaphore value
    sema->value++;
    
    if(sema->value <= 0) {
            pthread_cond_signal(&(sema->wait));
    }

    // this is for unlocking the semaphore
    pthread_mutex_unlock(&(sema->mutex));
}

// This function is used to down particular semaphore
void wait (struct my_semaphore *sema){
    // this is for locking the semaohore
    // this will not allow access to more than one thread
    pthread_mutex_lock(&(sema->mutex));

    // decreasing the semaphore value
    sema->value--;

    // if value less than 0 then thread should wait
    if(sema->value < 0) {
        pthread_cond_wait(&(sema->wait),&(sema->mutex));
    }

    // this is for unlocking the semaphore
    pthread_mutex_unlock(&(sema->mutex));
}

// To print the value attribute of semaphore for debugging
void printvalue(struct my_semaphore *sema){
    // prints the value of semaphore sema
    printf("Value of semaphore is %d", sema->value);
}



void* philospher(int sema);
void thread_create();
void thread_join();

int main(){

    // calling initalize_semaphore function to initialize all semaphores
    // initialzing value will be 1 beacuse at a time only one thread can access them
    bowl1= initalize_semaphore(1, &bowl1);
    bowl2 = initalize_semaphore(1, &bowl2);
    for (int i = 0; i < k; i++)
    {
        forks[i]=initalize_semaphore(1, &forks[i]);
    }
    
    // calling thread_create function
    thread_create();

    // calling thread_join function;
    thread_join();
	
    return 0;	
}


// This function is to create k threads i.e. philospheres
void thread_create(){
    for(int i=0; i<k; i++)
    {
        pthread_create(&philosopheres[i], NULL,(void *)philospher, (int *)i);
    }	

}

// This is join all k threads that were created earlier
void thread_join(){
    for(int i=0; i<k; i++)
    {
        pthread_join(philosopheres[i],NULL);
    }
}

void* philospher(int i) 
{ 
    int true = 1;

    //infinte loop
    while (true)
    {
        
        printf("Philosopher %d is thinking\n", i + 1);
        // left fork
        struct my_semaphore left = forks[i];
        // right fork
        struct my_semaphore right = forks[(i+1)%k];

        // taking left and right fork
        // this condition is used to prevent it from special condition of deadlock 
        // i.e. what if all the philosophers take there left fork then no one will be able to take there right fork
        // to prevent our code from above deadlock we simply take right fork first for our kth philosopher.
        if(i==k-1){
            wait(&right);
            wait(&left);
        }
        else{
             wait(&left);
             wait(&right);

        }
        // taking bowl 1 first and then bowl 2
        wait(&bowl1);
        wait(&bowl2);

        printf("Philosopher %d eats using forks %d and %d.\n",i+1, i+1, (i+1)%k +1);
        
        sleep(2);

        printf("Philosopher %d finished eating\n", i+1);

        // putting bowl 1 and bowl 2
        signal(&bowl1);
        signal(&bowl2);
        

        // putting left and right fork
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