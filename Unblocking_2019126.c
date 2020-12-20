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
struct my_semaphore sauceBowl1;
// this is semaphore for bowl 2
struct my_semaphore sauceBowl2;




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
    while( pthread_mutex_trylock(&(sema->mutex))!=0);
        
    // increasing the semaphore value
    sema->value++;
    

    // this is for unlocking the semaphore
    pthread_mutex_unlock(&(sema->mutex));


}




// This function is used to down particular semaphore
void wait (struct my_semaphore *sema){


    while (sema->value<=0);

     // this is for locking the semaohore
    // this will not allow access to more than one thread
    while( pthread_mutex_trylock(&(sema->mutex))!=0);
    // decreasing the semaphore value
    sema->value--;
    

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
    sauceBowl1= initalize_semaphore(1, &sauceBowl1);
    sauceBowl2 = initalize_semaphore(1, &sauceBowl2);
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
    while (true)
    {
        // true--;
        printf("Philosopher %d is thinking\n",i+1);
        // left fork
        // struct my_semaphore left = forks[i];
        // // right fork
        // struct my_semaphore right = forks[(i+1)%k];

        if(i==k-1){
            wait(&forks[i]);
            wait(&forks[(i+1)%k]);
        }
        else{
             wait(&forks[(i+1)%k]);
             wait(&forks[i]);

        }

        wait(&sauceBowl1);
        wait(&sauceBowl2);


        printf("Philosopher %d eats using forks %d and %d.\n",i+1, i+1, (i+1)%k +1);
        sleep(1);
        printf("Philosopher %d finished eating\n", i+1);

        signal(&sauceBowl1);
        signal(&sauceBowl2);
         if(i==k-1){
            signal(&forks[i]);
            signal(&forks[(i+1)%k]);
        }
        else{
            signal(&forks[(i+1)%k]);
            signal(&forks[i]);

        }

    }



}