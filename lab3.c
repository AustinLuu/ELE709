#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

struct thread_info {
   int maxitr;
   double exec_time;
   double a, b, c;
   char op;
};

typedef struct thread_info thread_info_t;

void *func(void *arg)
{
   struct timespec time_1, time_2;
   double exec_time;
   thread_info_t *info;
   int i, maxitr;
   volatile double a, b,c;
   char op;
 

   info = (thread_info_t *)arg;
   maxitr = info->maxitr;
   a = info->a;
   b = info->b;
   op = info->op;
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
   info->c = c;
   info->exec_time = exec_time;

   pthread_exit(NULL);
   
}

int main(void)
{
   pthread_t thread[3];
   thread_info_t info[3];
   double maxitr;
   int i;
   maxitr = 5.0e8;

   info[0].op='+'; 
   info[1].op='*';   
   info[2].op='/';
   info[3].op='-';
   
   for (i = 0;i < 4;i++){ 
   info[i].maxitr=(int)maxitr;
   info[i].a=5;
   info[i].b=5;  
   if (pthread_create(&thread[i], NULL, &func, &info[i]) != 0) {
           printf("Error in creating thread 1\n");
           exit(1);
   }
   pthread_join(thread[i], NULL);
   printf("Exec time for thread = %lf nsec\n", info[i].exec_time);
   }
   

   pthread_exit(NULL);
}
