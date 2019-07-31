

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
    
    print(tail);
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
void print(runner head){
    runner current = head;
    while(current!=NULL){
        printf("%6d %-30s%2c%4s%6d%6d%6d%6d\n",current->bib,current->name,current->gender,current->country,current->time_5k,current->time_10k,current->time_15k,current->time_official);
        current=current->prev;
    }
}
void timeToString(int n){
    
}
void add(){

}
void detail(){

}
void edit(){

}
void delete(){

}
int search(){

}
void sort(){

}
