#include "HashTable.h"

struct Node{
    char key[MAX_NAME_LENGTH];
    int value;
    struct  Node* next;
    struct  Node* prev;
};


struct Hash_table{
    struct Node *table[TABLE_SIZE];
};

void Initial_table(struct Hash_table* hs) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        hs->table[i] = NULL;
    }
}

//asc取值相加后取模；链地址法解决冲突问题
unsigned int hashFunction(const char* key){
    int hash=0;
    while(*key){
        hash += (int)*key;
//      printf("The value if the hash is %d\n",hash);
        key++;
    }
    return hash%TABLE_SIZE;
}


//查找key对应的节点
struct Node* findNode(struct Hash_table* hs,const char* key){
    int index=hashFunction(key);
    struct Node* currentNode=hs->table[index];
    while(currentNode!=NULL){
        if(strcmp(currentNode->key,key)==0){
            return currentNode;
        }
        currentNode=currentNode->next;
    }
    
    //printf("The node corresponding to the key could not be found.\n");

    //抛出异常
    return NULL;
}


//更新key对应的value
void update(struct Hash_table* hs,const char* key,int newValue ){
    struct Node* currentNode;
    currentNode=findNode(hs,key);
    printf("The ket of currentNode is%s,value为%d",currentNode->key,currentNode->value);
    strcpy(currentNode->key,key);
    currentNode->value=newValue;
}



void insert(struct Hash_table* hs,const char* key,int value){
    struct Node* currentNode;
    currentNode=findNode(hs,key);
    int index=hashFunction(key);
    if(currentNode==NULL){
        struct Node* newNode=(struct Node*)malloc(sizeof(struct Node));
        strcpy(newNode->key,key);
        newNode->value=value;
        newNode->next=NULL;
        if (hs->table[index]==NULL){
            hs->table[index]=newNode;
            }else{
                newNode->next=hs->table[index];
                hs->table[index]=newNode;
                }
    }else{
        //抛出异常
        printf("The node is already exsist");
    }
}

//查找键对应的值
int search(struct Hash_table* hs,char* key){
    int index=hashFunction(key);
    struct Node* currentNode=hs->table[index];
    while(currentNode!=NULL){
        if(strcmp(currentNode->key,key)==0){
            printf("%s对应的value为%d",key,hs->table[index]->value);
            return currentNode->value;
        }
        currentNode=currentNode->next;
    }
    return -1;
}

//LRU算法，常找的槽，常找的key


void delete_(struct Hash_table* hs, const char* key) {
    int index = hashFunction(key);
    struct Node* currentNode = findNode(hs, key);

    if (currentNode != NULL) {
        // 处理头节点
        if (currentNode->prev == NULL) {
            hs->table[index] = currentNode->next;
            if (currentNode->next != NULL) {
                currentNode->next->prev = NULL;
            }
        } else {
            // 处理非头节点
            currentNode->prev->next = currentNode->next;
            if (currentNode->next != NULL) {
                currentNode->next->prev = currentNode->prev;
            }
        }

        
    } else {
        // 节点未找到
        printf("The node corresponding to the key could not be found.\n");
    }
}


void print(struct Hash_table* ht,const char* key){
    int index=hashFunction(key);
    printf("key:%s,value:%d\n",key,ht->table[index]->value);
}

