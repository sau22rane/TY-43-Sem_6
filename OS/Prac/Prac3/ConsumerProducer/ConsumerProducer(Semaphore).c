#include<semaphore.h>
#include<pthread.h>
#include<stdio.h>
#include<time.h>

sem_t empty, full, buffSem;

pthread_t p[5],c[5];

void *producer(void *i){
    while(1){
        sleep(rand()%4);
        printf("Producer-%d wants to enter\n\n", i);
        sem_wait(&empty);
        sem_wait(&buffSem);

        printf("Producer-%d Entered CS\n\n", i);
        sleep(rand()%4);

        // Insert item;
        sem_post(&buffSem);
        sem_post(&full);
        printf("Producer-%d Exit CS\n\n", i);
    }
}

void *consumer(void *i){
    while(1){
        sleep(rand()%4);
        printf("Consumer-%d wants to enter\n\n", i);
        sem_wait(&full);
        sem_wait(&buffSem);

        printf("Consumer-%d Entered CS\n\n", i);
        sleep(rand()%4);

        // Insert item;
        sem_post(&buffSem);
        sem_post(&empty);
        printf("Consumer-%d Exit CS\n\n", i);
    }
}

int main(){
    srand(time(0));
    int size;
    printf("Enter the size of Production Buffer: ");
    scanf("%d", &size);

    sem_init(&empty,0,size);
    sem_init(&full,0,0);
    sem_init(&buffSem,0,1);

    pthread_create(&p[0],NULL, producer,(void *)0);
    pthread_create(&p[1],NULL, producer,(void *)1);
    pthread_create(&p[2],NULL, producer,(void *)2);
    pthread_create(&p[3],NULL, producer,(void *)3);
    pthread_create(&p[4],NULL, producer,(void *)4);

    pthread_create(&c[0],NULL, consumer,(void *)0);
    pthread_create(&c[1],NULL, consumer,(void *)1);
    pthread_create(&c[2],NULL, consumer,(void *)2);
    pthread_create(&c[3],NULL, consumer,(void *)3);
    pthread_create(&c[4],NULL, consumer,(void *)4);

    pthread_join(p[0],NULL);
    pthread_join(c[0],NULL);

    pthread_join(p[1],NULL);
    pthread_join(c[1],NULL);

    pthread_join(p[2],NULL);
    pthread_join(c[2],NULL);

    pthread_join(p[3],NULL);
    pthread_join(c[3],NULL);

    pthread_join(p[4],NULL);
    pthread_join(c[4],NULL);

    return 0;

}