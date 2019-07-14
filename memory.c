#include <stdio.h>
#include <stdlib.h>
#include "oslabs.h"
/*
    struct MEMORY_BLOCK {
       int start_address;
       int end_address;
       int segment_size;
       int process_id; //0 indicates a free block
    }
*/


struct MEMORY_BLOCK NULLBLOCK = {0,0,0,0};

struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id)
{
    int fit;        // variable to hold the difference between the requested block and the next potential block
    int next_block = -1;      //  index of next block to be used
    int first_pass = 1;
    for (int i = 0; i < *map_cnt; i++)
    {
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size && first_pass)
        {
            fit = memory_map[i].segment_size - request_size;
            next_block = i;
            first_pass = 0;
        }
        else if (memory_map[i].process_id == 0 && (memory_map[i].segment_size - request_size) > 0 && (memory_map[i].segment_size - request_size) < fit)
        {
            fit = (memory_map[i].segment_size - request_size);
            next_block = i;
        }
    }
    if (next_block == -1)
        return NULLBLOCK;
    else if (fit == 0)
    {
        memory_map[next_block].process_id = process_id;
        return memory_map[next_block];
    }
    else
    {
        *map_cnt = *map_cnt + 1;
        for ( int i = *map_cnt; i > next_block; i--)
        {
            memory_map[i] = memory_map[i - 1];
        }
        memory_map[next_block].start_address = memory_map[next_block + 1].start_address;
        memory_map[next_block].end_address = memory_map[next_block].start_address + (request_size - 1);
        memory_map[next_block].segment_size = request_size;
        memory_map[next_block].process_id = process_id;
        memory_map[next_block + 1].segment_size = memory_map[next_block + 1].segment_size - request_size;
        memory_map[next_block + 1].start_address = memory_map[next_block + 1].start_address + request_size;
        return memory_map[next_block];
    }

}




struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id)
{
    int next_block = -1;      //  index of next block to be used
    for (int i = 0; i < *map_cnt; i++)
    {
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size)
        {
            next_block = i;
            break;
        }
    }
    if (next_block == -1)
        return NULLBLOCK;
    else if ((memory_map[next_block].segment_size - request_size) == 0)
    {
        memory_map[next_block].process_id = process_id;
        return memory_map[next_block];
    }
    else
    {
        *map_cnt = *map_cnt + 1;
        for ( int i = *map_cnt; i > next_block; i--)
        {
            memory_map[i] = memory_map[i - 1];
        }
        memory_map[next_block].start_address = memory_map[next_block + 1].start_address;
        memory_map[next_block].end_address = memory_map[next_block].start_address + (request_size - 1);
        memory_map[next_block].segment_size = request_size;
        memory_map[next_block].process_id = process_id;
        memory_map[next_block + 1].segment_size = memory_map[next_block + 1].segment_size - request_size;
        memory_map[next_block + 1].start_address = memory_map[next_block + 1].start_address + request_size;
        return memory_map[next_block];
    }

}





struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id)
{
    int fit;        // variable to hold the difference between the requested block and the next potential block
    int next_block = -1;      //  index of next block to be used
    int first_pass = 1;
    for (int i = 0; i < *map_cnt; i++)
    {
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size && first_pass)
        {
            fit = memory_map[i].segment_size - request_size;
            next_block = i;
            first_pass = 0;
        }
        else if (memory_map[i].process_id == 0 && (memory_map[i].segment_size - request_size) > 0 && (memory_map[i].segment_size - request_size) > fit)
        {
            fit = (memory_map[i].segment_size - request_size);
            next_block = i;
        }
    }
    if (next_block == -1)
        return NULLBLOCK;
    else if (fit == 0)
    {
        memory_map[next_block].process_id = process_id;
        return memory_map[next_block];
    }
    else
    {
        *map_cnt = *map_cnt + 1;
        for ( int i = *map_cnt; i > next_block; i--)
        {
            memory_map[i] = memory_map[i - 1];
        }
        memory_map[next_block].start_address = memory_map[next_block + 1].start_address;
        memory_map[next_block].end_address = memory_map[next_block].start_address + (request_size - 1);
        memory_map[next_block].segment_size = request_size;
        memory_map[next_block].process_id = process_id;
        memory_map[next_block + 1].segment_size = memory_map[next_block + 1].segment_size - request_size;
        memory_map[next_block + 1].start_address = memory_map[next_block + 1].start_address + request_size;
        return memory_map[next_block];
    }

}




struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id, int    last_address)
{
    int next_block = -1;      //  index of next block to be used
    for (int i = 0; i < *map_cnt; i++)
    {
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size && memory_map[i].start_address >= last_address)
        {
            next_block = i;
            break;
        }
    }
    if (next_block == -1)
        return NULLBLOCK;
    else if ((memory_map[next_block].segment_size - request_size) == 0)
    {
        memory_map[next_block].process_id = process_id;
        return memory_map[next_block];
    }
    else
    {
        *map_cnt = *map_cnt + 1;
        for ( int i = *map_cnt; i > next_block; i--)
        {
            memory_map[i] = memory_map[i - 1];
        }
        memory_map[next_block].start_address = memory_map[next_block + 1].start_address;
        memory_map[next_block].end_address = memory_map[next_block].start_address + (request_size - 1);
        memory_map[next_block].segment_size = request_size;
        memory_map[next_block].process_id = process_id;
        memory_map[next_block + 1].segment_size = memory_map[next_block + 1].segment_size - request_size;
        memory_map[next_block + 1].start_address = memory_map[next_block + 1].start_address + request_size;
        return memory_map[next_block];
    }

}



void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt)
{
    int fbi; // index to hold the position of the freed block in the memory map
    for (int i = 0; i < *map_cnt; i++)
    {
        if (freed_block.process_id == memory_map[i].process_id)
        {
            fbi = i;
            break;
        }
    }
    if (memory_map[fbi-1].process_id == 0 && memory_map[fbi + 1].process_id ==0)
    {
        memory_map[fbi - 1].end_address = memory_map[fbi + 1].end_address;
        memory_map[fbi - 1].segment_size = (memory_map[fbi -1].end_address - memory_map[fbi - 1].start_address) + 1;
        *map_cnt = *map_cnt - 2;
        for (int i = fbi; i < *map_cnt; i++)
            memory_map[i] = memory_map[i+2];
    }
    else if (memory_map[fbi-1].process_id == 0)
    {
        memory_map[fbi - 1].end_address = memory_map[fbi].end_address;
        memory_map[fbi - 1].segment_size = (memory_map[fbi -1].end_address - memory_map[fbi - 1].start_address) + 1;
        *map_cnt = *map_cnt - 1;
        for (int i = fbi; i < *map_cnt; i++)
            memory_map[i] = memory_map[i+1];
    }
    else if (memory_map[fbi+1].process_id == 0)
    {
        memory_map[fbi].end_address = memory_map[fbi+1].end_address;
        memory_map[fbi].segment_size = (memory_map[fbi].end_address - memory_map[fbi].start_address) + 1;
        memory_map[fbi].process_id = 0;
        *map_cnt = *map_cnt - 1;
        for (int i = fbi + 1; i < *map_cnt; i++)
            memory_map[i] = memory_map[i+1];
    }
    else
    {
        memory_map[fbi].process_id = 0;
    }
}
/*
int main()
{
    struct MEMORY_BLOCK zero = {0,1023,1024,0};
    struct MEMORY_BLOCK one = {1024,1044,21,1};
    struct MEMORY_BLOCK two = {1045,1144,100,0};
    struct MEMORY_BLOCK three = {1145,2144,1000,2};
    struct MEMORY_BLOCK memory_map[10] = {zero, one, two, three};
    int  map_cnt = 4;
    int request_size = 10;
    int process_id = 32;
    struct MEMORY_BLOCK next_block = next_fit_allocate(request_size, memory_map, &map_cnt, process_id, 0);
    printf("    Memory Block Map\n  ------------------\n");
    for (int i = 0; i < map_cnt; i++)
    {
        printf("    Block #: %d, Start Address: %d, End Address: %d Segment Size: %d, Process ID: %d\n", i, memory_map[i].start_address, memory_map[i].end_address, memory_map[i].segment_size, memory_map[i].process_id);
    }
    printf("    Map count: %d\n", map_cnt);
    printf("    NEXT BLOCK Start Address: %d, End Address: %d Segment Size: %d, Process ID: %d\n", next_block.start_address, next_block.end_address, next_block.segment_size, next_block.process_id);

    return 0;
    
}
*/
