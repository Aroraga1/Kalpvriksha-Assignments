#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define MAX_SIZE 10
#define INTENSITY_MAX 255 
int sonarImage[MAX_SIZE][MAX_SIZE]; 
int tempImage[MAX_SIZE][MAX_SIZE];

void generateMatrix(int N){
    int *ptr = (int *)sonarImage; 
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            *(ptr + (i * MAX_SIZE) + j) = rand() % (INTENSITY_MAX + 1); 
        }
    }
}

void printImage(int (*ptr)[MAX_SIZE], int N){ 
    int *base_ptr = (int *)ptr; 
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            printf("%4d", *(base_ptr + (i * MAX_SIZE) + j)); 
        }
        printf("\n"); 
    }
}

void swap(int *ptr1, int *ptr2){
    int temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;
}

void rotateMatrix(int (*ptr)[MAX_SIZE], int N){
    int *ptrf = (int *)ptr;
    for(int i=0; i<N; i++){
        for(int j=i+1; j<N; j++){
            swap(ptrf + (i * MAX_SIZE) + j, ptrf + (j * MAX_SIZE) + i);
        }
    }
    for(int i=0; i<N; i++){
        for(int j=0; j<N/2; j++){
            swap(ptrf + (i * MAX_SIZE) + j, ptrf + (i * MAX_SIZE) + (N - 1 - j));
        }
    }
}

void applySmoothing(int (*ptr)[MAX_SIZE], int N){
    int *ptr_in = (int *)ptr; 
    int *ptr_out = (int *)tempImage; 
    int i,j;
    for(int raw = 0; raw < N; raw++){
        for(int col = 0; col < N; col++){
            long long sum = 0;
            int count = 0;
            for(int ki = -1; ki <= 1; ki++){
                for(int kj = -1; kj <= 1; kj++){
                    i = raw + ki; 
                    j = col + kj;        
                    if(i >= 0 && i < N && j >= 0 && j < N){
                        int index = i * MAX_SIZE + j;
                        sum += *(ptr_in + index);
                        count++;
                    }
                }
            }
            int avg = (int)(sum / count);
            int output_index = raw * MAX_SIZE + col;
            *(ptr_out + output_index) = avg;
        }
    }
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            int index = i * MAX_SIZE + j;
            *(ptr_in + index) = *(ptr_out + index);
        }
    }
}

int main(){
    srand(time(NULL)); 
    int imageSize;
    printf("Enter matrix size (2-10): ");
    if (scanf("%d", &imageSize) != 1 || imageSize < 2 || imageSize > MAX_SIZE) {
        printf("Invalid size or size < 3 entered. Filtering requires N >= 3.\n");
        return 0;
    }
    generateMatrix(imageSize);
    printf("\nOriginal Randomly Generated Matrix:\n");
    printImage(sonarImage, imageSize); 
    printf("\nMatrix after 90 degree Clockwise Rotation:\n");
    rotateMatrix(sonarImage,imageSize);
    printImage(sonarImage,imageSize);
    printf("\n");
    applySmoothing(sonarImage, imageSize);
    printf("\nMatrix after Applying 3*3 Smoothing Filter:\n");
    printImage(sonarImage, imageSize); 
    return 0; 
}
