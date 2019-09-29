#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

//#define DICTIONARY "wordlist.txt"
//#define TEST "test1.txt"

int main(int argc, char *argv[]) {
    hashmap_t hashtable[HASH_SIZE];
    char *mispelled[MAX_MISSPELLED];
    int count = 0;

    char *test_file = argv[1];
    char *dictionary = argv[2];

    //load dictionary
    load_dictionary(dictionary, hashtable);


    //open test file and check if open
    FILE *fp = fopen(test_file, "r");
    if (fp == NULL){
        printf("\nError. No test file loaded.\n");
    }
    else {

        //run check words
        count = check_words(fp, hashtable, mispelled);

        //check if count is empty
        if (count == 0) {
            printf("\nError. No misspelled words in test file\n");

        } else {
            printf("\nTotal misspelled words = %d", count);
            for (int i = 0; i < count; i++) {
                printf("\nMisspelled word %s at location %d\n", mispelled[i], i);
            }
        }
    }
    fclose(fp);

    //TODO: freeing memory
    for (int counter = 0; counter < HASH_SIZE; counter++) {
        hashmap_t curr = hashtable[counter];
        hashmap_t follow = NULL;
        while (curr != NULL) {
            follow = curr->next;
            free(curr);
            curr = follow;
        }
        free(follow);
    }
    for (int counter = 0; counter < count; counter++)
        free(mispelled[counter]);

}
