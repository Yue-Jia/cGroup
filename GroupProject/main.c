#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

//remove new line '\n' and return '\r' characters
#define REMOVERN(str) str[strcspn(str,"\r\n")] = 0;

// The maximum size of the string in the file.
#define MAX_LEN 1000
#define TABLESIZE 30671
#define FLUSH stdin=freopen(NULL,"r",stdin)

typedef struct runner {
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
} runnerType, *runner;

typedef struct hashTableNode {
    runner ptr;
    struct hashTableNode* next;
} htNodeType, *htNode;

void readFile(runner*, runner*, int*);
void print(runner);
void printBibArray(runner*);
void printRunner(runner);
char* tts(int);
runner* refreshBibArray(runner, int);
int searchBibIndex(runner*, int);
bool checkBib(runner*, int);
runner searchBib(runner*, int);
htNode* refreshNameHashTable(runner);
unsigned int hashFunction(unsigned int);
void htInsert(htNode*, char*, runner);
unsigned int keyToInt(char*);
runner searchName(htNode*, char*);
void delete(runner*, runner*, runner*);
void free_nameTable(htNode*);
void free_memory(runner, runner*, htNode*);
void writeFile(runner);

//global variable
int size = 0;

int main() {
    // Create head and tail pointers.
    // These pointers should point to the first node and last node of the double linked list
    // The linked list should be saved in heap.
    runner head = NULL;
    runner tail = NULL;

    // Number of runners in the file data.txt.
    // Must be updated after INSERT or DELETE operation!

    // Read runners data from file and place them into double linked list. 
    readFile(&head, &tail, &size);


    //before refresh, remember to free the memory
    runner* bibArray = refreshBibArray(head, size);
    //search bib, print detail
    //searchBib(bibArray,141);

    //Create name hash table
    htNode* nameTable = refreshNameHashTable(head);
    //search name, print detail
    //searchName(nameTable, "Juntong Hou");

    //Command Line Interface
    while (true) {
        printf("----------Main Menu----------\n");
        printf("|Q| Quit |B| List by Bib ASC |O| List by Official Time ASC\n");
        printf("|S| Search by Bib |N| Search by Name\n");
        printf(">");
        char userInput[MAX_LEN] = {0};
        fgets(userInput, MAX_LEN, stdin);
        FLUSH;
        REMOVERN(userInput);
        if (strlen(userInput) != 1) {
            continue;
        } else if (userInput[0] == 'q' || userInput[0] == 'Q') {
            break;
        } else if (userInput[0] == 'b' || userInput[0] == 'B') {
            printBibArray(bibArray);
        } else if (userInput[0] == 'o' || userInput[0] == 'O') {
            print(tail);
        } else if (userInput[0] == 'n' || userInput[0] == 'N') {
            //Search by Name
            while (true) {
                printf("----------Search by Name----------\n");
                printf("|Q| Quit\n");
                printf("Please enter the runner name:");
                fgets(userInput, MAX_LEN, stdin);
                FLUSH;
                REMOVERN(userInput);
                if (strlen(userInput) == 1 && (userInput[0] == 'q' || userInput[0] == 'Q')) {
                    break;
                } else {
                    runner temp = searchName(nameTable, userInput);
                    if (temp != NULL) {
                        printRunner(temp);
                        while (true) {
                            printf("|Q| Quit |E| Edit |D| Delete\n");
                            printf(">");
                            fgets(userInput, MAX_LEN, stdin);
                            FLUSH;
                            REMOVERN(userInput);
                            if (strlen(userInput) == 1 && (userInput[0] == 'q' || userInput[0] == 'Q')) {
                                break;
                            } else if (userInput[0] == 'e' || userInput[0] == 'E') {
                                //invoke edit here
                                printRunner(temp);
                            } else if (userInput[0] == 'd' || userInput[0] == 'D') {
                                //invoke delete here
                                delete(&temp, &head, &tail);
                                //update bibArray
                                free(bibArray);
                                bibArray = refreshBibArray(head, size);
                                //update nameTable
                                free_nameTable(nameTable);
                                nameTable = refreshNameHashTable(head);
                                break;
                            }
                        }
                    }
                }
            }
        } else if (userInput[0] == 's' || userInput[0] == 'S') {
            //Search by Bib
            while (true) {
                printf("----------Search by Bib----------\n");
                printf("|Q| Quit\n");
                printf("Please enter the bib number:");
                fgets(userInput, MAX_LEN, stdin);
                FLUSH;
                REMOVERN(userInput);
                if (strlen(userInput) == 1 && (userInput[0] == 'q' || userInput[0] == 'Q')) {
                    break;
                } else {
                    int userInputBib = strtol(userInput, NULL, 10);
                    if (userInputBib <= 0) {
                        printf("Incorrect input. Please try again.\n");
                        continue;
                    } else {
                        runner temp = searchBib(bibArray, userInputBib);
                        if (temp != NULL) {
                            printRunner(temp);
                            while (true) {
                                printf("|Q| Quit |E| Edit |D| Delete\n");
                                printf(">");
                                fgets(userInput, MAX_LEN, stdin);
                                FLUSH;
                                REMOVERN(userInput);
                                if (strlen(userInput) == 1 && (userInput[0] == 'q' || userInput[0] == 'Q')) {
                                    break;
                                } else if (userInput[0] == 'e' || userInput[0] == 'E') {
                                    //invoke edit here
                                    printRunner(temp);
                                } else if (userInput[0] == 'd' || userInput[0] == 'D') {
                                    //invoke delete here
                                    delete(&temp, &head, &tail);
                                    //update bibArray
                                    free(bibArray);
                                    bibArray = refreshBibArray(head, size);
                                    //update nameTable
                                    free_nameTable(nameTable);
                                    nameTable = refreshNameHashTable(head);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //print(head);
    //writeFile(head);
    free_memory(head, bibArray, nameTable);
    return 0;
}

void writeFile(runner head) {
    FILE *fp;
    fp = fopen("data2.txt", "w");
    if (fp == NULL) {
        printf("Error reading file");
        return;
    }
    runner current = head;
    while (current != NULL) {
        fprintf(fp, "%d\t%s\t%c\t%s\t%d\t%d\t%d\t%d\r\n", current->bib, current->name, current->gender, current->country, current->time_5k, current->time_10k, current->time_15k, current->time_official);
        current = current->next;
    }
    fclose(fp);
}

void readFile(runner* head, runner* tail, int* count) {
    FILE *fp;
    fp = fopen("data2.txt", "r");
    if (fp == NULL) {
        printf("Error reading file");
        exit(1);
    }
    while (!feof(fp)) {
        char temp[MAX_LEN];
        if (fgets(temp, sizeof (temp), fp)) {
            REMOVERN(temp);
            if (temp[0]) {
                runner node = (runner) malloc(sizeof (runnerType));
                if (node == NULL) {
                    printf("Cannot allocate memory for more runner");
                    return;
                }
                char* token;
                //bib
                token = strtok(temp, "\t");
                node->bib = (int) strtol(token, NULL, 10);
                //name
                token = strtok(NULL, "\t");
                node->name = (char*) calloc(strlen(token) + 1, sizeof (char));
                strncpy(node->name, token, strlen(token));
                //gender
                token = strtok(NULL, "\t");
                node->gender = token[0];
                //country
                token = strtok(NULL, "\t");
                strncpy(node->country, token, strlen(token));
                //time_5k
                token = strtok(NULL, "\t");
                node->time_5k = (int) strtol(token, NULL, 10);
                //time_10k
                token = strtok(NULL, "\t");
                node->time_10k = (int) strtol(token, NULL, 10);
                //time_15k
                token = strtok(NULL, "\t");
                node->time_15k = (int) strtol(token, NULL, 10);
                //time_official
                token = strtok(NULL, "\t");
                node->time_official = (int) strtol(token, NULL, 10);
                //next
                node->next = NULL;
                node->prev = NULL;
                *count = *count + 1;
                if (*count == 1) {
                    *head = node;
                    *tail = node;
                } else {
                    node->next = *head;
                    (*head)->prev = node;
                    *head = node;
                }
            }
        }
    }
    printf("%d records loaded.\n", *count);
    fclose(fp);
}

int searchBibIndex(runner* bibArray, int bib) {
    int low = 0;
    int high = size - 1;
    int mid;
    int key = -1;
    while (low <= high) {
        mid = (low + high) / 2;
        if (bib == bibArray[mid]->bib) {
            key = mid;
            break;
        }
        if (bib < bibArray[mid]->bib) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return key;
}

bool checkBib(runner* bibArray, int bib) {
    int index = searchBibIndex(bibArray, bib);
    if (index != -1) {
        return true;
    } else {
        return false;
    }
}

runner searchBib(runner* bibArray, int bib) {
    runner temp = NULL;
    int index = searchBibIndex(bibArray, bib);
    if (index != -1) {
        temp = bibArray[index];
    } else {
        printf("Cannot find the runner with bib number = %d\n", bib);
    }
    return temp;
}

htNode* refreshNameHashTable(runner head) {
    runner current = head;
    //allocate memory
    htNode* nameTable = (htNode*) malloc(TABLESIZE * sizeof (htNode));
    //clean table
    for (int i = 0; i < TABLESIZE; i++) {
        nameTable[i] = NULL;
    }
    //insert data into table
    while (current != NULL) {
        htInsert(nameTable, current->name, current);
        current = current->next;
    }
    printf("Runner name hash table updated.\n");
    return nameTable;
}

unsigned int hashFunction(unsigned int k) {
    double goldenRatio = (sqrt(5) - 1) / 2;
    return floor(TABLESIZE * (fmod(k * goldenRatio, 1)));
}

unsigned int keyToInt(char* key) {
    unsigned int n = 0;
    for (int i = 0; i < strlen(key); i++) {
        n = n * 32 + key[i];
    }
    return n;
}

void htInsert(htNode* nameTable, char* key, runner r) {
    int index = hashFunction(keyToInt(key));
    htNode temp = nameTable[index];
    nameTable[index] = (htNode) malloc(sizeof (htNodeType));
    if (nameTable[index] != NULL) {
        (nameTable[index])->ptr = r;
        (nameTable[index])->next = temp;
    } else {
        printf("Cannot allocate memory for hash table separating chain node");
    }
}

htNode htsearch(htNode* nameTable, char* key) {
    int index = hashFunction(keyToInt(key));
    htNode temp = nameTable[index];
    while (temp != NULL && strcmp(temp->ptr->name, key) != 0) {
        temp = temp->next;
    }
    return temp;
}

runner searchName(htNode* nameTable, char* key) {
    htNode h = htsearch(nameTable, key);
    if (h != NULL) {
        return (h->ptr);
    } else {
        printf("Cannot find the runner with this name \"%s\"\n", key);
        return NULL;
    }
}

char* tts(int n) {
    static char str[9] = {0};
    snprintf(str, 9, "%2d:%2d:%2d", n / 3600, (n % 3600) / 60, n % 60);
    for (int i = 1; i < 8; i++) {
        if (str[i] == ' ') {
            str[i] = '0';
        }
    }
    return str;
}

void print(runner head) {
    runner current = head;
    while (current != NULL) {
        printf("%6d %-30s%2c%4s%9s", current->bib, current->name, current->gender, current->country, tts(current->time_5k));
        printf("%9s", tts(current->time_10k));
        printf("%9s", tts(current->time_15k));
        printf("%9s\n", tts(current->time_official));
        if (head->prev == NULL) {
            current = current->next;
        } else {
            current = current->prev;
        }
    }
}

void printBibArray(runner* bibArray) {
    for (int i = 0; i < size; i++) {
        printf("%6d %-30s%2c%4s%9s", bibArray[i]->bib, bibArray[i]->name, bibArray[i]->gender, bibArray[i]->country, tts(bibArray[i]->time_5k));
        printf("%9s", tts(bibArray[i]->time_10k));
        printf("%9s", tts(bibArray[i]->time_15k));
        printf("%9s\n", tts(bibArray[i]->time_official));
    }
}

void printRunner(runner current) {
    printf("----------Detail----------\n");
    printf("Bib Number: %d\n", current->bib);
    printf("Name: %s\n", current->name);
    printf("Gender: %c\n", current->gender);
    printf("Country: %s\n", current->country);
    printf("5km Split Time:  %s\n", tts(current->time_5k));
    printf("10km Split Time: %s\n", tts(current->time_10k));
    printf("15km Split Time: %s\n", tts(current->time_15k));
    printf("Official Time:   %s\n", tts(current->time_official));
}

void swap(runner* a, runner* b) {
    runner temp = *a;
    *a = *b;
    *b = temp;
}

int Partition(runner* a, int low, int high) {
    int i = low;
    for (int j = low; j < high; j++) {
        if ((a[j])->bib < (a[high])->bib) {
            //swap
            swap(&a[i], &a[j]);
            i++;
        }
    }
    swap(&a[high], &a[i]);
    return i;
}

void QuickSort(runner* a, int low, int high) {
    if (low < high) {
        int p = Partition(a, low, high);
        QuickSort(a, low, p - 1);
        QuickSort(a, p + 1, high);
    }
}

runner* refreshBibArray(runner head, int size) {
    runner current = head;
    runner* bibArray = (runner*) malloc(size * sizeof (runner));
    for (int i = 0; i < size; i++) {
        bibArray[i] = current;
        current = current->next;
    }
    QuickSort(bibArray, 0, size - 1);
    printf("Sorted Bib Array successfully updated.\n");
    return bibArray;
}

void add(runner* head, runner* tail) {
    char*tem = (char*) calloc(MAX_LEN, sizeof (char));
    int tempp;
    char ttt[MAX_LEN];
    runner tempr = (runner) malloc(sizeof (runnerType));
    if (tempr == NULL) {
        printf("Can not get space from heap for new runner node.");
        return;
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

void edit() {

}

void delete(runner* current, runner* head, runner* tail) {

    //check if the node is at the end of array
    if ((*current)->next == NULL) {
        (*tail) = (*current)->prev;
    }

    //check if the node is at the front of the array
    if ((*current)->prev == NULL) {
        (*head) = (*current)->next;

        //if node is not the end or front of the array, change the next pointer of 
        //previous node to next of current node, and change the previous pointer of 
        //the next node to previous of current node.
    } else {
        (*current)->prev->next = (*current)->next;
        (*current)->next->prev = (*current)->prev;
    }

    //free the current node;
    free(*current);
    size--;
    printf("Delete successfully\n");
}

void free_nameTable(htNode* nameTable) {
    for (int i = 0; i < TABLESIZE; i++) {
        free(nameTable[i]);
    }
    free(nameTable);
}

void free_memory(runner head, runner* bibArray, htNode* nameTable) {
    runner current = head;
    while (current != NULL) {
        head = current->next;
        free(current);
        current = head;
    }
    free(bibArray);
    for (int i = 0; i < TABLESIZE; i++) {
        free(nameTable[i]);
    }
    free(nameTable);
}