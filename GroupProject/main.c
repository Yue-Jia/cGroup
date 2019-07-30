

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//remove new line '\n' and return '\r' characters
#define REMOVERN(str) str[strcspn(str,"\r\n")] = 0;

// The maximum number of quotes we can read from input file
#define MAX_QUOTES 10050

// The maximum size of the string in the file.
#define MAX_LEN 1000

typedef struct runner{
    char* name;
    char gender;
    char country[3];
    int bib;
    int record;
}runnerType,*runner;

void readFile(char**, size_t*);
void print(char**, size_t);

int main() {
    // Create array of pointers. Each pointer should point to heap memory where
    // each quote is kept. I.e. arr[0] points to quote N1 saved on the heap.
    char *arr[MAX_QUOTES];

    // Number of quotes in the file quotes.txt. We assume initially that file has MAX_QUOTES quotes.
    // Must be adjusted in the function read_in when the file is read!
    size_t size=MAX_QUOTES; 

    // Read quotes from file and place them into array arr. 
    // Note: size reflects how many quotes we read from the file (between 0 and MAX_QUOTES).
    readFile(arr, &size);

    //selection_sort(arr, size);
    
    print(arr, size);
    //write_out(arr, size);
    
    //free_memory(arr, size);
    return 0;
}
void readFile(char** arr, size_t* quotes_count){
    FILE *fp;
    fp = fopen("data.txt","r");
    if(fp == NULL){
        printf("Error reading file");
        exit(1);
    }
    int i = 0;
    while(!feof(fp) && i+1 <= MAX_QUOTES){
        char temp[MAX_LEN];
        if(fgets(temp,sizeof(temp),fp)){
            REMOVERN(temp);
            if (temp[0]){
                arr[i] = (char*) calloc(strlen(temp)+1,sizeof(char));
                if(arr[i] == NULL){
                    printf("Cannot allocate memory for quotes No.%d", i);
                    exit(1);
                }
                strncpy(arr[i],temp,strlen(temp));
                i++;
            }
        }
    }
    if(i==MAX_QUOTES){
        printf("Warning: Number of quotes in the file exceeds MAX_QUOTES allowed by this program. Partial of them will be processed.\n");
    }
    *quotes_count = i;
    fclose(fp);
}
void print(char** arr, size_t quotes_count){
    for(int i = 0;i < quotes_count; i++){
        printf("%s\n",arr[i]);
    }
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
