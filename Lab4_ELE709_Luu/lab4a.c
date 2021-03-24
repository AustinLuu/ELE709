#include <stdio.h>      
#include <stdlib.h>    
#include <unistd.h>      
#include <pthread.h>    

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t can = PTHREAD_COND_INITIALIZER;
char string_to_print[] = "02468";
char string_to_print1[] = "13579";
int string_index = 0, tmp_index;



void *function1(void *arg)
{
 
    sleep(1);
    char letter = *(char *) arg;
    pthread_mutex_lock(&mut); 
    while (1)
    {
        tmp_index = string_index;
        if (!(tmp_index+2 > sizeof(string_to_print))) 
        {
            printf("%c", letter);
            printf("%c", string_to_print[tmp_index]);
            printf(" ");
            pthread_cond_signal(&can);
            usleep(1); 
        }
        pthread_cond_wait(&can, &mut);
    }
    pthread_mutex_unlock(&mut); 
}

void *function2(void *arg)
{
    sleep(1); 
    char letter = *(char *) arg;
    pthread_mutex_lock(&mut); 
    while (1) 
    {
        tmp_index = string_index;
        if (!(tmp_index+2 > sizeof(string_to_print1))) 
        {
            pthread_cond_wait(&can, &mut);
            printf("%c", letter);
            printf("%c\n", string_to_print1[tmp_index]);
            printf(" ");
            usleep(5);   
        }
        string_index = tmp_index + 1;
        if (tmp_index+2 > sizeof(string_to_print1)) 
        {
            string_index = 0;   
        }
        pthread_cond_signal(&can);      
   }
   pthread_mutex_unlock(&mut);

}


int main(void)
{
    pthread_t thread1, thread2;
    pthread_t threads[1];
    int k;
    char A = 'A';
    char B = 'B';
    if (pthread_create(&thread1, NULL, &function1, &A) != 0) 
    {
        printf("Error in creating thread\n");
        exit(1);
    }
    if (pthread_create(&thread2, NULL, &function2, &B) != 0) 
    {
        printf("Error in creating thread\n");
        exit(1);
    }
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    sleep(20);  
    for (k = 0; k < 2; k++) 
    {
        pthread_cancel(threads[k]);
    }
    pthread_exit(NULL);
}
