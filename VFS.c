#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BLOCK_PER_FILE 100
#define TOTAL_BLOLCKS 1024
#define BLOCK_SIZE 512

typedef struct Nav{
    int ind;
    struct Nav *prev;
    struct Nav *next;
} Nav;

typedef struct Node{
    char name[51];
    int isDirectory;
    int size;
    int blockCount;
    int blockPointer[BLOCK_PER_FILE];
    struct Node *parent;
    struct Node *child;
    struct Node *next;
}Node;

char virtualDisk[TOTAL_BLOLCKS][BLOCK_SIZE];
Nav *head = NULL;
Nav *tail = NULL;
Node *cwd = NULL;
Node *root = NULL;


void releaseBlocks();
void freeBlock(int idx);
Node *findInDirectory(Node *dir, char *name);
void insertNode(Node *dir, Node *node);
void removeNode(Node *dir, Node *node);
void mkdir(char *name);
void create(char *name);
int allocateBlock();
void write(char *name, char *data);
void read(char *name);
void delete(char *name);
void rmdir(char *name);
void ls();
void cd(char *name);
void pwd(Node *dir);
void df();


void releaseBlocks(){
    for(int i=0; i<TOTAL_BLOLCKS; i++){
        Nav *block = malloc(sizeof(Nav));
        block->ind=i;
        block->prev=tail;
        block->next=NULL;
        if(tail){
            tail->next = block;
        }else{
            head=block;
        }
        tail=block;
    }
}

void freeBlock(int idx) {
    Nav *b = malloc(sizeof(Nav));
    b->ind = idx;
    b->next = NULL;
    b->prev = tail;
    if (tail) tail->next = b;
    else head = b;
    tail = b;
}

Node *findInDirectory(Node *dir, char *name){
    if(!dir->child) return NULL;
    Node *t = dir->child;
    do{
        if(strcmp(t->name,name)==0) return t;
        t = t->next;
    }while(t != dir->child);
    return NULL;
}

void insertNode(Node *dir, Node *node){
    if(!dir->child){
        dir->child=node;
        node->next=node;
    }else{
        Node *t = dir->child;
        while (t->next != dir->child) t = t->next;
        t->next=node;
        node->next=dir->child;        
    }
}

void removeNode(Node *dir, Node *node) {
    if (!dir->child) return;
    Node *t = dir->child;
    Node *p = NULL;
    do {
        if (t == node) break;
        p = t;
        t = t->next;
    } while (t != dir->child);
    if (t != node) return;
    if (t == dir->child && t->next == t) {
        dir->child = NULL;
    } else {
        if (t == dir->child) dir->child = t->next;
        if (p) p->next = t->next;
        else {
            Node *x = dir->child;
            while (x->next != t) x = x->next;
            x->next = t->next;
        }
    }
}

void mkdir(char *name){
    if(findInDirectory(cwd,name)){
        printf("Directory Already Exist!");
        return;
    }
    Node *newDir = malloc(sizeof(Node));
    strcpy(newDir->name,name);
    newDir->isDirectory=1;
    newDir->parent=cwd;
    newDir->child=NULL;
    insertNode(cwd,newDir);
    printf("Directory %s created successfully!",newDir->name);
}

void create(char *name){
    if(findInDirectory(cwd,name)){
        printf("File Already Exist!");
        return;
    }
    Node *file = malloc(sizeof(Node));
    strcpy(file->name,name);
    file->isDirectory = 0;
    file->size = 0;
    file->blockCount = 0;
    file->parent = cwd;
    insertNode(cwd,file);
    printf("The file %s created successfully!",name);
}

int allocateBlock() {
    if (!head) return -1;
    Nav *b = head;
    int idx = b->ind;
    head = b->next;
    if (head) head->prev = NULL;
    else tail = NULL;
    free(b);
    return idx;
}

void write(char *name, char *data){
    Node *file = findInDirectory(cwd,name);
    if(!file || file->isDirectory){
        printf("File Do not Exist!");
        return;
    }
    int len = strlen(data);
    int needed = (len + BLOCK_SIZE-1) / BLOCK_SIZE;
    if(needed > BLOCK_PER_FILE){
        printf("This file is too larger!");
        return;
    }
    for (int i = 0; i < needed; i++){
        int b = allocateBlock();
        if(b==-1){
            printf("Disk full!");
            return;
        }
        *(file->blockPointer+i) = b;
        memset(virtualDisk+b,0,BLOCK_SIZE);
        memcpy(virtualDisk+b,data+i*BLOCK_SIZE,BLOCK_SIZE);
    }
    file->blockCount=needed;
    file->size = len;
    printf("Data written successfully!");
}

void read(char *name){
        Node *file = findInDirectory(cwd, name);
    if (!file || file->isDirectory) {
        printf("Invalid file.\n");
        return;
    }
    for (int i = 0; i < file->blockCount; i++)
        printf("%s", virtualDisk+*(file->blockPointer+i));
    printf("\n");
}

void delete(char *name){
    Node *file = findInDirectory(cwd, name);
    if (!file || file->isDirectory) {
        printf("Invalid file.\n");
        return;
    }
    for (int i = 0; i < file->blockCount; i++)
        freeBlock(*(file->blockPointer+i));
    removeNode(cwd, file);
    free(file);
    printf("File deleted successfully.\n");
}

void rmdir(char *name) {
    Node *dir = findInDirectory(cwd, name);
    if (!dir || !dir->isDirectory || dir->child) {
        printf("Directory not empty or invalid.\n");
        return;
    }
    removeNode(cwd, dir);
    free(dir);
    printf("Directory removed successfully.\n");
}

void ls() {
    if (!cwd->child) {
        printf("(empty)\n");
        return;
    }
    Node *t = cwd->child;
    do {
        printf("%s%s\n", t->name, t->isDirectory ? "/" : "");
        t = t->next;
    } while (t != cwd->child);
}

void cd(char *name) {
    if (strcmp(name, "..") == 0) {
        if (cwd->parent) cwd = cwd->parent;
        return;
    }
    Node *dir = findInDirectory(cwd, name);
    if (!dir || !dir->isDirectory) {
        printf("Invalid directory.\n");
        return;
    }
    cwd = dir;
}

void pwd(Node *dir) {
    if (!dir) return;
    if (dir->parent) pwd(dir->parent);
    if (strcmp(dir->name, "/") != 0) printf("/%s", dir->name);
}

void df() {
    int freeCount = 0;
    Nav *t = head;
    while (t) {
        freeCount++;
        t = t->next;
    }
    printf("Total Blocks: %d\n", TOTAL_BLOLCKS);
    printf("Used Blocks: %d\n", TOTAL_BLOLCKS - freeCount);
    printf("Free Blocks: %d\n", freeCount);
}

int main(){
    releaseBlocks();

    root = malloc(sizeof(Node));
    strcpy(root->name, "/");
    root->isDirectory=1;
    root->child=NULL;
    root->parent=NULL;
    cwd=root;

    char cmd[256];
    printf("\nCompact VFS - ready. Type 'exit' to quit.");
    while(1){
        printf("\n$%s>",cwd==root ? "/" : cwd->name);
        fgets(cmd,256,stdin);
        cmd[strcspn(cmd,"\n")]=0;
        if (strncmp(cmd, "mkdir ", 6) == 0) mkdir(cmd + 6);
        else if (strncmp(cmd, "create ", 7) == 0) create(cmd + 7);
        else if (strncmp(cmd, "write ", 6) == 0) {
            char *f = strtok(cmd + 6, " ");
            char *d = strtok(NULL, "\"");
            if (f && d) write(f, d);
        }
        else if (strncmp(cmd, "read ", 5) == 0) read(cmd + 5);
        else if (strncmp(cmd, "delete ", 7) == 0) delete(cmd + 7);
        else if (strncmp(cmd, "rmdir ", 6) == 0) rmdir(cmd + 6);
        else if (strcmp(cmd, "ls") == 0) ls();
        else if (strncmp(cmd, "cd ", 3) == 0) cd(cmd + 3);
        else if (strcmp(cmd, "pwd") == 0) {
            pwd(cwd);
            printf("\n");
        }
        else if (strcmp(cmd, "df") == 0) df();
        else if (strcmp(cmd, "exit") == 0) break;
    }
    return 0;
}