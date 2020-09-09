#include <cstdio>
#include <cstdlib>
#include <utility>
#include "sorts.hpp"

const size_t TEST_SIZE = 1000;

#define SET_GREEN "\x1b[32m"
#define SET_RED "\x1b[31m"
#define RESET "\x1b[0m"


#define TEST_ALGO(NAME) do { \
                        NAME<int> functor; \
                        buildSequence(array, TEST_SIZE); \
                        shuffle(array, TEST_SIZE); \
                        functor(array, TEST_SIZE); \
                        printf("%s: ", functor.getName()); \
                        if(verifySequence(array, TEST_SIZE)) printf(SET_GREEN "OK\n" RESET); \
                        else printf(SET_RED "FAILED\n" RESET); \
                        } while(0)

void buildSequence(int *array, size_t size) {
    for(size_t i = 0; i < size; i++) {
        array[i] = i;
    }
}

bool verifySequence(int* array, size_t size) {
    for(size_t i = 0; i < size; i++) {
        if(array[i] != i)
            return false;
    }

    return true; 
}

void shuffle(int *array, size_t size) {
    if(size > 1) {
        for(size_t i = 0; i < size; i++) {
            size_t j = i + rand() / (RAND_MAX / (size - i) + 1);
            std::swap(array[i], array[j]);
        }
    }
}

int main() {
    int array[TEST_SIZE];
    printf("========================================\nTesting sorting algos in sorts.hpp file\n========================================\n\n");

    TEST_ALGO(BubbleSort);
    TEST_ALGO(InsertionSort);
    TEST_ALGO(SelectionSort);
    TEST_ALGO(QuickSort);
}