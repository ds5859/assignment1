#include <check.h>
#include "dictionary.h"
#include <stdlib.h>

#define DICTIONARY "wordlist.txt"
#define TESTDICT "test_worlist.txt"
#define NULLDICT ""
#define UNUSUAL "test_unusual.txt"

START_TEST(test_dictionary_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(load_dictionary(TESTDICT, hashtable));
    const char* word1 = "first";
    const char* word2 = "second";
    const char* word3 = "third";
    const char* word4 = "test";
    int index1 = hash_function(word1);
    int index2 = hash_function(word2);
    int index3 = hash_function(word3);
    int index4 = hash_function(word4);
    ck_assert(strcmp(hashtable[index1]->word, word1) == 0);
    ck_assert(strcmp(hashtable[index2]->word, word2) == 0);
    ck_assert(strcmp(hashtable[index3]->word, word3) == 0);
    ck_assert(strcmp(hashtable[index4]->word, word4) == 0);
}
END_TEST

START_TEST(test_check_word_extended)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* word1 = "test";
    const char* word2 = "?test";
    const char* word3 = "te?st";
    const char* word4 = "?test?";
    const char* word5 = "Llanfairpwllgwyngyllgogerychwyrndrobwllllantysiliogogogoch"; //Welsh town
    const char* word6 = "0123456789";
    const char* word7 = "te!!!st";
    const char* word8 = "@te2st!";
    ck_assert(check_word(word1, hashtable));
    ck_assert(!check_word(word2, hashtable));
    ck_assert(!check_word(word3, hashtable));
    ck_assert(!check_word(word4, hashtable));
    ck_assert(!check_word(word5, hashtable));
    ck_assert(!check_word(word6, hashtable));
    ck_assert(!check_word(word7, hashtable));
    ck_assert(!check_word(word8, hashtable));
    // As my spell.c strips punctuation in the check_words function and not check_word,
    // most of these should fail
}
END_TEST

START_TEST(test_null_dictionary)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(!load_dictionary(NULLDICT, hashtable));
    // try opening an empty dictionary
}
END_TEST

START_TEST(test_check_word_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* correct_word = "Justice";
    const char* incorrect_word = "athens";
    const char* punctuation_word_2 = "pl.ace";
    ck_assert(check_word(correct_word, hashtable));
    ck_assert(!check_word(incorrect_word, hashtable));
    ck_assert(!check_word(punctuation_word_2, hashtable));
    // Test here: What if a word begins and ends with "?
}
END_TEST

START_TEST(test_check_words_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char* expected[3];
    expected[0] = "sogn";
    expected[1] = "skyn";
    expected[2] = "betta";
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("test1.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 3);
    bool test = strlen(misspelled[0]) == strlen(expected[0]);
    int len1 = strlen(misspelled[0]);
    int len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);
    ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0);
}
END_TEST

START_TEST(test_check_words_unusual)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);

    char* expected[3];
    expected[0] = "sec?ond";
    expected[1] = "0123456789";
    expected[2] = "日本語のテスト";
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen(UNUSUAL, "r");

    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 3);
    bool test = strlen(misspelled[0]) == strlen(expected[0]);
    int len1 = strlen(misspelled[0]);
    int len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);
    ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0);
}
END_TEST

START_TEST(test_check_word_buffer_overflow)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char incorrect_word[500000];
    for(int i = 0; i < 499999; i++)
        incorrect_word[i] = 'A';
    incorrect_word[499999] = 0;
    ck_assert(!check_word(incorrect_word, hashtable));
}
END_TEST


Suite *
check_word_suite(void)
{
    Suite * suite;
    TCase * check_word_case;
    suite = suite_create("check_word");
    check_word_case = tcase_create("Core");
    tcase_add_test(check_word_case, test_check_word_normal);
    tcase_add_test(check_word_case, test_check_words_normal);
    tcase_add_test(check_word_case, test_check_words_unusual);
    tcase_add_test(check_word_case, test_check_word_extended);
    tcase_add_test(check_word_case, test_check_word_buffer_overflow);
    tcase_add_test(check_word_case, test_dictionary_normal);
    tcase_add_test(check_word_case, test_null_dictionary);

    suite_add_tcase(suite, check_word_case);

    return suite;
}

int
main(void)
{
    int failed;
    Suite *suite;
    SRunner *runner;
    
    suite = check_word_suite();
    runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

