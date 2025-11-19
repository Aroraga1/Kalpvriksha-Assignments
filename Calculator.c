#include<stdio.h>
#include<ctype.h>
#include<string.h>

char *ptr; 

int arr[100];
int *stack; 

void skipWhiteSpace();
int combineDigit();
void HigherOrderOperations(char op, int operand2);
void FirstPass(char *input);
int SecondPass();
int MainProcess(char *input);

void skipWhiteSpace(){
    while(*ptr!='\0' && *ptr==' ') ptr++;
}

int combineDigit(){
    int value=0;
    while(*ptr!='\0' && isdigit(*ptr)){
        value = (value * 10) + (*ptr - '0');
        ptr++;
    }
    return value;
}

void HigherOrderOperations(char op, int operand2){
    stack--; 
    int operand1 = *stack;

    int ans = 0;
    
    switch(op){
        case '*': ans = operand1 * operand2; break;
        case '/': 
            if (operand2 == 0) {
                printf("Error: Division by zero!\n");

                *arr = 0; 
                stack = arr + 1;
                return;
            }
            ans = operand1 / operand2; 
            break;
        case '%': ans = operand1 % operand2; break;
    }
    
    *stack = ans;
    stack++; 
}

void FirstPass(char *input){
    ptr = input;
    stack = arr; 
    
    skipWhiteSpace();
    
    if(isdigit(*ptr)){
        *stack = combineDigit(); 
        stack++;
    } else {
        printf("Error: Expression must start with a number.\n");

        stack = arr; 
        return;
    }

    while(*ptr != '\0'){
        skipWhiteSpace();
        
        if(*ptr == '\0') break;
        
        if(*ptr == '*' || *ptr == '/' || *ptr == '%'){
            char op = *ptr;
            ptr++; 
            skipWhiteSpace();
            
            int operand2 = combineDigit(); 
            HigherOrderOperations(op, operand2); 
        }
        else if(*ptr == '+' || *ptr == '-'){
            *stack = (int)*ptr;
            stack++;
            ptr++; 
            skipWhiteSpace();
            
            if (isdigit(*ptr)) {
                 int num = combineDigit();
                 *stack = num;
                 stack++;
            } else {
                 printf("Error: Missing operand after operator.\n");
                 stack = arr; 
                 return;
            }
        }
        else {
            printf("Error: Unknown character '%c'\n", *ptr);
            stack = arr; 
            return;
        }
    }
}

int SecondPass(){
    if (stack == arr || stack == arr + 1) {
        if (stack == arr + 1) return *arr; 
        return 0;
    }

    int result = *arr; 
    int *current = arr + 1;

    while (current < stack) {
        char op = (char)*current; 
        current++;
        
        if (current >= stack) {
            printf("Error: Stack structure broken in Second Pass.\n");
            return 0;
        }

        int num = *current; 
        current++;

        switch (op) {
            case '+': result += num; break;
            case '-': result -= num; break;
            default: 
                printf("Error: Invalid token in Second Pass.\n");
                return 0;
        }
    }
    return result;
}

int MainProcess(char *input){
    
    FirstPass(input);
    
    if (stack == arr) {
        return 0;
    }
    
    return SecondPass();
}

int main(){
    char string[100];
    printf("Enter expression: ");
    
    fgets(string, 100, stdin);
    

    int result = MainProcess(string);
    printf("Result: %d\n", result);
    return 0;
}