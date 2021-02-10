// # I used hash table implementation code from https://steemit.com/programming/@drifter1/programming-c-hashtables-with-chaining
// I used file reading and printing function from https://www.programiz.com/c-programming/examples/read-file
//I used hash table implementation code https://github.com/engineer-man/youtube/blob/master/077/hashtable.c
// Arrow is dot operator but for pointers (assign things using ->)

//Steps
// 8. Make read from file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h> //reporting errors using the symbol errno (short for "error number"). // errno acts like an integer variable.
#include <unistd.h> //for chdir

//Table size represents max items you store before collision
#define TABLE_SIZE 20000
//Definitions for command line
#define MAX_LENGTH 1024
#define DELIMS " \t\r\n"

// Node for List
//Include 5 components - First/Last name, RIN, Zip, Voted
// Key, Value, pointer to next entry
// If collision, has pointer to next entry, and so one (SLL)
typedef struct entry_t {
    char *key; //RIN
    char *last; //Last
    char *first; // First
    char *zip; //Zip
    char *voted; //Yes/No
    struct entry_t *next;
} entry_t;

//Hashmap structure
// Array of pointers to an entry
typedef struct {
    entry_t **entries;
} ht_t;

//Hash function
//Must return value between 0 and SIZE-1
unsigned int hash(const char *key) {
    unsigned long int value = 0;
    unsigned int i = 0;
    unsigned int key_len = strlen(key);

    // do several rounds of multiplication
    // For (0-length of key) - value = value * 37 + key[1,2,3] 
    for (; i < key_len; ++i) {
        value = value * 37 + key[i];
    }

    // make sure value is 0 <= value < TABLE_SIZE
    // Ex. Value = 5 % 11 = 1 
    value = value % TABLE_SIZE;

    return value;
}

//Make the voter entry - with RIN, last, first, voted, zip
entry_t *ht_pair(const char *key, const char *last, const char *first, const char *zip, const char *voted) {
    // allocate the entry
    // Create entry
    // Assign key, last
    entry_t *entry = malloc(sizeof(entry_t) * 1);
    entry->key = malloc(strlen(key) + 1);
    entry->last = malloc(strlen(last) + 1);
    entry->first = malloc(strlen(first) + 1);
    entry->zip = malloc(strlen(zip) + 1);
    entry->voted = malloc(strlen(voted) + 1);
   
    // copy the key and last in place
    strcpy(entry->key, key);
    strcpy(entry->last, last);
    strcpy(entry->first, first);
    strcpy(entry->zip, zip);
    strcpy(entry->voted, voted);

    // next starts out null but may be set later on
    entry->next = NULL;

    return entry;
}

//Creates base table
ht_t *ht_create(void) {
    // allocate table
    // 1 pointer to a hashtable
    ht_t *hashtable = malloc(sizeof(ht_t) * 1);

    // allocate table entries
    // Create TABLE_SIZE number of pointers to an entry
    hashtable->entries = malloc(sizeof(entry_t*) * TABLE_SIZE);

    // set each pointer to null (needed for proper operation)
    // Now we can look up to check if NULL - see if there is entry or not
    int i = 0;
    for (; i < TABLE_SIZE; ++i) {
        hashtable->entries[i] = NULL;
    }

    return hashtable;
}

//Insert entry into hashtable
void ht_set(ht_t *hashtable, const char *key, const char *last, const char *first, const char *zip, const char *voted) {
    // Start by taking hash of given key (RIN) and find out which slot it will go into
    unsigned int slot = hash(key);

    // try to look up an entry set
    // Once we know slot, try to find an entry at that slot
    // Entry will either be NULL or valid entry
    // ?? - Where does entries come from 
    entry_t *entry = hashtable->entries[slot];

    // no entry means slot empty, insert immediately
    // If entry is null, sumply insert new key value pair at that slot
    if (entry == NULL) {
        hashtable->entries[slot] = ht_pair(key, last, first, zip, voted);
        return;
    }
    // If not NULL, either:
    // 1. Key already exists and must update last
    // 2. Collision 
    // Solution: Walk through each entry until end reached or found matching key
    // Matching key - update value
    // Get to end - add new entry
    entry_t *prev;

    // walk through each entry until either the end is
    // reached or a matching key is found
    while (entry != NULL) {
        // check key
        // If found matching key - update value, return
        if (strcmp(entry->key, key) == 0) {
            // match found, replace value
            free(entry->last);
            entry->last = malloc(strlen(last) + 1);
            strcpy(entry->last, last);
            free(entry->first);
            entry->first = malloc(strlen(first) + 1);
            strcpy(entry->first, first);
            free(entry->zip);
            entry->zip = malloc(strlen(zip) + 1);
            strcpy(entry->zip, zip);
            free(entry->voted);
            entry->voted = malloc(strlen(voted) + 1);
            strcpy(entry->voted, voted);
            return;
        }

        // walk to next
        prev = entry;
        entry = prev->next;
    }
    // Entry = NULL at end of line, add new entry 
    // Now, 2 entries at this slot
    // end of chain reached without a match, add new
    prev->next = ht_pair(key, last, first, zip, voted);
}

// Look up last by a key
char *ht_get(ht_t *hashtable, const char *key) {
    // Start by hashing given key
    unsigned int slot = hash(key);

    // Look up entry at that slot
    entry_t *entry = hashtable->entries[slot];

    // Entry is NULL means key not found, return, that's it
    if (entry == NULL) {
        return NULL;
    }

    // If entry NOT NULL, walk through each entry in the slot until we can find a matching key
    //This is the segmentation fault!!
    while (entry != NULL) {
        char notnull[]="entry not null";
        char pastvariable[]="got past variables";
        char notnull1[]="got past assigning";
        char notnull2[]="entry not null";
        printf("%s\n",notnull); //now with this, bus error 10
        // return value if found
        // If we find the matching key, simply return the value
        //This is the segmentation fault!! bus error now
        if (strcmp(entry->key, key) == 0) {
            // Add other values to return here!!
            //Do I need to declare this variables?
            char *last;
            char *first;
            char *zip;
            char *voted;
            char *record;
            last = entry->last;
            first = entry->first;
            zip = entry->zip;
            voted = entry->voted;
            printf("%s\n",notnull1); //now with this, bus error 10 - just worked now for some reason!
            strcat(record,last);
            strcat(record,first);
            strcat(record,zip);
            strcat(record,voted);
            return record;
            break;
        }

        // proceed to next key if available
        entry = entry->next;
    }

    // reaching here means there were >= 1 entries but no key match
    return NULL;
}

void ht_del(ht_t *hashtable, const char *key) {
    unsigned int bucket = hash(key);

    // try to find a valid bucket
    entry_t *entry = hashtable->entries[bucket];

    // no bucket means no entry
    if (entry == NULL) {
        return;
    }

    entry_t *prev;
    int idx = 0;

    // walk through each entry until either the end is reached or a matching key is found
    while (entry != NULL) {
        // check key
        if (strcmp(entry->key, key) == 0) {
            // first item and no next entry
            if (entry->next == NULL && idx == 0) {
                hashtable->entries[bucket] = NULL;
            }

            // first item with a next entry
            if (entry->next != NULL && idx == 0) {
                hashtable->entries[bucket] = entry->next;
            }

            // last item
            if (entry->next == NULL && idx != 0) {
                prev->next = NULL;
            }

            // middle item
            if (entry->next != NULL && idx != 0) {
                prev->next = entry->next;
            }

            // free the deleted entry
            free(entry->key);
            free(entry->last);
            free(entry->first);
            free(entry->zip);
            free(entry->voted);
            free(entry);

            return;
        }

        // walk to next
        prev = entry;
        entry = prev->next;

        ++idx;
    }
}

// Loops through table and outputs what is at every slot
void ht_dump(ht_t *hashtable) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        entry_t *entry = hashtable->entries[i];

        if (entry == NULL) {
            continue;
        }

        printf("slot[%4d]: ", i);

        for(;;) {
            // Print more here !!
            // TODO - Validate format specifiers.
            printf("%s=%s %s %s %s", entry->key, entry->last, entry->first, entry->zip, entry->voted);

            if (entry->next == NULL) {
                break;
            }

            entry = entry->next;
        }

        printf("\n");
    }
}

int main(int argc, char **argv) {

    char *cmd;
    char *record;
    char line[MAX_LENGTH];


    //Create hash table at variable ht
    ht_t *ht = ht_create();
    ht_set(ht,"rin","mcmillen","lauren","zip","n");
    ht_set(ht,"rin1","mcmillen","lauren","zip","YES");
    ht_set(ht,"rin2","mcmillen","lauren","zip","YES");
    ht_dump(ht);

    //Populate hash table with file values
    //Integrate with set function
    // Use strtok to break up by spaces
    // Add voted space
    FILE *fpointer;
    char *token;
    int i;
    //Be careful when using both of these - writing over past variable
    char *key;
    char *first;
    char *last;
    char *zip;
    char *voted;
    fpointer = fopen("voters50.csv","r"); 
    //TODO - Write in vote as fifth and set to N
    //function = opening up notepad program, opens up own version of notepad
    // first parameter = "file, open", last is what you want to do with it
    // to display contents in file, need to first store content in char array
    char singleLine[150];
    // while(!feof(fpointer)){ //file end of file - while not end of file
    //     //Run this loop from beginning of file to the end
    //     //Default- file pointer starts at the beginning
    //     fgets(singleLine, 150, fpointer); //gets one line from FILE, not from standard input like keyboard
    //     token = strtok(singleLine, " ");
    //     i=0;
    //     while (token != NULL){
    //         printf("%s\n", token); 
    //         if (i==0){
    //             key = token;
    //         }
    //         if (i == 1){
    //             last = token;
    //         }
    //         if (i == 2){
    //             first = token;
    //         }
    //         if (i == 3){
    //             zip = token;
    //         }
    //         // if (i == 4){
    //         //     voted = token;
    //         // }
    //         token = strtok(NULL, " "); 
    //         i++;
    //     }
    //    //automatically adds content to new line every time
    //     //get 5 things
    //     // loop through 4 things
    //     // 1=rin, 2=zip
    //     //Write yes to voted
    //     // Maybe segmentation fault because of voted
    //     // ht_set(ht, key, last, first, zip, voted);

    // }
    // fclose(fpointer);

    //char *strtok(char *str, const char *delim) - breaks string str into a series of tokens using the delimiter delim.
    //strtok(string,string that contains each character we want to break at)
    //strtok(str, " ")
    //returns char* (pointer to next occurence of each piece of our string)
    //first time we call it, it will return first piece of our string
    //%s char* 
    //How to print every word - 
    //After runs, still retains its memory - for next piece of our string,  call strtok with same 2nd parameter but now with first paramter NULL
    //just use previous one and continue giving us pieces of that string
    //How do we know we reached the end? Once you already reached the last piece, just return NULL

    //stderr stands for standard error stream. Here it is the console -- the screen
    //The standard practice is to redirect all error messages to stderr
    //fprintf is basically similar to printf, but takes extra parameter stderr 

    while (1) {
      
      printf("\n");
      printf("$ ");

      //char *fgets(char *str, int n, FILE *stream) - reads a line from the specified stream and stores it into the string pointed to by str.
      if (!fgets(line, MAX_LENGTH, stdin))
        break;
      
      // Parse and execute command
      if ((cmd = strtok(line, DELIMS))) 
      {
        // Clear errors
        errno = 0;

        if (strcmp(cmd, "l") == 0) {
            char *arg = strtok(0, DELIMS);
            printf("%s\n",arg); //get to here in code

            if (!arg) //if no arguments
               {
                fprintf(stderr, "l- missing key.\n");
                break;
               } 
            else
                record = ht_get(ht,arg);
                printf("%s", record);
            
        }
        
        //Insert - i
        else if (strcmp(cmd, "i") == 0) {
                int i;
                // char *key1;
                // char *first1;
                // char *last1;
                // char *zip1;
                // char *voted1;
                char *arg1 = strtok(0, DELIMS);

            if (!arg1) //if no arguments
                fprintf(stderr, "i- missing keys.\n"); 

            else
                i = 0;
                while (arg1 != NULL){
                    if(i==0){
                        key = arg1;
                    }
                    if(i ==1){
                        last = arg1;
                    }
                    if(i ==2){
                        first = arg1;
                    }
                    if(i ==3){
                        zip = arg1;
                    }
                    if(i ==4){
                        voted = arg1;
                    }
                    arg1 = strtok(NULL," ");
                    i++;
                }
                printf("%s %s %s %s %s",key, last, first, zip, voted);
                ht_dump(ht);
                ht_set(ht,key,last,first,zip,voted); //This does work! This puts it in
                ht_dump(ht);
            }
        //d- Delete
        else if (strcmp(cmd, "d") == 0) {
                char *arg = strtok(0, DELIMS);
                printf("%s\n",arg); //get to here in code

                if (!arg) //if no arguments
                {
                    fprintf(stderr, "d- Record does not exist\n");
                    break;
                } 
                else
                    ht_del(ht,arg);
                    ht_dump(ht);
            }
        //r - Mark as YES
        else if (strcmp(cmd, "r") == 0) {
            int i;
            char yes[]="This user has voted already.";
            char changed[]="System has changed vote to YES";
            char *arg = strtok(0, DELIMS);
            if (!arg) //if no arguments
            {
                fprintf(stderr, "r- missing argument\n");
                break;
            } 
            else 
                //Loop through hash table
                for (int i = 0; i < TABLE_SIZE; ++i) {
                    entry_t *entry = ht->entries[i];
                    if (entry == NULL) {
                        continue;
                    }
                for(;;) {
                    if(strcmp(entry->key, arg) == 0){
                        printf("voted entry = %s\n", entry->voted);
                        if (strcmp(entry->voted, "YES") == 0){ //If voted YES, no action needed. Print message
                            printf("%s\n",yes);
                            break;
                        }
                        else
                            //Change string to YES and display changed
                            entry->voted="YES";
                            printf("%s\n", changed);
                            break;
                      }  
                      if (entry->next == NULL) {
                            break;
                        }
                      entry = entry->next;
                }
            }
        } 
        //bv <fileofkeys> - Bulk-vote for all the keys
        else if (strcmp(cmd, "bv") == 0) {
            int i;
            char yes[]="This user has voted already.";
            char changed[]="System has changed vote to YES";
            char *arg = strtok(0, DELIMS);
            if (!arg) //if no arguments
            {
                fprintf(stderr, "bv- missing file argument\n");
                break;
            } 
            //Read file
            fpointer = fopen(arg,"r"); 
             //Get line by line
             while(!feof(fpointer)){ //file end of file - while not end of file
                fgets(singleLine, 150, fpointer); //gets one line from FILE, not from standard input like keyboard
                token = strtok(singleLine, DELIMS);
                //Loop through hash table
                for (int i = 0; i < TABLE_SIZE; ++i) {
                    entry_t *entry = ht->entries[i];
                    if (entry == NULL) {
                        continue;
                    }
                for(;;) {
                    if(strcmp(entry->key, token) == 0){
                        printf("voted entry = %s\n", entry->voted);
                        if (strcmp(entry->voted, "YES") == 0){ //If voted YES, no action needed. Print message
                            printf("%s\n",yes);
                            break;
                        }
                        else
                            //Change string to YES and display changed
                            entry->voted="YES";
                            printf("%s\n", changed);
                            break;
                      }  
                      if (entry->next == NULL) {
                            break;
                        }
                      entry = entry->next;
                }
            }
         }      
    } 
        
           //V - Number of people who voted 
           else if (strcmp(cmd, "v") == 0) {
                int i;
                int c;
                char *arg1 = strtok(0, DELIMS);

            //If argument then say v does not take arguments
            if (arg1) //if no arguments
                fprintf(stderr, "v does not take any arguments\n"); 

            else
            {    //Loop through entire hash table
                for (int i = 0; i < TABLE_SIZE; ++i) {
                    entry_t *entry = ht->entries[i];
                    //If nothing in entry, igrnoed
                    if (entry == NULL) {
                        continue;
                    }
                    //For each entry, look up fifth argument
                    for(;;) {
                        if(strcmp(entry->voted, "YES")== 0){   //If yes, add to counter
                            c++;
                        };
                        if (entry->next == NULL) {
                            break;
                        }
                        entry = entry->next;
                    }
                } printf("%i",c);    
            }
        }
        //Perc - show percent who voted YES
        else if (strcmp(cmd, "perc") == 0) {
                float i;
                float c;
                float num;
                char *arg1 = strtok(0, DELIMS);

            //If argument then say v does not take arguments
            if (arg1) //if no arguments
                fprintf(stderr, "perc does not take any arguments\n"); 

            else
            {    //Loop through entire hash table
                for (int i = 0; i < TABLE_SIZE; ++i) {
                    entry_t *entry = ht->entries[i];
                    //If nothing in entry, igrnoed
                    if (entry == NULL) {
                        continue;
                    }
                    //For each entry, look up fifth argument
                    for(;;) {
                        num++;
                        if(strcmp(entry->voted, "YES")== 0){   //If yes, add to counter
                            c++;
                        };
                        if (entry->next == NULL) {
                            break;
                        }
                        entry = entry->next;
                    }
                } 
                printf("%f\n",c);   
                printf("%f\n",num); 
                float perc;
                char sign[] = "%%";
                perc = (c/num)*100;
                printf("%f%s",perc,sign);
            }
        }

        //in case exit is typed
        else if (strcmp(cmd, "exit") == 0) 
            break;

        //for any other command
        else 
            system(line);

        //handling errors 
        if (errno) 
            perror("Command failed"); //will print descriptive error msg starting with Command failed
        }

        }

    return 0;
}
