#include<stdio.h>

#define MAX_STUDENT 100

char getGrade(int avg){
    char grade;

    if(avg>=85) grade='A';
    else if(avg>=70) grade='B';
    else if(avg>=50) grade='C';
    else if(avg>=35) grade='D';
    else grade='F';

    return grade;
}

typedef struct StudentData{
    int id;
    char name[50];
    int marks[3];
    int total;
    float average;
    char grade; 
} st;

void assignValues(st *st){
    st->total=0;
    for(int i=0; i<3; i++) st->total+=st->marks[i];
    st->average= (float) st->total/3;
    st->grade=getGrade(st->average);
}

void clearBuffer(){
    char c;
    while(c = getchar() != '\n');
}

void ContinueOperation(){
    st st[MAX_STUDENT];
    int temp;
    printf("Stop(0) / Number of entries: ");
    if(scanf("%d", &temp) != 1){
        printf("Wrong Entry! Enter specific numbers of Entries\n");
        clearBuffer();
        ContinueOperation();
    }
    if (temp < 0 || temp > 100) {
    printf("\n id %d is out of range (0-100)\n");
    }
    if(temp==0) return;
    printf("Enter your Entry as -> id name Mark1 Mark2 Mark3\n");

    //Taking input till the number of entries..
            for(int i=0; i<temp; i++){
                
                if (scanf("%d", &st[i].id) != 1) {
                printf("\nInvailid Entry!\n");
                break; 
                }
                
                if (scanf("%49s", st[i].name) != 1) { 
                printf("\nInvailid Entry!.\n");
                i--;
                continue;
                }

            for(int j=0; j<3; j++){
                if (scanf("%d", &st[i].marks[j]) != 1) {
                printf("\nInvailid Entry!\n", j + 1);
                break; 
            }
            if (st[i].marks[j] < 0 || st[i].marks[j] > 100) {
                printf("\nInvailid Entry!\n", j + 1);
                break; 
            }
            }
            printf("\n");
            assignValues(&st[i]);
        }
        for(int i=0; i<temp; i++) {
            printf("Roll: %d\n", st[i].id);
            printf("Name: %s\n", st[i].name);
            printf("Total: %d\n", st[i].total);
            printf("Average: %f\n", st[i].average);
            printf("Grade: %c\n", st[i].grade);
            printf("\n")
        }
        ContinueOperation();
}

int main(){
    
    ContinueOperation();
    
    return 0;
}