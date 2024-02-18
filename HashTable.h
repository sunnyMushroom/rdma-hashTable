#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 20
#define MAX_NAME_LENGTH 20

void update(struct Hash_table* hs,const char* key,int newValue );
void insert(struct Hash_table* hs,const char* key,int value);
int search(struct Hash_table* hs,char* key);
void delete_(struct Hash_table* hs, const char* key);