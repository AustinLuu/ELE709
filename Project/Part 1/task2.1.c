#include <stdio.h>
#include "dlab_def.h"
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>

#define MAXS 10000
#define PI M_PI
float Kp, run_time, Fs, theta[MAXS], ref[MAXS], magnitude, frequency, duty_cycle;
int motor_num, no_of_samples;
char selection;
sem_t data_avail;
pthread_mutex_t completion=PTHREAD_MUTEX_INITIALIZER;

void *Control(void *arg){
    int k=0;
    no_of_samples=(int)(run_time*Fs);
    float motor_position;
    float ek;   //tracking error
    float uk;  //calculate control value
    while(k<no_of_samples){
        sem_wait(&data_avail);
        motor_position=EtoR(ReadEncoder());
        ek=ref[k]-motor_position;
        uk=Kp*ek;
        DtoA(VtoD(uk));
        theta[k]=motor_position;
        k++;
        //sem_post(&data_avail);
	//pthread_mutex_unlock(&completion);
	if(k==no_of_samples){
	    pthread_mutex_unlock(&completion);
    	}
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t control;
    char user_input[8];
    Kp=1;
    run_time=30.0; //initial run time is 30.0
    Fs=200.0; //initial frequecy is 200Hz
    motor_num=2; //motor number is 2
    magnitude=5*M_PI/18; // default angle given in lab manual (50 deg)

// Initialize the reference input vector ref[k].   
    no_of_samples=(int)(run_time*Fs);
    int i;
    for(i=0; i<no_of_samples;i++){
        ref[i]=magnitude;               
    }
// Initialize the reference input vector ref[k].   

    while(1){
       
    	pthread_mutex_lock(&completion);
        printf("\t Feel free to enter a command\n");
        printf("r: Run the control algorithm \n");
        printf("p: Change value of Kp \n");
        printf("f: Change value of sample frequency, Fs \n");
        printf("t: Change value of run_time, Tf \n");
        printf("u: Change the type of inputs(Step or Square) \n");
        printf("g: Plot results on screen \n");
        printf("h: Save the plot results in Postscript \n");
        printf("q: exit \n \n"); 
        scanf(" %c",&selection);
        switch(selection){
        case 'r':   
            printf("\n Case r \n");
            no_of_samples=(int)(run_time*Fs);
            int i;
            // Initialize the reference input vector ref[k].   
            for(i=0; i<no_of_samples;i++){
                ref[i]=magnitude;               
            } 
            // Initialize the reference input vector ref[k].   
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
            printf(" Choose Step or Square input \n \n"); 
            scanf("%s",user_input);
            if(strcmp(user_input,"step")==0){
				printf("Enter the Magnitude of Step\n");
                scanf("%f", &magnitude);
                magnitude=magnitude*M_PI/180;//deg to rad 
                printf("New Step Magnitude = %f \n", magnitude);
            } 
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
