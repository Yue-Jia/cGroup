

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//remove new line '\n' and return '\r' characters
#define REMOVERN(str) str[strcspn(str,"\r\n")] = 0;

// The maximum size of the string in the file.
#define MAX_LEN 1000

typedef struct runner{
    char* name;
    char gender;
    char country[4];
    int bib;
    int time_5k;
    int time_10k;
    int time_15k;
    int time_official;
    struct runner* next;
    struct runner* prev;
}runnerType,*runner;

void readFile(runner*,runner*, int*);
void print(runner);
char* tts(int);
runner* refreshBibArray(runner, int);

int main() {
    // Create head pointers. This pointer should point to the first node of the linked list
    // The linked list should be saved on the heap.
    runner head = NULL;
    runner tail = NULL;

    // Number of quotes in the file quotes.txt. We assume initially that file has MAX_QUOTES quotes.
    // Must be adjusted in the function read_in when the file is read!
    int size=0; 

    // Read quotes from file and place them into array arr. 
    // Note: size reflects how many quotes we read from the file (between 0 and MAX_QUOTES).
    readFile(&head, &tail, &size);

    //selection_sort(arr, size);
    //before refresh, remember to free the memory
    runner* bibArray = refreshBibArray(head,size);
        for(int i=0;i<size;i++){
        printf("%6d ",bibArray[i]->bib);
    }
    //print(tail);
    //write_out(arr, size);
    
    //free_memory(arr, size);
    return 0;
}
void readFile(runner* head,runner*tail, int* count){
    FILE *fp;
    fp = fopen("data.txt","r");
    if(fp == NULL){
        printf("Error reading file");
        exit(1);
    }
    while(!feof(fp)){
        char temp[MAX_LEN];
        if(fgets(temp,sizeof(temp),fp)){
            REMOVERN(temp);
            if (temp[0]){
                runner node = (runner) malloc(sizeof(runnerType));
                if(node == NULL){
                    printf("Cannot allocate memory for more runner");
                    return;
                }
                char* token;
                //bib
                token = strtok (temp,"\t");
                node->bib = (int) strtol(token,NULL,10);
                //name
                token = strtok (NULL, "\t");
                node->name = (char*) calloc(strlen(token)+1,sizeof(char));
                strncpy(node->name, token, strlen(token));
                //gender
                token = strtok (NULL, "\t");
                node->gender = token[0];
                //country
                token = strtok (NULL, "\t");
                strncpy(node->country,token,strlen(token));
                //time_5k
                token = strtok (NULL,"\t");
                node->time_5k = (int) strtol(token,NULL,10);
                //time_10k
                token = strtok (NULL,"\t");
                node->time_10k = (int) strtol(token,NULL,10);
                //time_15k
                token = strtok (NULL,"\t");
                node->time_15k = (int) strtol(token,NULL,10);
                //time_official
                token = strtok (NULL,"\t");
                node->time_official = (int) strtol(token,NULL,10);
                //next
                node->next = NULL;
                *count = *count + 1;
                if(*count == 1){
                    *head = node;
                    *tail=node;
                }else{
                    (*head)->prev= node;
                    node->next = *head;
                    *head = node;
                }
            }
        }
    }
    printf("%d records loaded.",*count);
    fclose(fp);
}

char* tts(int n){
    static char str[9]={0};
    snprintf(str,9, "%2d:%2d:%2d", n/3600,(n%3600)/60,n%60);
    for(int i=1;i<8;i++){
        if(str[i] == ' '){
            str[i]='0';
        }
    }
    return str;
}

void print(runner head){
    runner current = head;
    while(current!=NULL){
        printf("%6d %-30s%2c%4s%9s",current->bib,current->name,current->gender,current->country,tts(current->time_5k));
        printf("%9s",tts(current->time_10k));
        printf("%9s",tts(current->time_15k));
        printf("%9s\n",tts(current->time_official));
        current=current->prev;
    }
}
void swap(runner* a, runner* b){
    runner temp = *a;
    *a = *b;
    *b = temp;
}
int Partition(runner* a, int low,int high){
    int i=low;
    for(int j = low;j<high;j++){
        if((a[j])->bib < (a[high])->bib){
            //swap
            swap(&a[i],&a[j]);
            i++;
        }
    }
    swap(&a[high],&a[i]);
    return i;
}
void QuickSort(runner* a, int low, int high){
    if(low<high){
        int p = Partition(a,low,high);
        QuickSort(a,low,p-1);
        QuickSort(a,p+1,high);
    }
}
runner* refreshBibArray(runner head, int size){
    runner current = head;
    runner* bibArray = (runner*) malloc(size*sizeof(runner));
    for(int i=0;i<size;i++){
        bibArray[i] = current;
        current = current->next;
    }

    QuickSort(bibArray,0,size-1);
    return bibArray;
}

void add(){

}
void detail(){

}
void edit(){

}
void delete(){

}
runner searchName(char* name){

}
runner searchBib(int number){

}
void sort(){

}
