#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void clearBuffer(); 

struct Product{
    int id;
    char name[51];
    float price;
    int quantity;
};

int getValidInput(void *var, char type, int size) {
    int result = 0;
    
    if (type == 'd') {
        result = scanf("%d", (int*)var);
    } else if (type == 'f') {
        result = scanf("%f", (float*)var);
    } else if (type == 's') {
        if (size == 51) {
            result = scanf("%50s", (char*)var);
        } else {
            return 0;
        }
    } else {
        return 0;
    }

    if (result != 1) {
        printf("Wrong way: Invalid input\n");
        clearBuffer();
        return 0;
    }
    
    return 1;
}

void printProduct(struct Product *p){
    if (p == NULL) {
        printf("\nThere is noo any data of products");
        return;
    }
    printf("\nProduct Id: %d | Name: %s | Price: %.2f | Quantity: %d",p->id, p->name, p->price, p->quantity);
}

void printAllProducts(struct Product *p, int currCount){
    printf("\n========= PRODUCT LIST =========");
    if (p == NULL || currCount <= 0) {
        printf("\nInventory is empty or not initialized.");
        return;
    }
    for(int i=0; i<currCount; i++){
        printProduct(p+i);
    }
}

void printOptions(){
    printf("\n\n========= INVENTORY MENU =========");
    printf("\n1. Add New Product");
    printf("\n2. View All Products");
    printf("\n3. Update Quantity");
    printf("\n4. Search Product by ID");
    printf("\n5. Search Product by Name");
    printf("\n6. Search Product by Price Range");
    printf("\n7. Delete Product");
    printf("\n8. Exit");
    printf("\nEnter your choice: ");
}
 
void getAllDetails(struct Product *p, int count){
    if (p == NULL) {
        printf("\nSomething wrong, no details found!");
        return;
    }
    printf("\nEnter the Details of %d product->",count);
    
    printf("\nid:");
    while (!getValidInput(&(p->id), 'd', 0)) { printf("Invalid ID.\nRe-enter: "); }
    
    printf("name:");
    while (!getValidInput(p->name, 's', 51)) { printf("Invalid Name. \nRe-enter: "); }
    
    printf("Price:");
    while (!getValidInput(&(p->price), 'f', 0)) { printf("Invalid Price. \nRe-enter: "); }
    
    printf("Quantity:");
    while (!getValidInput(&(p->quantity), 'd', 0)) { printf("Invalid Quantity. \nRe-enter: "); }
}

void updateQuantity(struct Product *p, int currCount){
    if (p == NULL || currCount <= 0) {
        printf("\nInventory is empty");
        return;
    }
    
    printf("\nEnter the id of the Product to update: ");
    int tempId;
    if (!getValidInput(&tempId, 'd', 0)) return;
    
    int found_index = -1;
    for(int i=0; i<currCount; i++){
        if((p+i)->id == tempId){
            found_index=i;
            break;
        } 
    }
    
    if (found_index != -1) {
        printf("\nEnter the updated Quantity of the Product: ");
        if (!getValidInput(&((p+found_index)->quantity), 'd', 0)) {
             printf("\ninvalid quantity");
             return;
        }
        printf("\nEnter the updated quantity:");
        printProduct(p+found_index); 
    } else {
        printf("\nProduct with ID %d not found.", tempId);
    }
}

void productById(struct Product *p, int currCount){
    if (p == NULL || currCount <= 0) {
        printf("\nInventory is empty or not initialized.");
        return;
    }
    
    printf("\nEnter the id of the Product: ");
    int tempId;
    if (!getValidInput(&tempId, 'd', 0)) return;
    
    int found = 0;
    for(int i=0; i<currCount; i++){
        if((p+i)->id == tempId){
            printf("\nYour Search Details:");
            printProduct(p+i);
            found = 1;
            break;
        } 
    }  
    if (!found) {
         printf("\nProduct with ID %d not found.", tempId);
    }
}

void productByName(struct Product *p, int currCount){
    if (p == NULL || currCount <= 0) {
        printf("\nInventory is empty");
        return;
    }
    
    printf("\nEnter the name of the Product: ");
    char name[51];
    if (!getValidInput(name, 's', 51)) return;
    
    int found = 0;
    for(int i=0; i<currCount; i++){
        if(strcmp((p+i)->name, name) == 0){
            printf("\nYour Search Details:");
            printProduct(p+i); 
            found = 1;
            break;
        } 
    }
    if (!found) {
         printf("\nProduct with Name %s not found.", name);
    }
}


void productByPriceRange(struct Product *p, int currCount){
    if (p == NULL || currCount <= 0) {
        printf("\nInventory is empty or not initialized.");
        return;
    }
    
    printf("\nEnter the maximum price limit: ");
    float temPrice;
    if (!getValidInput(&temPrice, 'f', 0)) return;
    
    int found = 0;
    for(int i=0; i<currCount; i++){
        if((p+i)->price <= temPrice){
            printf("\nProduct under the Price %.2f: ", temPrice);
            printProduct(p+i); 
            found = 1;
        } 
    }
    if (!found) {
         printf("\nNo products found under the price %.2f.", temPrice);
    }
}


void deleteProduct(struct Product *p, int *currCount){
    if (p == NULL || *currCount <= 0) {
        printf("\nInventory is empty or not initialized.");
        return;
    }
    
    printf("\nEnter the id to remove a product: ");
    int tempId;
    if (!getValidInput(&tempId, 'd', 0)) return;

    int found_index = -1; 
    
    for(int i = 0; i < *currCount; i++){
        if((p + i)->id == tempId){
            found_index = i; 
            break;  
        } 
    }
    
    if (found_index == -1) {
        printf("\nProduct with ID %d not found.", tempId);
        return; 
    }
    
    for(int i = found_index; i < *currCount - 1; i++){
        *(p + i) = *(p + i + 1); 
    }
    
    (*currCount)--; 
    
    printf("\nProduct with ID %d successfully deleted. Current count: %d", tempId, *currCount);
    
    printAllProducts(p, *currCount);
}

void clearBuffer(){
    int c;
    while((c = getchar()) != '\n' && c != EOF){}
}

int main(){
    int initialCount;
    printf("Enter initial number of products:");
    
    if (!getValidInput(&initialCount, 'd', 0) || initialCount < 0) {
        printf("Invalid initial count. Setting to 0.\n");
        initialCount = 0;
    }
    
    int currCount = initialCount; 

    struct Product *inventry = NULL;
    if (initialCount > 0) {
        inventry = (struct Product*)malloc(initialCount * sizeof(struct Product));
        if (inventry == NULL) {
            printf("Memory allocation failed.. Exiting.\n");
            return 1;
        }
    }
        
    printf("Enter Details: ");
    for(int i=0; i<initialCount; i++){
        printf("Enter details for product %d: ", i+1); 
        getAllDetails(inventry+i, i+1);
    } 

    int operationNumber;   
    do{
        printOptions();
        
        if (!getValidInput(&operationNumber, 'd', 0)) {
            operationNumber = 0;
            continue;
        }
        
        switch (operationNumber){
        case 1:{
            currCount++; 
            struct Product *temp = (struct Product*)realloc(inventry, currCount * sizeof(struct Product));
            if (temp != NULL) {
                inventry = temp;
                getAllDetails(inventry + (currCount - 1), currCount); 
            } else {
                 printf("\nMemory reallocation failed");
                 currCount--; 
            }
        }
        break;

        case 2: printAllProducts(inventry, currCount);
        break;

        case 3: updateQuantity(inventry, currCount);
        break;

        case 4: productById(inventry, currCount);
        break;

        case 5: productByName(inventry, currCount);
        break;

        case 6: productByPriceRange(inventry, currCount);
        break;

        case 7: deleteProduct(inventry, &currCount);
        break;

        case 8:
            printf("\nExiting program. Thank you!");
        break;
        
        default:
            printf("\nInvalid choice. Please enter a number between 1 and 8.");
        break;
        }
    } while (operationNumber!=8);

    free(inventry);

    return 0;
}