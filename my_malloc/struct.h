
#include <stdio.h>
#include <stdlib.h> 

//each block manages the chunk of data to the right of it
struct Block{
    int block_size;             //num bytes in data
    struct Block *next_block;   //pointer to next block
};
