#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) {
    int totalwords = 0;
    char input[LENGTH + 1];
    char cursor = '\0';
    //char word[LENGTH + 1];
    char lower[LENGTH + 1];
    for (int i = 0; i <= LENGTH; i++) {
        input[i] = '\0';
        lower[i] = '\0';
    }

    while (!feof(fp)) {
        cursor = fgetc(fp);
        //strcpy(rawinput, "") ;

        if ((ispunct(cursor)) || (isspace(cursor))) { //if cursor reads space or punctuation (any graphic character that is not alphanumeric)
            continue; //goes back to fgetc next char from fp
        }

        // if not space or punct - begin reading word string until next space
        int counter = 0;
        while (!isspace(cursor)) { //punt could be within word string eg apostrophes
            if (feof(fp)) { //hit end of file while getting word string
                break; //break out of while loop
            }
            if (counter > LENGTH) { //if word string exceeds LENGTH
                while ((!isspace(cursor)) && (!feof(fp))) { //move cursor until next whitespace
                    cursor = fgetc(fp);
                }
                break; //break out of while loop
            }
            if (counter <= LENGTH) { //word within limits - read cursor into rawinput
                input[counter] = cursor;
                counter++;
            }
            cursor = fgetc(fp); // gets next character
        }
        //scenario where word string exceeded LENGTH -> send LENGTH string to misspelled
        if (counter > LENGTH) {
            input[LENGTH] = '\0';
            misspelled[totalwords] = (char*) malloc(sizeof(char) * (LENGTH + 1));
            strcpy(misspelled[totalwords], input);
            totalwords++;
            input[0] = '\0';
            continue; //goes back to fgetc next char from fp
        }

        //remove trailing punctuation
        int revcounter = counter;
        while ((revcounter > 1) && (ispunct(input[revcounter-1]))) {
            input[revcounter-1] = '\0';
            revcounter--;
        }

        //create lowercase version
        for (int j = 0; j <= counter; j++) {
            lower[j] = tolower(input[j]);
        }
        //ensure null terminator at end of word string input and lower
        input[counter] = '\0';
        lower[counter] = '\0';

        if ((check_word(input, hashtable) == false) && (check_word(lower, hashtable) == false)) {
            misspelled[totalwords] = (char*) malloc(sizeof(char) * (LENGTH + 1));
            strcpy(misspelled[totalwords], input);
            totalwords++;
        }
    }
    return (totalwords);
}



bool check_word(const char* word, hashmap_t hashtable[]) {
    bool there = false;
    char input[LENGTH + 1];
    int i = 0;
    //printf("\n1");
    for (i = 0; i <= LENGTH; i++) {
        input[i] = '\0';
        //printf(". ");
    }

    int j = 0;
    //printf("\n2");
    while ((word[j] != '\0') && (j <= LENGTH)) {
        //if (word[j]) { //hangs here if punct is in word (isalpha(word[j]))
        input[j] = word[j];
        j++;
        //printf(". ");
        //}
    }
    //printf("\n3");
    int ind = hash_function(input);
    hashmap_t nxt = hashtable[ind];
    //printf("\n4");
    while (nxt != NULL) {
        if (strcmp(input, nxt->word) == 0) {
            there = true;
        }
        nxt = nxt->next;
    }
    //printf("\n5");
    return there;
}


bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {
    FILE *fp = fopen(dictionary_file, "r");
    if (fp == NULL) {
        //printf("\nError. No file loaded.");
        return false;
    }
    for (int counter = 0; counter < HASH_SIZE; counter++) {
        hashtable[counter] = NULL;
    }
    char word[LENGTH + 1];
    fgets(word, LENGTH + 1, fp);

    while (fgets(word, LENGTH + 1, fp)) {
        // put line in hashtable
        // get index
        char *token = strtok(word,"\n");
        //printf("%s\n", token);
        hashmap_t newnode = (hashmap_t) malloc(sizeof(node));
        strcpy(newnode->word,token);

        int ind = hash_function(token);
        newnode->next = hashtable[ind];
        hashtable[ind] = newnode;
    }
    fclose(fp);
    return true;
}
