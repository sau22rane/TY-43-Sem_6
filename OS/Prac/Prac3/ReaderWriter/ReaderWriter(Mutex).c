#include<pthread.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

pthread_mutex_t wsem,rsem;
pthread_t r[5],w[2];
int readcount;
int shared = 0;


void *reader (void *i){
    printf("\nReader-%d wants to enter\n", i);
    pthread_mutex_lock(&rsem);
    readcount++;
    if(readcount==1)
        pthread_mutex_lock(&wsem);
    pthread_mutex_unlock(&rsem);
    
    printf("\nReader-%d Entered Critical section.\n", i);
    printf("\nReader-%d Reading var = %d.\n", i,shared);
    sleep(5);

    pthread_mutex_lock(&rsem);
    readcount--;
    if(readcount==0)
        pthread_mutex_unlock(&wsem);
    pthread_mutex_unlock(&rsem);
    printf("\nReader-%d Exit Critical section.\n", i);

    return NULL;
}

void *writer (void *i){
    printf("\n\nWriter-%d is trying to enter", i);
    pthread_mutex_lock(&wsem);

    printf("\nWriter-%d entered Critical section.\n", i);
    shared = rand()%50;
    printf("\nWriter-%d Writing var = %d.\n", i,shared);
    sleep(5);
    pthread_mutex_unlock(&wsem);
    printf("\nWriter-%d Exit Critical section.\n", i);
    return NULL;
}

int main(){
    int n1,n2,i;
    pthread_mutex_init(&rsem,NULL);
    pthread_mutex_init(&wsem,NULL);
    readcount=0;
    pthread_create(&r[0],NULL,reader,(void *)0);
    pthread_create(&w[0],NULL,writer,(void *)1);
    pthread_create(&r[1],NULL,reader,(void *)1);
    pthread_create(&r[2],NULL,reader,(void *)2);
    pthread_create(&r[3],NULL,reader,(void *)3);
    pthread_create(&w[1],NULL,writer,(void *)2);
    pthread_create(&r[4],NULL,reader,(void *)4);

    pthread_join(r[0],NULL);
    pthread_join(w[0],NULL);
    pthread_join(r[1],NULL);
    pthread_join(r[2],NULL);
    pthread_join(r[3],NULL);
    pthread_join(w[1],NULL);
    pthread_join(r[4],NULL);

    sleep(1);
}
