
#include <stdio.h>
#include "struct.h"
#include <stdlib.h>
#include <stdbool.h> 
#include "helper.h"
#define OVERHEAD 16
#define VOID_SIZE 8 

struct Block *free_head;
//==============================================================================================================
// Name:     my_initialize_heap
// Function: allocates memory for free_head
// Input(s): size, how many blocks the caller wants allocated
// Pre(s):   size is a positive integer with minimum size of OVERHEAD (16)
// Post(s):  free_head points to this memory location, blocks are allocated
// Returns:  none
//==============================================================================================================
void my_initialize_heap(int size){
    free_head = (struct Block *) malloc(size);
    free_head->block_size = size - OVERHEAD;
    free_head->next_block = NULL;
}

//==============================================================================================================
// Name:     split
// Function: allocates memory for free_head
// Input(s): size, how many blocks the caller wants allocated
// Pre(s):   free_head points to first block of free list, alloc_size is a positive integer
// Post(s):  on success, data heap is split 
//              1) free_head points to new memory location (with min size of void*)
//              2) new block is a multiple of void*
//                 and its next block is pointing to free_head
//           on fail, data heap is not split
//              1) block big enough is free_head, update free_head
//              2) free list is linked appropriately
// Returns:  pointer to the data section
//==============================================================================================================
struct Block* split(int alloc_size, struct Block* block){
    // calculate size of new block
    int nb_size = (block->block_size - OVERHEAD - alloc_size); 

    // create request block
    *block = (struct Block) {.block_size = alloc_size, .next_block = NULL};  

    // increment block to data section
    struct Block *data_ptr = ++block; 

    // block is already at data section, just add remaining allocate size
    // round up to account for fragmentation
    struct Block *nb = (char*) block + round_up(alloc_size, VOID_SIZE);     
                                                                            
    nb->next_block = block->next_block;                                    

    // new block is where free_head should be updated to
    *nb = (struct Block){.block_size = nb_size, .next_block = NULL};
    
    free_head = nb;
    
    return data_ptr;
}

//==============================================================================================================
// Name:     my_free
// Function: frees a requested block
// Input(s): pointer to the data section
// Pre(s):   pointer is not NULL and is pointing to the data section
// Post(s):  data is "deallocated" from the heap ...
// Returns:  none
//==============================================================================================================
void my_free(void *data){
    struct Block *bp = (struct Block *) data;
    --bp;
    bp->next_block = free_head;
    free_head = bp;
    print_fh(free_head);
}

//==============================================================================================================
// Name:     my_malloc
// Function: prints a linked list
// Input(s): size, how many bytes the caller wants allocated
// Pre(s):   size is a positive integer
// Post(s):  data is allocated to the heap ...
// Returns:  pointer to data section (incr over int and *)
// new block points to free_head
//==============================================================================================================
void* my_malloc(int alloc_size){
    for(struct Block* temp=free_head; temp; temp=temp->next_block){
        // this block gets the predecessor for when you can't split
        // if requested size value < current block size, set it to predecessor
        struct Block* previous;
        if(temp->next_block != NULL){
            if(temp->next_block->block_size > alloc_size){
                previous = temp;
            }
        }

        // can split: block size big enough
        if(temp->block_size >= alloc_size){
            if(temp->block_size-OVERHEAD-alloc_size >= VOID_SIZE){
                return split(alloc_size, temp);
            }
            else{
                // can't split: block is not big enough, iterate to next block
                if(temp->block_size < alloc_size){
                    previous->next_block = temp->next_block;
                    temp->block_size = alloc_size;
                    temp->next_block = NULL;
                    return (char*) temp + OVERHEAD; // data ptr
                }
                // can't split: block is big enough, update free_head
                else{
                    free_head = free_head->next_block;
                    previous->block_size = alloc_size;
                    previous->next_block = NULL;
                    print_fh(free_head);
                    return (char*) previous + OVERHEAD; // data ptr
                }
            }
        }
    }
    return NULL;
}

// ************************************************************************************************************
// ************************************************ TEST CASES ************************************************
// ************************************************************************************************************
void test1(){
    my_initialize_heap(10000);
    struct Block* alice = my_malloc(sizeof(int));
    printf("\n1: %p",alice);

    struct Block* bob = my_malloc(sizeof(int));
    printf("\n2: %p",bob);
    my_free(bob);
}

void test2(){
    my_initialize_heap(10000);
    struct Block* alice = my_malloc(15);
    printf("\nalice: %p",alice);

    struct Block* bob = my_malloc(15);
    printf("\nbob: %p",bob);
}

void test3(){
    my_initialize_heap(10000);
    struct Block* alice = my_malloc(15);
    printf("\nalice: %p",alice);

    struct Block* bob = my_malloc(15);
    printf("\nbob: %p",bob);

    struct Block* charlie = my_malloc(15);
    printf("\ncharlie: %p", charlie);

    my_free(bob);

    struct Block* david = my_malloc(sizeof(double));
    printf("\ndavid: %p",david);
}

void test4(){
    my_initialize_heap(10000);
    struct Block* alice = my_malloc(15);
    printf("\nalice: %p",alice);

    struct Block* charlie = my_malloc(sizeof(char));
    printf("\ncharlie: %p",charlie);
}

void test5(){
    my_initialize_heap(10000);

    int arr[100];
    struct Block* isabelle = my_malloc(sizeof(arr));
    printf("\nisabelle: %p",isabelle);

    int i = 10;
    struct Block* alice = my_malloc(i);
    printf("\nint address: %p",alice);

    my_free(isabelle);
    printf("\n\nafter freeing...");
    printf("\nint address: %p",alice);
    printf("\nint val: %d",i);
}

void test_split(){
    int size = next_pow_of_two(100);
    my_initialize_heap(size);
    struct Block *p = my_malloc(4);
}

void test_program(){
    int num = 0;
    printf("Hi. Enter size of array: ");
    scanf("%d",&num);

    my_initialize_heap(num*sizeof(int)*OVERHEAD); // some size thats bigger than my_malloc
    int* x = (int *) my_malloc(num*sizeof(int));
    int* header = (char * )x - sizeof(struct Block);

    // input 
    int* temp = x;
    for(int i=0; i<num; i++){
        int n = 0;
        printf("enter number >> ");
        scanf("\n%d",&n);
        *temp = n;
        temp = (char *)temp + sizeof(int);
    }

    // calc std dev
    float sum = 0.0, std_dev = 0.0, mean;
    temp = x; // reset position to where x is
    for(int i=0; i<num; i++){
        sum += *temp;
        temp = (char *)temp + sizeof(int);
    }

    mean = sum/num; 
    temp = x; // reset position to where x is
    for(int i=0; i<num; i++){
        int n = *temp;
        temp = (char *)temp + sizeof(int);
        std_dev += pow(n-mean,2);
    }
    printf("std dev = %f", sqrt(std_dev/num));
}

void test_malloc(){
    int size = next_pow_of_two(100);
    //printf("asked for size %d", size);
    my_initialize_heap(size);
    struct Block *p = my_malloc(40);
    struct Block *q = my_malloc(10);
    //struct Block *r = my_alloc(8);
    my_free(p);
    printf("\n free p: ");
    print_fh(free_head);

    printf("\n free q: ");
    my_free(q);
    print_fh(free_head);

    struct Block *r = my_malloc(39);
    print_fh(free_head);
    my_free(r);
    print_fh(free_head);
    
    //struct Block *s = my_alloc(25);
    //struct Data *data_ptr = my_alloc(2);
}

int main(int argc, const char * argv[]) {
    printf("\n\n\n\n");

    //test1();
    //test2();
    //test3();
    //test4();
    //test5();
    //test_split();
    test_malloc();
    //test_program();

    printf("\n\n\n\n\n");
    return 0;
}