#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define MAX_SIZE 100 
#define INTENSITY_MAX 255 

int sonarImage[MAX_SIZE][MAX_SIZE]; 
const int ENCODE_BASE = INTENSITY_MAX + 1;

int getAverage(int (*ptr)[MAX_SIZE], int N, int raw, int col) {
    long long sum = 0;
    int count = 0;
    
    for(int cellOuter = -1; cellOuter <= 1; cellOuter++) {
        for(int cellInner = -1; cellInner <= 1; cellInner++) {
            int i = raw + cellOuter; 
            int j = col + cellInner; 
            
            if (i >= 0 && i < N && j >= 0 && j < N) {
                int index = i * MAX_SIZE + j;
                
                int neighborValue = *(((int *)ptr) + index) % ENCODE_BASE; 
                sum += neighborValue;
                count++;
            }
        }
    }
    return (int)(sum / count);
}

void encodeNewValue(int (*ptr)[MAX_SIZE], int raw, int col, int avg) {
    int index = raw * MAX_SIZE + col;
    *(((int *)ptr) + index) += avg * ENCODE_BASE;
}

void decodeMatrix(int (*ptr)[MAX_SIZE], int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int index = i * MAX_SIZE + j;
            *(((int *)ptr) + index) /= ENCODE_BASE; 
        }
    }
}

void applySmoothing(int (*ptr)[MAX_SIZE], int N) {
    for(int raw = 0; raw < N; raw++) {
        for(int col = 0; col < N; col++) {
            int avg = getAverage(ptr, N, raw, col); 
            encodeNewValue(ptr, raw, col, avg); 
        }
    }
    decodeMatrix(ptr, N); 
}

void generateMatrix(int N){
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            *(((int *)sonarImage) + (i * MAX_SIZE) + j) = rand() % (INTENSITY_MAX + 1); 
        }
    }
}

void printImage(int (*ptr)[MAX_SIZE], int N){ 
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            printf("%4d", *(((int *)ptr) + (i * MAX_SIZE) + j)); 
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
    int index1, index2;

    for(int i=0; i<N; i++){
        for(int j=i+1; j<N; j++){
            index1 = i * MAX_SIZE + j;
            index2 = j * MAX_SIZE + i;
            swap(((int *)ptr) + index1, ((int *)ptr) + index2);
        }
    }
    for(int i=0; i<N; i++){
        for(int j=0; j<N/2; j++){
            index1 = i * MAX_SIZE + j;
            index2 = i * MAX_SIZE + (N - 1 - j);
            swap(((int *)ptr) + index1, ((int *)ptr) + index2);
        }
    }
}

int main(){
    srand(time(NULL)); 
    int imageSize;
    printf("Enter matrix size (2-10): ");
    if (scanf("%d", &imageSize) != 1 || imageSize < 2 || imageSize > MAX_SIZE) {
        printf("Invalid size! It should be: 2>=N<=10.\n");
        return 0;
    }
    generateMatrix(imageSize);
    printf("\nGenerated Matrix:\n");
    printImage(sonarImage, imageSize); 
    
    printf("\n90 degree Rotated Matrix :\n");
    rotateMatrix(sonarImage,imageSize);
    printImage(sonarImage,imageSize);
    
    
    printf("\nMatrix after Applying 3*3 Smoothing Filter:\n");
    applySmoothing(sonarImage, imageSize);
    printImage(sonarImage, imageSize); 
    
    return 0; 
}
