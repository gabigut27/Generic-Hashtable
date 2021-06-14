#include "GenericHashTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Table *createTable(int size, int dType, int listLength)
{   
    if (size<0 || (dType!=INT_TYPE && dType!=STR_TYPE) || listLength<=0){
    printf("Invalid paramerts given. can not initialize the table \n");
    return NULL;
}
    
    Table *newTable = (Table *)calloc(1, sizeof(Table));

    if (newTable == NULL)
    {
        printf("allocation of table failed \n");
        return NULL;
    }
    newTable->originalSize = size;
    newTable->currentSize = size;
    newTable->listLength = listLength;
    newTable->ratio = 1; //the distance each original idx get for hash
    newTable->type = dType;

    newTable->arr = (Object **)calloc(newTable->currentSize, sizeof(Object *));
    if (newTable->arr== NULL)
    {
        printf("allocation of table failed \n");
        return NULL;
    }
    return newTable;
}

void printTable(Table *table)
{   if (table==NULL){
    printf("error. a null table was given as a parameter(func:printTable) \n");
    return;
}
    int i;
    for (i = 0; i < (table->currentSize); i++)
    {
        Object *current = table->arr[i];
        printf("[%d]", i);
        while (current != NULL)
        {
            if (table->type == STR_TYPE)
            {
                printf("    %s  -->", (char *)current->data);
                current = current->next;
            }
            else
            {
                printf("    %d  -->", *(int *)current->data);
                current = current->next;
            }
        }
        if (current == NULL)
        {
            printf("\n");
        }
    }
}

void freeTable(Table *table)
{   if (table==NULL){
    printf("either transfer of table failed or a non exist table was given(freeTable func) \n");
    return;
} 
//free for first the lists. than the array of Object** and then the table
      int i;
    for(i=0; i<table->currentSize; i++){
        freeList(table->arr[i]); 
    }
    free(table->arr);
    free(table);
}

int add(Table *table, void *data)
{      if (table==NULL || data==NULL){
    printf("error. either table is null or data is null(add func) \n");
    return -1;
}
    // if the data type is integer, make pointer of integer and set data's value
    if (table->type == INT_TYPE)
    {
        int *newData = (int *)calloc(1, sizeof(int));
        *newData = *((int *)data);
        int idx = intHashFun(newData, table->originalSize);
        Object *temp = createObject(newData);
        //if we have already an data in this index, so add this to next object that is NULL if the list not full yet
        if (table->arr[idx * (table->ratio)] != NULL)
        {
            int toReturn = handleTable(table, temp, idx);
            return toReturn;
        }
        else
        {
            table->arr[idx * (table->ratio)] = temp;
            return idx * (table->ratio);
        }
        //printf("%d \n", *newData);
    }
    //if the data type is string, make pointer of char* and set the data
    if (table->type == STR_TYPE)
    {
        char *newData = (char *)calloc(strlen(data) + 1, sizeof(int));
        strcpy(newData, (char *)data);
        int idx = strHashFun(newData, table->originalSize);
        Object *temp = createObject(newData);
        //if we have already an data in this index, so add this to next object that is NULL if the list not full yet
        if (table->arr[idx * (table->ratio)] != NULL)
        {
            int toReturn = handleTable(table, temp, idx);
            return toReturn;
        }
        else
        {
            table->arr[idx * (table->ratio)] = temp;
            return idx * (table->ratio);
        }
    }
    printf("it seems that the data was given is not str type and not int type(add func) \n");
    return -1;
}

Object *createObject(void *data)
{   if (data==NULL){
    printf("transfer of the data pointer failed(createObject func) \n");
    return NULL;
}
    Object *head = NULL;
    head = (Object *)malloc(sizeof(Object));
    if (head == NULL)
    {
        printf("creating Object failed(createObject func) \n");
        return NULL;
    }

    head->data = data;
    head->next = NULL;
    return head;
}

int intHashFun(int *key, int origSize)
{   if(origSize<=0){
    printf("error. original size that given is zero or negative(intHashFun func) \n");
}
    int idxReturn = (*key) % origSize;
    return idxReturn < 0 ? idxReturn + origSize : idxReturn;
}

int strHashFun(char *key, int origSize)
{   if(origSize<=0){
    printf("error. original size that given is zero or negative(intHashFun func) \n");
}
    int m = sumascii(key, 0);
    int idxReturn = m % origSize;
    return idxReturn < 0 ? idxReturn + origSize : idxReturn;
}

int multiplyTable(Table *table, Object *object, int idx)
{   if (table==NULL || object==NULL || idx<0){
    printf("either table or object null. or the idx is negative (multyiplyTable func \n");
    return -1;
}       //creating new array of Object** and copy the relevant objects and than make it tables's array.
    Object **newArr = (Object **)calloc((table->currentSize) * 2, sizeof(Object *));

    int i;
    for (i = 0; i < table->currentSize; i++)
    {
        //copy objects for the new array to current index*2
        if (table->arr[i] != NULL)
        {
            Object *temp;
            memcpy(&temp, &table->arr[i], sizeof(Object *));
            newArr[i * 2] = temp;
        }
    }
    
    free(table->arr);
    table->arr = newArr;
    table->currentSize *= 2;
    table->ratio *= 2;
    table->arr[(idx * table->ratio) + 1] = object;
    
    return (idx * table->ratio) + 1;
}

int handleTable(Table *table, Object *object, int idx)
{   if (table==NULL || object==NULL || idx<0){
    printf("either table or object null. or the idx is negative (handelTable func) \n");
    return -1;
}
    Object *current;
    int maxObjInlist = table->listLength;
    int ratioToRun = table->ratio;
    int countObj;
    int i;
    //running on the distance each idx hash of the original size has at current size
    for (i = 0; i < ratioToRun; i++)
    {
        countObj = 0;
        current = table->arr[(idx * ratioToRun) + i];
        if (current == NULL)
        { //if the list is empty
            table->arr[(idx * ratioToRun) + i] = object;
            return (idx * ratioToRun) + i;
        }
        //we already have in that idx(list) at least one obj
        if (current != NULL)
        {
            countObj = 1;
            while (current->next != NULL && countObj < maxObjInlist)
            {
                current = current->next;
                countObj++;
            }
            //if we at the last idx we can run that the original idx contain(ratio) and the list is full
            if ((i == ratioToRun - 1) && (countObj == maxObjInlist)) 
            {   //we need to double the array
                int toReturn = multiplyTable(table, object, idx);
                return toReturn;
            }
            // if we found a space to push an object in existing list
            if (countObj < maxObjInlist) 
            {
                current->next = object;
                return (idx * ratioToRun) + i;
            }
        }
    }
}

int sumascii(char str[], int num)
{
    if (num < strlen(str))
    {
        return (str[num] + sumascii(str, num + 1));
    }

    return 0;
}

 Object* search(Table* table, void* data){
     if (table==NULL || data==NULL){
         printf("error. either table is null or data is null(search func) \n");
         return NULL;
     }
int i;
int check;
for (i=0; i<table->currentSize; i++){
Object * current=table->arr[i];
if (current!=NULL){
    check=isEqual(table->type, data, current->data );
    if(check==0)
    return current;
    
    while(current->next!=NULL){
        current=current->next;
        check=isEqual(table->type, data, current->data );
        if(check==0)
    return current;
    }
}
} 
printf("Object not found \n");
return NULL;
 }
//if return 0 its equal and if -1 not equal
int isEqual(int type, void *data1, void *data2)
{   if(data1==NULL || data2==NULL){
    printf("error. data1 or data2 is null (isEqual func) \n");
    return -1;
}
    if (type == INT_TYPE)
    {
        int first = *(int *)data1;
        int second = *(int *)data2;
        if (first == second)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    if (type == STR_TYPE)
    {
        
       int result = strcmp((char *)data1, (char *)data2);
        if (result == 0)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    printf("error. it seems the type was given is not equal to int type or str type (isEqual func) \n");
    return -1;
}

void freeObject(Object* obj, int type){
    free(obj->data);
    free(obj);
}

void freeList(Object* obj)
{
    Object* temp;

   while (obj != NULL)
    {
       temp = obj;
       obj = obj->next;
       freeObject(temp, 0); //second parameter has no effect
    }

}

int removeObj(Table* table, void* data){
    if(table==NULL || data==NULL){
        printf("error. either table is null or data(removeObj func) \n");
        return -1;
    }
    Object * current;
    Object *previous;
    int i;
    for (i=0; i<table->currentSize; i++){
        current=table->arr[i];
        if (current!=NULL){
            //this is the header
            if(isEqual(table->type, data, current->data )==0){
                if (current->next!=NULL){
            table->arr[i]=current->next;
            freeObject(current, table->type);
            return i;
                }
            else
            {
                table->arr[i]=NULL;
                freeObject(current, table->type);
                return i;
            }
            
            }
            while(current->next!=NULL){
                previous=current;
                current=current->next;
                //this is in the middle or in the end
                if(isEqual(table->type, data, current->data )==0){
                    if (current->next!=NULL){
                    previous->next=current->next;
                    freeObject(current, table->type);
                    return i;
                    }
                    else{
                        previous->next=NULL;
                        freeObject(current, table->type);
                        return i;
                    }
                }
            }
        }
        
    }
    printf("Object not found");
    return -1;
}