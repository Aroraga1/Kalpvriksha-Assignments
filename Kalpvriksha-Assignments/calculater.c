#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h> // for INT_MAX and INT_MIN

const char *p; // global pointer 


void skipSpaces(void) {
    while (*p == ' ') p++;
}


int checkAndResolve(int *err) {
    skipSpaces();
    if (*p == '\0') { *err = 1; return 0; }

    if (isdigit((unsigned char)*p)) {
        int num = 0;
        while (isdigit((unsigned char)*p)) {
            num = num * 10 + (*p - '0');
            p++;
        }
        return num;
    }
    else if (*p == '(' || *p == ')') {
        *err = 1;
        return 0;
    }
    else {
        *err = 1; 
        return 0;
    }
}


int HighOrderOperations(int *err) {
    int result = checkAndResolve(err);
    if (*err) return 0;
    skipSpaces();

    while (*p == '*' || *p == '/') {
        char op = *p;
        p++;
        int right = checkAndResolve(err);
        if (*err) return 0;

        if (op == '*') {
            result *= right;
        } else {
            if (right == 0) {
                *err = 2; 
                return 0;
            }
            result /= right;
        }
        skipSpaces();
    }
    return result;
}

// for + & - operations
int LowOrderOperations(int *err) {
    skipSpaces();
    int result = HighOrderOperations(err);
    if (*err) return 0;
    skipSpaces();

    while (*p == '+' || *p == '-') {
        char op = *p;
        p++;
        int right = HighOrderOperations(err);
        if (*err) return 0;

        if (op == '+') result += right;
        else result -= right;

        skipSpaces();

        // checking for invalid character in our input
        if (*p && !isdigit(*p) && *p != '+' && *p != '-' && *p != '*' && *p != '/' && *p != ' ') {
            *err = 1;
            return 0;
        }

        // FIX: Yeh code *p != '\0' check ko LowOrderOperations ki loop se bahar move karne ke liye zaroori hai
        // kyunki HighOrderOperations ke baad bache hue characters (jaise *) ko is loop mein check nahi kiya jaana chahiye.
        // Remove the misplaced end-of-expression check inside the while loop
        // skipSpaces(); 
        // if (*p != '\0') {
        // *err = 1;
        // return 0;
        // }
    }
    
    skipSpaces(); 
    //checking for any extra characters
    if (*p != '\0') {
    *err = 1;
    return 0;
    }
    
    return result;
}

int main() {
    char input[100];

    printf("Enter expression: ");
    fgets(input, sizeof(input), stdin);


    p = input; // initializing global pointer here
    int err = 0;
    int ans = LowOrderOperations(&err);

    if (err == 1) {
        printf("Error: Invalid expression.\n");
    } else if (err == 2) {
        printf("Error: Division by zero.\n");
    } else {
        printf("%d\n", ans);
    }

    return 0;
}