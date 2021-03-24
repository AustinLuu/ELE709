#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct thread_info thread_info_t;

struct thread_info {
   int maxitr;
   double exec_time;
   char op;
};

int main(void)
{
   pthread_t thread1;
   pthread_t thread2;
   pthread_t thread3;
   pthread_t thread4;

   thread_info_t info1;
   thread_info_t info2;
   thread_info_t info3;
   thread_info_t info4;

   double maxitr;

   maxitr = 5.0e8;

   info1.maxitr = (int)maxitr;
   info2.maxitr = (int)maxitr;
   info3.maxitr = (int)maxitr;
   info4.maxitr = (int)maxitr;

   info1.op='+'; 
   info2.op='*';   
   info3.op='/';
   info4.op='-';

   pthread_create(&thread1, NULL, &func, &info1);
   pthread_create(&thread2, NULL, &func, &info2);
   pthread_create(&thread3, NULL, &func, &info3);
   pthread_create(&thread4, NULL, &func, &info4);

   pthread_join(thread1, NULL);
   pthread_join(thread2, NULL);
   pthread_join(thread3, NULL);
   pthread_join(thread4, NULL);

   printf("Exec time for thread1 = %lf nsec\n", info1.exec_time);
   printf("Exec time for thread2 = %lf nsec\n", info2.exec_time);
   printf("Exec time for thread3 = %lf nsec\n", info3.exec_time);
   printf("Exec time for thread4 = %lf nsec\n", info4.exec_time);

   pthread_exit(NULL);
}

void *func(void *arg)
{
   struct timespec time_1, time_2;
   double exec_time;
   thread_info_t *info;
   int i, maxitr;
   volatile double a, b, c;
   char op;

   info = (thread_info_t *)arg;
   maxitr = info->maxitr;
   op = info->op;
   
   b = 2.3; c = 4.5;

   exec_time = 0.0;

   clock_gettime(CLOCK_REALTIME, &time_1);

   for (i = 0; i < maxitr ; i++) {
      if (op =='/'){
      c= a/b;
      }
      else if (op =='*'){     
      c= a*b;
      }
      else if (op =='+'){     
      c= a+b;
      }
      else if (op =='-'){    
      c= a-b;
      } 
   }
         
   clock_gettime(CLOCK_REALTIME, &time_2);

   exec_time = (time_2.tv_sec - time_1.tv_sec);
   exec_time = exec_time + (time_2.tv_nsec - time_1.tv_nsec);

   info->exec_time = exec_time;

   pthread_exit(NULL);
   
}