#include <math.h>

// just helper functions

//==============================================================================================================
// Name:     round up
// Function: rounds to the nearest given multiple
// Input(s): integer, multiple to round to
// Pre(s):   input and multiple are positive integers
// Post(s):  input rounded to the multiple 
// Returns:  input
//==============================================================================================================
int round_up(int input, int multiple)
{
    if(multiple == 0){
        return input;
    }
    int rem = input % multiple;
    if(rem == 0){
        return input;
    }
    return input + multiple - rem;
}

//==============================================================================================================
// Name:     next_pow_of_two
// Function: computes the next highest power of 2
// Input(s): unsigned 32-bit integer
// Pre(s):   n is an int
// Post(s):  value returned is the next highest power of 2
// Returns:  the next highest power of 2
//==============================================================================================================
int next_pow_of_two(unsigned int n){
    int value = 1;
    while(value <= n){
        value = value << 1; //adding 1 gives the next base 2 logarithm
    }
    return value;
}

//==============================================================================================================
// Name:     print
// Function: prints the free list
// Input(s): head pointer to the free list
// Pre(s):   head is not null
// Post(s):  list is linked correctly
// Note:     i designed this function to test my links 
//==============================================================================================================
void print_fh(struct Block *a){
    printf("\nfree list: [");
    struct Block *walker = a;
    while(walker != NULL){
        printf("%d] -> [",walker->block_size);
        walker = walker->next_block;
    }
}