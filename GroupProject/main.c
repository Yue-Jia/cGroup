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

void readFile(runner*, runner*);
void print(runner);
void printBibArray(runner*);
void printRunner(runner);
char* tts(int);
char* tts2(int);
runner* refreshBibArray(runner);
int searchBibIndex(runner*, int);
bool checkBib(runner*, int);
runner searchBib(runner*, int);
htNode* refreshNameHashTable(runner);
unsigned int hashFunction(unsigned int);
void htInsert(htNode*, char*, runner);
unsigned int keyToInt(char*);
runner searchName(htNode*, char*);
void add(runner*, runner*, htNode**, runner**);
void edit(runner, runner*, runner*, runner*);
void delete(runner, runner*, runner*, htNode**, runner**);
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
    readFile(&head, &tail);


    //before refresh, remember to free the memory
    runner* bibArray = refreshBibArray(head);
    //search bib, print detail
    //searchBib(bibArray,141);

    //Create name hash table
    htNode* nameTable = refreshNameHashTable(head);
    //search name, print detail
    //searchName(nameTable, "Juntong Hou");

    //Command Line Interface
    while (true) {
        printf("\n--------------------Main Menu-----------------------\n");
        printf("|            |B| List by Bib ASC                   |\n");
        printf("|            |O| List by Official Time ASC         |\n");
        printf("|            |S| Search by Bib                     |\n");
        printf("|            |N| Search by Name                    |\n");
        printf("|            |I| Insert Runner                     |\n");
        printf("|            |Q| Quit                              |\n");
        printf("----------------------------------------------------\n");
        //printf("|Q| Quit |B| List by Bib ASC |O| List by Official Time ASC\n");
        //printf("|S| Search by Bib |N| Search by Name |I| Insert Runner\n");
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
            print(head);
        } else if (userInput[0] == 'n' || userInput[0] == 'N') {
            //Search by Name
            while (true) {
                printf("\n-----------------Search by Name---------------\n");
                printf("|Q| Quit\n\n");
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
                                edit(temp, &head, &tail, bibArray);
                                printRunner(temp);
                            } else if (userInput[0] == 'd' || userInput[0] == 'D') {
                                //invoke delete here
                                delete(temp, &head, &tail, &nameTable, &bibArray);
                                break;
                            }
                        }
                    }
                }
            }
        } else if (userInput[0] == 's' || userInput[0] == 'S') {
            //Search by Bib
            while (true) {
                printf("\n----------Search by Bib----------\n");
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
                                    edit(temp, &head, &tail, bibArray);
                                    printRunner(temp);
                                } else if (userInput[0] == 'd' || userInput[0] == 'D') {
                                    printf("Are you sure you want to delete runner %s? Y/Any key to Abort", temp->name);
                                    FLUSH;
                                    fgets(userInput, MAX_LEN, stdin);
                                    FLUSH;
                                    REMOVERN(userInput);
                                    if (strlen(userInput) == 1 && (userInput[0] == 'y' || userInput[0] == 'Y')) {
                                        //invoke delete here
                                        delete(temp, &head, &tail, &nameTable, &bibArray);
                                        break;
                                    } else {
                                        printRunner(temp);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } else if (userInput[0] == 'i' || userInput[0] == 'I') {
            add(&head, &tail, &nameTable, &bibArray);
        }
    }

    writeFile(head);
    free_memory(head, bibArray, nameTable);
    return 0;
}

void writeFile(runner head) {
    FILE *fp;
    fp = fopen("data.txt", "w");
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
    printf("Data saved.\n");
}

void readFile(runner* head, runner* tail) {
    FILE *fp;
    fp = fopen("data.txt", "r");
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
                size++;
                if (size == 1) {
                    *head = node;
                    *tail = node;
                } else {
                    //if the official time is smaller than head, then insert the node at the start and make it head
                    if(node->time_official <= (*head)->time_official){
                        node->next = (*head);
                        (*head)->prev = node;
                        *head = node;
                        continue;
                    }
                    //if the official time is larger than tail, then insert the node at the end and make it tail
                    else if(node->time_official >= (*tail)->time_official){
                        (*tail)->next = node;
                        node->prev = (*tail);
                        *tail = node;
                        continue;
                    }
                    //if the official time is in the middle
                    else{
                        runner current = *head;
                        while(current->next != NULL && node->time_official > current->time_official){
                            current = current->next;
                        }
                        node->prev = (current->prev);
                        node->next = current;
                        (current->prev)->next = node;
                        current->prev = node;
                    }
                    //
                }
            }
        }
    }
    printf("%d records loaded.\n", size);
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
    htNode* nameTable = (htNode*) calloc(TABLESIZE, sizeof (htNode));
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
    nameTable[index] = (htNode) calloc(1, sizeof (htNodeType));
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

char* tts2(int n) {
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
    int pauseCount = -1;
    while (current != NULL) {
        pauseCount++;
        printf("%6d %-30s%2c%4s%9s\n", current->bib, current->name, current->gender, current->country, tts(current->time_official));
        if (head->prev == NULL) {
            current = current->next;
        } else {
            current = current->prev;
        }
        
        if(pauseCount == 1000) {
            printf("Press Enter to Continue...\n");
            getchar();
            pauseCount = -1;
        };
    }
}

void printBibArray(runner* bibArray) {
    //printf("Bib Number  Runner's Name                   Gender        Country       Official Time");
    //printf("-------------------------------------------------------------------------------------");
    int pauseCount = -1;
    for (int i = 0; i < size; i++) {
        pauseCount++;
        printf("%6d %-30s%4c%6s%9s\n", bibArray[i]->bib, bibArray[i]->name, bibArray[i]->gender, bibArray[i]->country, tts(bibArray[i]->time_official));
        if(pauseCount == 1000) {
            printf("Press Enter to Continue...\n");
            getchar();
            pauseCount = -1;
        };
    }
}

void printRunner(runner current) {
    printf("\n----------Detail----------\n");
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

runner* refreshBibArray(runner head) {
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

void add(runner* head, runner* tail, htNode** nameTable, runner** bibArray) {
    //local variable
    char tem[MAX_LEN];
    bool rep;
    int tempp;
    char ttt[MAX_LEN];

    //allocate memory for new runner
    runner node = (runner) malloc(sizeof (runnerType));
    if (node == NULL) {
        printf("Can not get space from heap for new runner node.");
        return;
    }

    //get name
    do {
        printf("please enter the runner name:");
        FLUSH;
        fgets(tem, MAX_LEN, stdin);
        REMOVERN(tem);
    } while (strlen(tem) == 0);
    node->name = (char*) calloc(strlen(tem) + 1, sizeof (char));
    if (node->name == NULL) {
        printf("Can not get space from heap.");
        return;
    }
    strncpy(node->name, tem, strlen(tem));

    //get gender
    do {
        printf("please enter the runner gender:");
        FLUSH;
        fgets(tem, MAX_LEN, stdin);
        REMOVERN(tem);
    } while (strlen(tem) == 0);
    //change lowercase to upper
    if (tem[0] > 96) {
        tem[0] -= 32;
    }
    node->gender = tem[0];

    //get country
    do {
        printf("please enter the runner country:");
        memset(tem, 0, 4);
        FLUSH;
        fgets(tem, MAX_LEN, stdin);
        REMOVERN(tem);
    } while (strlen(tem) == 0);
    tem[3] = '\0';
    //change lowercase to upper
    for (int k = 0; k < strlen(tem); k++) {
        if (tem[k] > 96)
            tem[k] -= 32;
    }
    strncpy(node->country, tem, strlen(tem) + 1);

    do {
        printf("please enter the runner bib number:");
        FLUSH;
        fgets(ttt, MAX_LEN, stdin);
        tempp = strtol(ttt, NULL, 10);
        rep = checkBib(*bibArray, tempp);
        if (rep) {
            printf("Error: Bib number already exist.\n");
        }
    } while (tempp <= 0 || rep);
    node->bib = tempp;

    do {
        printf("please enter the runner official time in seconds:");
        FLUSH;
        fgets(ttt, MAX_LEN, stdin);
        tempp = strtol(ttt, NULL, 10);

    } while (!tempp || tempp < 0);
    node->time_official = tempp;

    do {
        printf("please enter the runner 5k time in seconds:");
        FLUSH;
        fgets(ttt, MAX_LEN, stdin);
        tempp = strtol(ttt, NULL, 10);

    } while (!tempp || tempp < 0);
    node->time_5k = tempp;

    do {
        printf("please enter the runner 10k time in seconds:");
        FLUSH;
        fgets(ttt, MAX_LEN, stdin);
        tempp = strtol(ttt, NULL, 10);

    } while (!tempp || tempp < 0);
    node->time_10k = tempp;

    do {
        printf("please enter the runner 15k time in seconds:");
        FLUSH;
        fgets(ttt, MAX_LEN, stdin);
        tempp = strtol(ttt, NULL, 10);

    } while (!tempp || tempp < 0);
    node->time_15k = tempp;
    node->next = NULL;
    node->prev = NULL;
    
    //insert into right position
    //if the official time is smaller than head, then insert the node at the start and make it head
    if (node->time_official <= (*head)->time_official) {
        node->next = (*head);
        (*head)->prev = node;
        *head = node;
    }        //if the official time is larger than tail, then insert the node at the end and make it tail
    else if (node->time_official >= (*tail)->time_official) {
        (*tail)->next = node;
        node->prev = (*tail);
        *tail = node;
    }        //if the official time is in the middle
    else {
        runner current = *head;
        while (current->next != NULL && node->time_official > current->time_official) {
            current = current->next;
        }
        node->prev = (current->prev);
        node->next = current;
        (current->prev)->next = node;
        current->prev = node;
    }

    size++;
    printf("Insert successfully.\n");

    //update bibArray
    free(*bibArray);
    *bibArray = refreshBibArray(*head);
    //update nameTable
    free_nameTable(*nameTable);
    *nameTable = refreshNameHashTable((*head));
}

void edit(runner node, runner* head, runner* tail, runner* bibArray) {
    char tem[MAX_LEN];
    int tempp;
    char ttt[MAX_LEN];

    //get name
    do {
        printf("Edit please enter the runner name:");
        FLUSH;
        fgets(tem, MAX_LEN, stdin);
        REMOVERN(tem);
    } while (strlen(tem) == 0);
    char name[MAX_LEN] = {0};
    strncpy(name, tem, strlen(tem));

    //get gender
    do {
        printf("please enter the runner gender:");
        FLUSH;
        fgets(tem, MAX_LEN, stdin);
        REMOVERN(tem);
    } while (strlen(tem) == 0);
    //change lowercase to upper
    if (tem[0] > 96) {
        tem[0] -= 32;
    }
    char gender = tem[0];

    //get country
    do {
        printf("please enter the runner country:");
        memset(tem, 0, 4);
        FLUSH;
        fgets(tem, MAX_LEN, stdin);
        REMOVERN(tem);
        tem[3] = '\0';
    } while (strlen(tem) == 0);
    //change lowercase to upper
    for (int k = 0; k < strlen(tem); k++) {
        if (tem[k] > 96)
            tem[k] -= 32;
    }
    char country[4] = {0};
    strncpy(country, tem, 3);

    //get bib number
    bool rep;
    do {
        printf("please enter the runner bib number:");
        FLUSH;
        fgets(ttt, MAX_LEN, stdin);
        tempp = strtol(ttt, NULL, 10);
        rep = checkBib(bibArray, tempp);
        if (rep && tempp != node->bib) {
            printf("Error: Bib number already exist.\n");
        }
    } while (tempp <= 0 || (rep && tempp != node->bib));
    int bib = tempp;

    //get official time
    do {
        printf("please enter the runner official time in seconds:");
        FLUSH;
        fgets(ttt, MAX_LEN, stdin);
        tempp = strtol(ttt, NULL, 10);
    } while (!tempp || tempp < 0);
    int time_official = tempp;

    //get 5k time
    do {
        printf("please enter the runner 5k time in seconds:");
        FLUSH;
        fgets(ttt, MAX_LEN, stdin);
        tempp = strtol(ttt, NULL, 10);
    } while (!tempp || tempp < 0);
    int time_5k = tempp;

    //get 10k time
    do {
        printf("please enter the runner 10k time in seconds:");
        FLUSH;
        fgets(ttt, MAX_LEN, stdin);
        tempp = strtol(ttt, NULL, 10);
    } while (!tempp || tempp < 0);
    int time_10k = tempp;

    //get 15k time
    do {
        printf("please enter the runner 15k time in seconds:");
        FLUSH;
        fgets(ttt, MAX_LEN, stdin);
        tempp = strtol(ttt, NULL, 10);
    } while (!tempp || tempp < 0);
    int time_15k = tempp;

    //print the old information compare with new information
    printf("\n----------  Old -----------| ---------- New ----------\n");
    printf("Bib Number: %-14d | Bib Number: %-14d\n", node->bib, bib);
    printf("Name: %-20s | Name: %-20s\n", node->name, name);
    printf("Gender: %-18c | Gender: %-18c\n", node->gender, gender);
    printf("Country: %-17s | Country: %-17s\n", node->country, country);
    printf("5km Split Time:  %s  | 5km Split Time:  %s\n", tts(node->time_5k), tts2(time_5k));
    printf("10km Split Time: %s  | 10km Split Time: %s\n", tts(node->time_10k), tts2(time_10k));
    printf("15km Split Time: %s  | 15km Split Time: %s\n", tts(node->time_15k), tts2(time_15k));
    printf("Official Time:   %s  | Official Time:   %s\n", tts(node->time_official), tts2(time_official));
    //get comfirmation from user
    printf("The runner's information will be updated as shown above, do you want to continue? Y/Any key to Abort");
    FLUSH;
    fgets(tem, MAX_LEN, stdin);
    REMOVERN(tem);
    if (strlen(tem) == 1 && (tem[0] == 'y' || tem[0] == 'Y')) {
        //update the node
        free(node->name);
        node->name = (char*) calloc(strlen(name) + 1, sizeof (char));
        if (node->name == NULL) {
            printf("Can not get space from heap.");
            return;
        }
        strncpy((node)->name, name, strlen(name));

        (node)->gender = gender;
        strncpy((node)->country, country, 4);
        (node)->bib = bib;
        (node)->time_official = time_official;
        (node)->time_5k = time_5k;
        (node)->time_10k = time_10k;
        (node)->time_15k = time_15k;
        //extract the node
        if ((node)->prev == NULL) {
            ((node)->next)->prev = NULL;
            (*head) = (node)->next;
        } else if ((node)->next == NULL) {
            ((node)->prev)->next = NULL;
            (*tail) = (node)->prev;
        } else {
            runner node1 = (node)->prev;
            runner node2 = (node)->next;
            node1->next = node2;
            node2->prev = node1;
        }
        (node)->next = NULL;
        (node)->prev = NULL;

        //insert into right position
        runner curr = *head;
        while ((((node)->time_official)<(curr->time_official))&&(curr->next != NULL)) {
            curr = curr->next;
        }
        if (curr->next == NULL) {
            if ((((node)->time_official)<(curr->time_official))) {
                curr->next = (node);
                (node)->prev = curr;
                (*tail) = (node);
            } else {
                runner node1 = curr->prev;
                runner node2 = curr;
                (node)->prev = node1;
                (node)->next = node2;
                node1->next = (node);
                node2->prev = (node);
            }
        } else {
            if ((node)->time_official >= (*head)->time_official) {
                (*head)->prev = (node);
                (node)->next = (*head);
                (*head) = (node);
            } else {
                (node)->prev = curr->prev;
                (node)->next = curr;
                curr->prev->next = (node);
                curr->prev = (node);
            }
        }
        printf("Update runner information successfully.\n");
    } else {
        printf("Update aborted.\n");
    }
}

void delete(runner current, runner* head, runner* tail, htNode** nameTable, runner** bibArray) {

    //check if the node is at the end of array
    if (current->next == NULL) {
        (*tail) = current->prev;
        (*tail)->next = NULL;
    }

    //check if the node is at the front of the array
    if (current->prev == NULL) {
        (*head) = current->next;
        (*head)->prev = NULL;
        //if node is not the end or front of the array, change the next pointer of 
        //previous node to next of current node, and change the previous pointer of 
        //the next node to previous of current node.
    } else {
        (current->prev)->next = current->next;
        (current->next)->prev = current->prev;
    }

    //free the current node;
    free(current->name);
    free(current);
    size--;
    printf("Delete successfully\n");

    //update bibArray
    free(*bibArray);
    *bibArray = refreshBibArray(*head);
    //update nameTable
    free_nameTable(*nameTable);
    *nameTable = refreshNameHashTable((*head));
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
        free(current->name);
        free(current);
        current = head;
    }
    free(bibArray);
    for (int i = 0; i < TABLESIZE; i++) {
        free(nameTable[i]);
    }
    free(nameTable);
}