#include<stdio.h>

#define MAX_STUDENT 100

char getGrade(int avg){
    char grade;
    if(avg>=85) {
        grade='A';
    }
    else if(avg>=70){
        grade='B';
    }
    else if(avg>=50){
        grade='C';
    }
    else if(avg>=35){
        grade='D';
    } 
    else {
        grade='F';
    }
    return grade;
}

typedef struct studentMarksData{
    int id;
    char name[50];
    int marks[3];
    int total;
    float average;
    char grade; 
} data;

void assignValues(data *data){
    data->total=0;
    for(int i=0; i<3; i++){
        data->total+=data->marks[i];
    } 
    data->average= (float) data->total/3;
    data->grade=getGrade(data->average);
}

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void printData(int temp, data dataArray[]){
    printf("\n");
        for(int i=0; i<temp; i++) {
            printf("Roll: %d\n", dataArray[i].id);
            printf("Name: %s\n", dataArray[i].name);
            printf("Total: %d\n", dataArray[i].total);
            printf("Average: %f\n", dataArray[i].average);
            printf("Grade: %c\n", dataArray[i].grade);
            switch (dataArray[i].grade)
            {
            case 'A':
                printf("Performance: *****");
                break;
            case 'B':
                printf("Performance: ****");
                break;
            case 'C':
                printf("Performance: ***");
                break;
            case 'D':
                printf("Performance: **");
                break;
            default: 
                break;

            }
            printf("\n\n");
        }
    printf("\n");
}

void evaluateStudentRecord(){
    data dataArray[MAX_STUDENT];
    int temp;
    printf("Stop(0) / Number of entries: ");
    if(scanf("%d", &temp) != 1){
        printf("Wrong Entry! Enter specific numbers in Entries\n");
        clearBuffer();
        evaluateStudentRecord();
    }
    if (temp < 0 || temp > MAX_STUDENT) {
    printf("\nInvalid Entry\n");
    clearBuffer();
    evaluateStudentRecord();
    return;
    }
    if(temp==0) return;
    printf("Enter your Entry as -> id name Mark1 Mark2 Mark3\n");
    //Taking input till the number of entries..
            for(int i=0; i<temp; i++){
                
                if (scanf("%d", &dataArray[i].id) != 1) {
                printf("\nInvailid Entry!\n");
                break; 
                }
                
                if (scanf("%49s", dataArray[i].name) != 1) { 
                printf("\nInvailid Entry!.\n");
                i--;
                continue;
                }

            for(int j=0; j<3; j++){
                if (scanf("%d", &dataArray[i].marks[j]) != 1) {
                printf("\nInvailid Entry!\n", j + 1);
                break; 
            }
            if (dataArray[i].marks[j] < 0 || dataArray[i].marks[j] > 100) {
                printf("\nInvailid Entry!\n", j + 1);
                break; 
            }
            }
            assignValues(&dataArray[i]);
        }
        printData(temp,dataArray);
        evaluateStudentRecord();
}


int main(){
    printf("*Student Marks eveluation system*\n");
    evaluateStudentRecord();
    return 0;
}
