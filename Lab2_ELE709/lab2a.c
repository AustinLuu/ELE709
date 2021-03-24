#include <stdio.h>
#include <time.h>

int main()
{
  int i, itr;
  struct timespec start_time, stop_time, sleep_time, res;
  double tmp, a, b, c, elapsed_time;
  char op;
  

 
  itr = 500000000;

  printf("Enter a, b & operation: ");
  scanf("%lf %lf %c", &a, &b, &op);  
  
  clock_gettime(CLOCK_REALTIME, &start_time);  
  for(i = 0; i < itr; i++)
  {
      
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
  clock_gettime(CLOCK_REALTIME, &stop_time); 
  elapsed_time= (stop_time.tv_sec - start_time.tv_sec)*1e9;

  printf("Average time: %lf nsec\n", (elapsed_time/itr));
  return 0;
}
