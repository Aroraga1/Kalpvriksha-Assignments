#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define TOTAL_BLOCKS 1024
#define BLOCKS_PER_FILE 100
#define SIZE_OF_BLOCK 512

typedef struct Node{
    char name[51];
    int isDirectory;
    int size;
    int blockCount;
    int blockPointer[BLOCKS_PER_FILE];
    struct Node *parent;
    struct Node *child;
    struct Node *next;
} Node;


typedef struct disk{
    int index;
    struct disk *prev;
    struct disk *next;
} disk;

struct disk *head = NULL;
struct disk *tail = NULL;
struct Node *root = NULL;
struct Node *cwd = NULL;
char VD[TOTAL_BLOCKS][SIZE_OF_BLOCK]; 

void allocateDisk();
void mkdir(char *name);
struct Node *findDir(struct Node *dir,char *name);
void create(char *name);
void read(char *name);
void write(char *name, char *data);
void pwd();
void rmdir(char *name);
void df();
void ls();
void cd(char *name);
void insertNode(struct Node *dir,struct Node *node);
int allocateBlock();
void removeNode(struct Node *cwd,struct Node *node);
void freeBlocks(int index);

void allocateDisk(){
   for (int i = 0; i < TOTAL_BLOCKS; i++)
   {
    struct disk *block = malloc(sizeof(struct disk));
    block->index = i;
    block->next = NULL;
    block->prev = tail;
    if(tail) tail->next = block;
    else head = block;
    tail = block; 
   }
}

struct Node *findDir(struct Node *dir,char *name){
    if(!dir->child) return NULL;
    struct Node *t = dir->child;
    do{
        if(strcmp(t->name,name)==0) return t;  
        t = t->next;
    }while(t!=dir->child);
    return NULL;
}

void insertNode(struct Node *dir,struct Node *node){
    if(dir->child){
        struct Node *t = dir->child;
        while(t->next!=dir->child) t = t->next;
        t->next = node;
        node->next = dir->child;
    }else{
        dir->child = node;
        node->next = dir->child;
    }
}

void mkdir(char *name){
    if(findDir(cwd,name)){
        printf("Dir already Exists!\n");
        return;
    }
    struct Node *t = malloc(sizeof(struct Node));
    strcpy(t->name,name);
    t->isDirectory=1;
    t->child=NULL;
    t->parent = cwd;
    insertNode(cwd,t);
    printf("Directory created successfully!\n");
}

void create(char *name){
    if(findDir(cwd,name)){
        printf("same File/Directory already Exists!\n");
        return;
    }
    struct Node *t = malloc(sizeof(Node));
    strcpy(t->name,name);
    t->isDirectory=0;
    t->child=NULL;  
    t->parent=cwd;  
    insertNode(cwd,t);
    printf("File created successfully\n");
}

int allocateBlock(){
    if(!head) return -1;
    struct disk *t = head;
    int index = t->index;
    head = t->next;
    if(head) head->prev = NULL;
    else tail = NULL;
    free(t);
    return index;
}

void write(char *name, char *data){
    struct Node *f = findDir(cwd,name);
    if(!f || f->isDirectory==1) printf("Invailid input!\n");
    int len = strlen(data);
    int capturesBlock = (len+SIZE_OF_BLOCK-1)/SIZE_OF_BLOCK;
    if(capturesBlock>BLOCKS_PER_FILE){
        printf("File is too long!\n");
        return;
    }
    for(int i=0; i<capturesBlock; i++){
        int block = allocateBlock();
        if(block==-1){
            printf("Disk is full!\n");
            return;
        }
        *(f->blockPointer+i) = block;
        memset(VD+block,0,SIZE_OF_BLOCK);
        memcpy(VD+block,data+i*SIZE_OF_BLOCK,SIZE_OF_BLOCK);
    }
    f->blockCount=capturesBlock;
    f->size=len;
    printf("Data written successfully!\n");
}

void read(char *name){
    struct Node *f = findDir(cwd,name);
    if(!f || f->isDirectory==1){
        printf("Invailid Input!\n");
        return;
    }
    for(int i=0; i<f->blockCount; i++){
        printf("%s\n",VD+(*(f->blockPointer+i)));
    }
}

void removeNode(struct Node *dir,struct Node *node){
    struct Node *t = dir->child; 
    while(t->next!=node){
        t = t->next;
        if(t==dir->child) return;
    } 
    t->next = node->next;
    if(node==dir->child) dir->child = (node->next==node) ? NULL : node->next;
}

void freeBlocks(int index){
    struct disk *t = malloc(sizeof(struct disk));
    t->index = index;
    t->next=NULL;
    t->prev=tail;
    if(tail) tail->next = t;
    else head = t;
    tail = t;
}

void delete(char *name){
    struct Node *f = findDir(cwd,name);
    if(!f || f->isDirectory==1){
        printf("Invailid Input!\n");
        return;
    }
    for(int i=0; i<f->blockCount; i++){
        freeBlocks(*(f->blockPointer+i));
    }
    removeNode(cwd,f);
    free(f);
    printf("File deleted successfully!\n");
}

void rmdir(char *name){
    struct Node *t = findDir(cwd,name);
    if(!t || t->isDirectory==0 || t->child){
        printf("Invailid Input!\n");
        return;
    }
    removeNode(cwd,t);
    free(t);
    printf("Directory deleted successfully!\n");
}

void ls(){  
    if(!cwd->child){
        printf("Directory is empty\n");
        return;
    }
    struct Node *t = cwd->child;
    do{
        printf("%s\n",t->name);
        t=t->next;
    }while(t!=cwd->child);
}

void cd(char *name){
    if(strcmp(name,"..")==0){
        if(cwd->parent) cwd = cwd->parent;
        return;
    }
    struct Node *t = findDir(cwd,name);
    if(!t || t->isDirectory==0){
        printf("Invailid input!\n");
        return;
    }
    cwd=t;
}

void pwd(){
    if(cwd->parent && cwd!=root) printf("%s\n",cwd->parent->name);
}

void df() {
    int count = 0;
    struct disk *d = head;
    while(d) {
        count++;
        d = d->next;
    }
    int used = TOTAL_BLOCKS - count;
    float usagePercent = ((float)used / TOTAL_BLOCKS) * 100;

    printf("total Blocks: %d\n", TOTAL_BLOCKS);
    printf("used Blocks: %d\n", used);
    printf("free Blocks: %d\n", count);
    printf("disk Usage: %.2f%%\n", usagePercent);
}

int main(){
    allocateDisk();
    root = malloc(sizeof(struct Node));
    strcpy(root->name,"/");
    root->isDirectory = 1;
    root->child = NULL;
    root->parent = NULL;
    cwd = root;
    char cmd[256];
    printf("VFM is Ready to operate:\n");
    while(1){
        struct Node *t = root;
        printf("\n$%s>",(cwd==root) ? "/" : cwd->name);
        fgets(cmd,256,stdin);
        cmd[strcspn(cmd, "\n")] = 0;
        if(strncmp(cmd,"mkdir ",6)==0) mkdir(cmd+6);
        else if(strncmp(cmd,"create ",7)==0) create(cmd+7);
        else if(strncmp(cmd,"write ",6)==0){
            char *f = strtok(cmd+6, " ");
            char *d = strtok(NULL,"\"");
            if(f && d) write(f,d);
        }
        if(strncmp(cmd,"read ",5)==0) read(cmd+5);
        else if(strncmp(cmd,"delete ",7)==0) delete(cmd+7);
        else if(strncmp(cmd,"rmdir ",6)==0) rmdir(cmd+6);
        else if(strncmp(cmd,"cd ",3)==0) cd(cmd+3);
        else if(strcmp(cmd,"ls")==0) ls();
        else if(strcmp(cmd,"pwd")==0) pwd();
        else if(strcmp(cmd,"df")==0) df();
        else if(strcmp(cmd,"exit")==0) break;
        else continue;
    }
    return 0;
}
