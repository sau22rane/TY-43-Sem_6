#include<semaphore.h>
#include<pthread.h>
#include<stdio.h>

int readcount=0;
sem_t s, wsem;
pthread_t r[5],w[2];
int shared = 0;

void *reader(void *i) {
    printf("\nReader-%d wants to enter\n", i);

    sem_wait(&s);
    readcount++;
    if(readcount==1)
        sem_wait(&wsem);
    sem_post(&s);
    printf("\nReader-%d Entered Critical section.\n", i);
    printf("\nReader-%d Reading var = %d.\n", i,shared);
    sleep(5);
    sem_wait(&s);
    readcount--;
    if(readcount==0)
        sem_post(&wsem);
    sem_post(&s);
    printf("\nReader-%d Exit Critical section.\n", i);

    return NULL;
}

void *writer(void *i){
    printf("\nWriter-%d wants to enter\n", i);

    sem_wait(&wsem);

    printf("\nWriter-%d entered Critical section.\n", i);
    shared = rand()%50;
    printf("\nWriter-%d Writing var = %d.\n", i,shared);
    sleep(5);
    sem_post(&wsem);
    printf("\nWriter-%d Exit Critical section.\n", i);

    return NULL;
}

int main()
{
sem_init(&s,0,1);
sem_init(&wsem,0,1);

pthread_create(&r[0],NULL, reader,(void *)0);
pthread_create(&w[0],NULL,writer,(void *)0);
pthread_create(&r[1],NULL,reader,(void *)1);
pthread_create(&r[2],NULL,reader,(void *)2);
pthread_create(&r[3],NULL,reader,(void *)3);
pthread_create(&w[1],NULL,writer,(void *)3);
pthread_create(&r[4],NULL,reader,(void *)4);
pthread_join(r[0],NULL);
pthread_join(w[0],NULL);
pthread_join(r[1],NULL);
pthread_join(r[2],NULL);
pthread_join(r[3],NULL);
pthread_join(w[1],NULL);
pthread_join(r[4],NULL);
 return 0;
}  
