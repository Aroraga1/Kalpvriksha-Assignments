#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 10
#define INTENSITY_MAX 255

int sonarImage[MAX_SIZE * MAX_SIZE]; 

unsigned long customSeed = 1; 

int customRandom() {
    return rand(); 
}

void askForSizeAndStart(int *sizeToUse);
void printImage(int *dataPointer, int sizeOfImage);
void generateRandomImage(int *dataPointer, int sizeOfImage);
void performRotation(int *dataPointer, int sizeOfImage);
void applySmoothing(int *dataPointer, int sizeOfImage);
void swap(int *pointerFirst, int *pointerSec);

void askForSizeAndStart(int *sizeToUse) {
    int userValue = 0;
    while (userValue < 2 || userValue > MAX_SIZE) {
        printf("Enter image size (2-%d): ", MAX_SIZE);
        if (scanf("%d", &userValue) != 1) {
            while(getchar() != '\n'); 
        }
        if (userValue < 2 || userValue > MAX_SIZE) {
            printf("Invalid size. Must be between 2 and %d.\n", MAX_SIZE);
        }
    }
    *sizeToUse = userValue;
}

void printImage(int *dataPointer, int sizeOfImage) {
    int N = sizeOfImage;
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            int *currentPtr = dataPointer + (row * N) + col;
            int value = *currentPtr;
            printf("%4d", value);
        }
        printf("\n");
    }
}

void generateRandomImage(int *dataPointer, int sizeOfImage) {
    int N = sizeOfImage;
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            int *currentPtr = dataPointer + (row * N) + col;
            *currentPtr = customRandom() % (INTENSITY_MAX + 1);
        }
    }
}

void swap(int *pointerFirst, int *pointerSec) {
    int tempValue = *pointerFirst;
    *pointerFirst = *pointerSec;
    *pointerSec = tempValue;
}

void performRotation(int *dataPointer, int sizeOfImage) {
    int N = sizeOfImage;
    
    for (int row = 0; row < N; row++) {
        for (int col = row + 1; col < N; col++) {
            int *p_rc = dataPointer + (row * N) + col;
            int *p_cr = dataPointer + (col * N) + row;
            swap(p_rc, p_cr);
        }
    }

    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N / 2; col++) {
            int *p_left = dataPointer + (row * N) + col;
            int *p_right = dataPointer + (row * N) + (N - 1 - col);
            swap(p_left, p_right);
        }
    }
}

void applySmoothing(int *dataPointer, int sizeOfImage) {
    int N = sizeOfImage;
    int originalRow[MAX_SIZE]; 

    for (int row = 0; row < N; row++) {
        
        for (int col = 0; col < N; col++) {
             int *currentPtr = dataPointer + (row * N) + col;
             originalRow[col] = *currentPtr;
        }

        for (int col = 0; col < N; col++) {
            long totalIntensity = 0;
            int neighborCount = 0;

            for (int rOffset = -1; rOffset <= 1; rOffset++) {
                for (int cOffset = -1; cOffset <= 1; cOffset++) {
                    int neighborRow = row + rOffset;
                    int neighborCol = col + cOffset;

                    if (neighborRow >= 0 && neighborRow < N && neighborCol >= 0 && neighborCol < N) {
                        int neighborValue;

                        if (neighborRow < row) {
                            int *neighborPtr = dataPointer + (neighborRow * N) + neighborCol;
                            neighborValue = *neighborPtr;
                        } else if (neighborRow == row) {
                            neighborValue = originalRow[neighborCol];
                        } else { 
                            int *neighborPtr = dataPointer + (neighborRow * N) + neighborCol;
                            neighborValue = *neighborPtr;
                        }
                        
                        totalIntensity += neighborValue;
                        neighborCount++;
                    }
                }
            }

            int newIntensity = (int)((float)totalIntensity / (float)neighborCount + 0.5f);
            
            int *currentPlacePtr = dataPointer + (row * N) + col;
            *currentPlacePtr = newIntensity;
        }
    }
}

int main() {
    srand(time(NULL)); 
    
    int imageSize = 0;
    
    askForSizeAndStart(&imageSize);
    
    int *dataPointer = sonarImage; 
    
    generateRandomImage(dataPointer, imageSize);
    printf("Original Randomly Generated Matrix:\n");
    printImage(dataPointer, imageSize);
    
    performRotation(dataPointer, imageSize);
    printf("\nMatrix after 90 degree Clockwise Rotation:\n");
    printImage(dataPointer, imageSize);
    
    applySmoothing(dataPointer, imageSize);
    printf("\nMatrix after Applying 3x3 Smoothing Filter:\n");
    printImage(dataPointer, imageSize);
    
    return 0;
}