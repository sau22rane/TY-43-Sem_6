#include<semaphore.h>
#include<stdio.h>
#include<pthread.h>
#include<time.h>

#define PhilCount 5

sem_t S[PhilCount];
pthread_t p[5];

void *philosopher(void *i){
    while(1){
        printf("\n Philosopher - %d is Thinking", i);
        sleep(rand()%5);

        if( i != PhilCount-1){
            sem_wait(&S[(int)i]);
            sem_wait(&S[((int)i+1)%PhilCount]);
            printf("\n Philosopher - %d picked both sticks", i);
        }
        else{
            sem_wait(&S[((int)i+1)%PhilCount]);
            sem_wait(&S[(int)i]);
            printf("\n Philosopher - %d picked both sticks", i);
        }
            printf("\n Philosopher - %d is Eating", i);
            sleep(rand()%5);

        if(i != PhilCount-1){
            sem_post(&S[(int)i]);
            sem_post(&S[((int)i+1)%PhilCount]);
            printf("\n Philosopher - %d released both sticks", i);
        }
        else{
            sem_post(&S[((int)i+1)%PhilCount]);
            sem_post(&S[(int)i]);
            printf("\n Philosopher - %d released both sticks", i);
        }


        sleep(rand()%5);


        return NULL;
    }
}


int main(){
    srand(time(0));
    for(int i=0;i<PhilCount;i++){
        sem_init(&S[i],0,1);
    }

    pthread_create(&p[0],NULL,philosopher,(void *)0);
    pthread_create(&p[1],NULL,philosopher,(void *)1);
    pthread_create(&p[2],NULL,philosopher,(void *)2);
    pthread_create(&p[3],NULL,philosopher,(void *)3);
    pthread_create(&p[4],NULL,philosopher,(void *)4);

    pthread_join(p[0],NULL);
    pthread_join(p[1],NULL);
    pthread_join(p[2],NULL);
    pthread_join(p[3],NULL);
    pthread_join(p[4],NULL);
    return 0;
}  