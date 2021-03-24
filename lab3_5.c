#include <stdio.h>
#include <math.h>
#include <sys/mman.h>
#include <pthread.h>
#include <malloc.h>
#include <stdlib.h>

#include <time.h>

//this structure is pretty useless I should make a new one smh
struct matrix {
    int i; //column
    int j; //row 
    int size;
    int product;
};

//global variables representing matrices where c = a * b
int a[18][16], b[16][18], c[18][18];

//define structure of type matrix
typedef struct matrix matrix_t;

//generates matrix a
void createA(int a[18][16]){
    printf("matrix A is: \n");
    for (int i = 0; i < 18; i++){
        for (int j = 0; j < 16; j++){
            a[i][j] = (i + 1) + (j + 1); 
            printf(" %d", a[i][j]);
        }
        printf(" \n");
    }
}

//generates matrix b
void createB(int b[16][18]){
    printf("matrix B is: \n");
    for (int i = 0; i < 16; i++){
        for (int j = 0; j < 18; j++){
            b[i][j] = (i + 1) + 2 * (j + 1);   
            printf(" %d", b[i][j]);
        }
        printf(" \n");
    }
}

//matrix multiplication at coord i,j
void *func(void *arg){
    matrix_t *multiply;
    multiply = (matrix_t*)arg;
    multiply->product = 0;

    for (int i = 0; i < 16; i++){

        
            multiply->product += a[multiply->i][i] * b[i][multiply->j];
    }

    c[multiply->i][multiply->j] = multiply->product; //returns matrix multiplication to matrix C (global vars)
}

int main (void){
    int i, j, iCounter = 0;

    //18, since the product of this matrix multiplication is a 18x18 matrix
    matrix_t m[18][18];
    pthread_t t[18][18];
    
    //create matrices a and b
    createA(a);
    createB(b);

    //make one thread for each var of the product of a matrix multiplication
    for (i = 0; i < 18; i++){
        for (j = 0; j <18; j++){
            m[i][j].i = i;
            m[i][j].j = j;
            pthread_create(&t[i][j], NULL, &func, &m[i][j]);
        }
    }

    //join pthreads once done
    for (i = 0; i < 18; i++){
        for (j = 0; j <18; j++){
            pthread_join(t[i][j], NULL);

            //prints diaganols of matrix
            if (i ==j){
                printf("i = %d j = %d c= %d \n", i, j, c[i][j]);
            }
        }
    }

    pthread_exit(NULL);
}


