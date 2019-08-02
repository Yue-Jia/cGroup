

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//remove new line '\n' and return '\r' characters
#define REMOVERN(str) str[strcspn(str,"\r\n")] = 0;

// The maximum size of the string in the file.
#define MAX_LEN 1000
#define FLUSH stdin=freopen(NULL,"r",stdin)

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
    // Create head and tail pointers.
    // These pointers should point to the first node and last node of the double linked list
    // The linked list should be saved in heap.
    runner head = NULL;
    runner tail = NULL;

    // Number of runners in the file data.txt.
    // Must be updated after INSERT or DELETE operation!
    int size=0; 

    // Read runners data from file and place them into double linked list. 
    // Note: size reflects how many runners we read from the file.
    readFile(&head, &tail, &size);

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

void add(runner* head,runner* tail){
    char*tem = (char*) calloc(MAX_LEN, sizeof (char));
    int tempp;
    char ttt[MAX_LEN];
    runner tempr = (runner) malloc(sizeof (runnerType));
    if (tempr == NULL) {
        printf("Can not get space from heap for new runner node.");
        exit(1);
    }
    tempr->time_10k = tempr->time_15k = tempr->time_5k = 0;
    printf("Please enter the runner name:");
    fgets(tem, MAX_LEN, stdin);
    REMOVERN(tem);
    strncpy(tempr->name, tem, strlen(tem));
    FLUSH;
    printf("Please enter the runner gender:");
    fgets(tem, MAX_LEN, stdin);
    REMOVERN(tem);
    tempr->gender = tem[0];
    FLUSH;
    printf("Please enter the runner country:");
    fgets(tem, MAX_LEN, stdin);
    REMOVERN(tem);
    tem[3] = '\0';
    strncpy(tempr->country, tem, strlen(tem) + 1);
    FLUSH;
    do {
        printf("Please enter the runner bib number:");
        fgets(ttt, MAX_LEN, stdin);
        tempp = strtol(ttt, NULL, 10);
        FLUSH;
    } while (!tempp);
    tempr->bib = tempp;
    //scanf("%d",&(tempr->bib)); 
    do {
        printf("Please enter the runner official time:");
        //scanf("%d",&(tempr->time_official));
        fgets(ttt, MAX_LEN, stdin);
        tempp = strtol(ttt, NULL, 10);
        FLUSH;
    } while (!tempp);
    tempr->time_official = tempp;
    tempr->next = NULL;
    tempr->prev = NULL;
    //insert into right position
    runner curr = *head;
    while (((tempr->time_official)<(curr->time_official))&&(curr->next != NULL)) {
        curr = curr->next;
    }
    if (curr->next == NULL) {
        if (((tempr->time_official)<(curr->time_official))) {
            curr->next = tempr;
            tempr->prev = curr;
            (*tail) = tempr;
        } else {
            tempr->prev = curr->prev;
            tempr->next = curr;
            curr->prev->next = tempr;
            curr->prev = tempr;
        }
    } else {
        if (tempr->time_official >= (*head)->time_official) {
            (*head)->prev = tempr;
            tempr->next = (*head);
            (*head) = tempr;
        } else {
            tempr->prev = curr->prev;
            tempr->next = curr;
            curr->prev->next = tempr;
            curr->prev = tempr;
        }
    }

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
