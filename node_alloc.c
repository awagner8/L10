
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "CuTest.h"

typedef struct node {
    int64_t val;
    struct node *next;
} node_t;

void print_free_list();

#define NUM_ALLOC 16

node_t *free_list = NULL; // global variable for free list

// TODO: return node at head of free list, allocate NUM_ALLOC more nodes using malloc if free list empty
node_t *get_node() {
    return NULL;
}

// TODO: insert freed nodes at head of free list
void free_node (node_t *ptr) {
    return;
}

// utility functions
// print the list
void print_free_list() {
    node_t *ptr = free_list;
    while (ptr) {
        printf("0x%p val %ld next %p\n",ptr, ptr->val, ptr->next);
        ptr = ptr->next;
    }
}


// single node allocate and free
void test_1 (CuTest *tc) {
    node_t *node_ptr;

    // test 1 allocate and free one node
    node_ptr = get_node();
    if (node_ptr == NULL) {
        printf("Error allocating nodes!\n");
        exit(1);
    }
    node_ptr->val = 12345;
    node_ptr->next = NULL;
    free_node(node_ptr);
    CuAssertIntEquals(tc, 12345, free_list->val);
    CuAssertPtrEquals(tc, node_ptr, free_list);
}

// allocate two, free in reverse order, allocate again
void test_2 (CuTest *tc) {
    node_t *p1, *p2;
    p1 = get_node();
    p2 = get_node();
    p1->val = 1;
    p2->val = 2;
    free_node(p1);
    free_node(p2);
    CuAssertIntEquals(tc, 1, free_list->next->val);  // note: relying on not bzero in free
    CuAssertIntEquals(tc, 2, free_list->val);
    CuAssertPtrEquals(tc, p2, free_list);
}

// test allocate and free of exactly NUM_ALLOC
void test_3 (CuTest *tc) {

    node_t *ptrs[NUM_ALLOC] = {NULL};

    for (int i = 0; i < NUM_ALLOC; i++) {
        ptrs[i] = get_node();
        ptrs[i]->val = i;
    }

    CuAssertPtrEquals(tc, NULL, free_list);
}


//test alloc and free of exactly NUM_ALLOC*2
void test_4 (CuTest *tc) {
    node_t *ptrs[NUM_ALLOC*2] = {NULL};
    for (int i = 0; i < NUM_ALLOC*2; i++) {
        ptrs[i] = get_node();
        ptrs[i]->val = i;
        ptrs[i]->next = NULL;
    }
    
    for (int i = NUM_ALLOC*2 -1 ;  i >= 0; i--) {
        free_node(ptrs[i]);
        ptrs[i] = NULL;
    }

    for (int i = 0; i < NUM_ALLOC*2; i++) {
        ptrs[i] = get_node();
        ptrs[i]->val = i*2;
        ptrs[i]->next = NULL;
    }

    for (int i = 0; i < NUM_ALLOC*2; i++) {
        CuAssertIntEquals(tc, i*2, ptrs[i]->val);
    }

    CuAssertPtrEquals(tc, NULL, free_list);

    for (int i = 0; i < NUM_ALLOC*2; i++) {
        free_node(ptrs[i]);
    }
}


//test random alloc and free w/ NUM_ALLOC * 4
void test_5 (CuTest *tc) {
    node_t *ptrs[NUM_ALLOC*4] = {NULL};
    int num_alloc = 0, num_free = 0;

    for (int k = 0; k < 5000; k++) { // do 1024 random gets and deletes
        
        int idx = rand() % (NUM_ALLOC * 4);
        if (!ptrs[idx] && (idx & 1)) {     // randomly allocat if not already allocated
                ptrs[idx] = get_node();
                ptrs[idx]->val = idx;
                ptrs[idx]->next = NULL;
                ++num_alloc;
        }

        idx = rand() % (NUM_ALLOC * 4);
        if (idx & 1) {   // randomly delete something (may or may not be allocated)
            if (ptrs[idx])
                ++num_free;
            free_node(ptrs[idx]);
            ptrs[idx] = NULL;          
        }
    }
    
    for (int i = 0; i < NUM_ALLOC*4; i++) {
        free_node(ptrs[i]);
    }
}

int main(int argc, char **argv){
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();
    char c;
    int test_num = 0;

    while ((c = getopt(argc, argv, "t:")) != -1) {
        if (c == 't')
            test_num = atoi(optarg);
    }
    switch (test_num) {
        case 1:
            SUITE_ADD_TEST(suite, test_1);
            break;
        case 2:
           SUITE_ADD_TEST(suite, test_2);
            break;
        case 3:
           SUITE_ADD_TEST(suite, test_3);
            break;
        case 4:
           SUITE_ADD_TEST(suite, test_4);
            break;
        case 5:
           SUITE_ADD_TEST(suite, test_5);
            break;
        default:
            SUITE_ADD_TEST(suite, test_1);
            SUITE_ADD_TEST(suite, test_2);
            SUITE_ADD_TEST(suite, test_3);
            SUITE_ADD_TEST(suite, test_4);
            SUITE_ADD_TEST(suite, test_5);
    }

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
    CuStringDelete(output);
    CuSuiteDelete(suite);
    return 0;
}