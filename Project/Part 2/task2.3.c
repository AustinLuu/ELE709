#include <stdio.h>
#include "dlab_def.h"
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>

#define MAXS 10000
#define PI M_PI
float Kp, run_time, Fs, theta[MAXS], ref[MAXS], magnitude;  

float Ti, Td, N; 
float Fsq, duty_cycle;         /* Input frequency for a square wave and duty cycle*/

int motor_num, no_of_samples;
char selection;
sem_t data_avail;
pthread_mutex_t completion=PTHREAD_MUTEX_INITIALIZER;

int main()
{
    pthread_t control;
    char user_input[8];
    Kp=101; //initializing Kp to 101
    run_time=10.0; //initial run time is 10.0
    Fs=200.0; //initial frequecy is 200Hz 
    motor_num=2; //motor number is 2
    magnitude=5*M_PI/18; // default angle given in lab manual (50 deg) 

    Fsq = 0.5; 
    duty_cycle = 50; 
    Ti = 0.03666666;
    Td = 0.00916666; 
    N = 20; 
    strcpy(user_input,"square");

    no_of_samples=(int)(run_time*Fs);
    int i;
    if(strcmp(user_input,"step")==0){ //if input is step, initialized ref as normally would
        for(i=0; i<no_of_samples;i++){
            ref[i]=magnitude;               
        }  
    } 
    if (strcmp(user_input, "square") == 0){ 
        Square(ref, no_of_samples, Fs, magnitude, Fsq, duty_cycle);
    }

    while(1){
       
    	pthread_mutex_lock(&completion);
        printf("\t Feel free to enter a command\n");
        printf("r: Run the control algorithm \n");
        printf("p: Change value of Kp \n");
        printf("f: Change value of sample frequency, Fs \n");
        printf("t: Change value of run_time, Tf \n");
        printf("u: Change the type of inputs(Step or Square) \n");   
        printf("i: Change value of Ti\n");
        printf("d: Change value of Td\n");
        printf("n: Change value of N\n");
        printf("g: Plot results on screen \n");
        printf("h: Save the plot results in Postscript \n");
        printf("q: exit \n \n"); 
        scanf(" %c",&selection);
        switch(selection){
        case 'r':   
            printf("\n Case r \n");
            no_of_samples=(int)(run_time*Fs);
            int i;
            if(strcmp(user_input,"step")==0){ //if input is step, initialized ref as normally would
                for(i=0; i<no_of_samples;i++){
                    ref[i]=magnitude;               
                }  
            } 
            if (strcmp(user_input, "square") == 0){ 
                Square(ref, no_of_samples, Fs, magnitude, Fsq, duty_cycle);
            }
            sem_init(&data_avail,0,1);
            printf("\n Initializing Motor \n");
            Initialize(Fs,motor_num); 
            printf("\n Creating Thread \n \n");
            pthread_create(&control,NULL,&Control,NULL);
            pthread_join(control,NULL);
            Terminate();
            sem_destroy(&data_avail);
            break;
        case 'u':
            printf("\n Case U \n");   
            printf(" Type either step or square to choose input \n \n"); 
            scanf("%s",user_input);
            if(strcmp(user_input,"step")==0){
				printf("Enter the Magnitude of Step\n");
                scanf("%f", &magnitude);
                magnitude=magnitude*M_PI/180;//deg to rad 
                printf("New Step Magnitude = %f \n", magnitude);
            } 
            if (strcmp(user_input,"square")==0) //Square
			{
				printf("Enter Magnitude: \n");
				scanf("%f", &magnitude);
                magnitude=magnitude*M_PI/180;//deg to rad 
                printf("New Square Magnitude = %f \n", magnitude); 

                printf("Enter Square Frequency\n");
				scanf("%f", &Fsq);
                printf("New Square Frequency = %f \n", Fsq ); 
                
                printf("Enter Duty Cycle:\n");
				scanf("%f", &duty_cycle);
                printf("New Square DutyCycle = %f \n", duty_cycle); 
                Square(ref, no_of_samples, Fs, magnitude, Fsq, duty_cycle);
			}
            pthread_mutex_unlock(&completion);
            break; 
		case 'i':  
            printf(" \n Case I \n");
		    printf("Enter Ti \n");
            scanf("%f",&Ti);
            pthread_mutex_unlock(&completion);
            break;
		case 'd':
            printf(" \n Case D \n");
		    printf("Enter Td\n");
            scanf("%f",&Td);
            pthread_mutex_unlock(&completion);
            break;
		case 'n':
            printf(" \n Case N \n");
		    printf("Enter N \n");
            scanf("%f",&N);
            pthread_mutex_unlock(&completion);
            break;
        case 'p':
            printf("\n Case P \n");   
            printf("Enter new value for Kp \n"); 
            scanf("%f",&Kp); 
            printf("New Kp value = %f \n", Kp);
            pthread_mutex_unlock(&completion);
            break;
        case 'g':
            printf("\n Case G \n");
            plot(ref,theta,Fs,no_of_samples,SCREEN,"Proportional Control (Kp)","Time(sec)","Rotation(rad)");
            pthread_mutex_unlock(&completion);
            break;
        case 'h':
            printf("\n Case H \n");
            plot(ref,theta,Fs,no_of_samples,PS,"Proportional Control (Kp)","Time(sec)","Rotation(rad)");
            pthread_mutex_unlock(&completion);
            break;
        case 'q': 
            printf("\n Case Q \n");
            printf("Done! \n");
            pthread_mutex_unlock(&completion);
            exit(0);
        case 'f': 
            printf("\n Case F \n");
            printf(" Enter a new Frequency :\n");
            scanf("%f",&Fs);
            printf("New Fs value = %f \n", Fs);
		    pthread_mutex_unlock(&completion);
            break;
        case 't':
            printf("\n Case T \n");
            printf("Enter Run Time :\n");
            scanf("%f",&run_time);
            printf("New Tf value = %f \n", run_time);
            pthread_mutex_unlock(&completion);
            break;
        default:
            printf("\n Case DEFAULT \n");
            printf("Invalid\n");
		    pthread_mutex_unlock(&completion);
            break;
        }
       
    }
    return 0;
}

void *Control(void *arg){
    int k=0;
    no_of_samples=(int)(run_time*Fs); 
    float Ts = 1/Fs;
    float motor_position;
    float ek = 0;   //tracking error
    float uk = 0;  //calculate control value 
    float ek_p = 0, ukint = 0, ukint_p = 0, ukder = 0, ukder_p = 0, uksum = 0; 
    
while(k<no_of_samples){
        sem_wait(&data_avail);
        motor_position=EtoR(ReadEncoder());
        ek=ref[k]-motor_position;
        uk=Kp*ek; 
        ukint = (ukint_p) + ((Kp/Ti)*ek_p*Ts); /* creating u(kt) of integral term */ 
        ukder = ((Td)/(N*Ts+Td)*ukder_p)+((Kp*Td*N)/(N*Ts+Td)*(ek-ek_p)); /* creating u(kt) of the current derivative term */ 
        uksum = uk + ukint + ukder;
        DtoA(VtoD(uksum));
        theta[k]=motor_position;  

        ek_p = ek;
        ukint_p = ukint; 
        ukder_p = ukder;
        k++;

	if(k==no_of_samples){
	    pthread_mutex_unlock(&completion);
    	}
    }
    pthread_exit(NULL);
}