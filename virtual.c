#include "oslabs.h"
#include <stdlib.h>
#include <stdio.h>


/*
struct PTE {
  int is_valid;
  int frame_number;
  int arrival_timestamp;
  int last_access_timestamp;
  int reference_count;
};
*/



int process_page_access_fifo(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp)
{
  //if the PTE at page_number is in memory, return that frame number
  if (page_table[page_number].is_valid == 1)
    {
      page_table[page_number].last_access_timestamp = current_timestamp;
      page_table[page_number].reference_count = page_table[page_number].reference_count + 1;
      return page_table[page_number].frame_number;
    }

  else if (*frame_cnt > 0)
    {
        int next_frame = frame_pool[*frame_cnt - 1];
        frame_pool[*frame_cnt - 1] = frame_pool[*frame_cnt];
        *frame_cnt = *frame_cnt - 1;
        page_table[page_number].is_valid = 1;
        page_table[page_number].frame_number = frame_pool[*frame_cnt];
        page_table[page_number].arrival_timestamp = current_timestamp;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        page_table[page_number].frame_number = next_frame;
        return page_table[page_number].frame_number;
    }

  else 
  {
      int eavi; // earliest arriving valid pte index
      int first_pass = 1; // marker for first pass through loop
      for (int i = 0; i < *table_cnt; i++)
      {
          if (page_table[i].is_valid && first_pass)
          {
              eavi = i;
              first_pass = 0;
          }
          else if (page_table[i].is_valid && page_table[i].arrival_timestamp < page_table[eavi].arrival_timestamp)
          {
              eavi = i;
          }
      }
      page_table[eavi].is_valid = 0;
      page_table[eavi].arrival_timestamp = 0;
      page_table[eavi].last_access_timestamp = 0;
      page_table[eavi].reference_count = 0;
      page_table[page_number].is_valid = 1;
      page_table[page_number].arrival_timestamp = current_timestamp;
      page_table[page_number].last_access_timestamp = current_timestamp;
      page_table[page_number].reference_count = 1;
      page_table[page_number].frame_number = page_table[eavi].frame_number;
      page_table[eavi].frame_number = -1;
      return page_table[page_number].frame_number;
  }
}





int count_page_faults_fifo(struct PTE page_table[TABLEMAX],int table_cnt, int reference_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt)
{
    int faults = 0;
    int timestamp = 0;
    for (int i = 0; i < reference_cnt; i++ )
    {
        int j = reference_string[i]; // This variable holds the page number being looked up
        // page being referenced is in memory
        if (page_table[j].is_valid)
        {
            page_table[j].last_access_timestamp = timestamp;// timestamp 
            page_table[j].reference_count = page_table[j].reference_count + 1;
            timestamp = timestamp + 1;
        }
        // page being referenced is NOT in memory AND there ARE empty frames
        else if (frame_cnt > 0)
        {
            int next_frame = frame_pool[frame_cnt - 1];
            frame_pool[frame_cnt - 1] = frame_pool[frame_cnt];
            frame_cnt = frame_cnt - 1;
            page_table[j].is_valid = 1;
            page_table[j].frame_number = next_frame;
            page_table[j].arrival_timestamp = timestamp;
            page_table[j].last_access_timestamp = timestamp;
            page_table[j].reference_count = 1;
            timestamp = timestamp + 1;
            faults = faults + 1;
        }
        // page being referenced is NOT in memory AND there ARE NO empty frames
        else
        {
          int eavi = -1;; // earliest arriving valid pte index
          int first_pass = 1; // marker for first pass through loop
          for (int i = 0; i < table_cnt; i++)
          {
              if (page_table[i].is_valid && first_pass)
              {
                  eavi = i;
                  first_pass = 0;
              }
              else if (page_table[i].is_valid && page_table[i].arrival_timestamp < page_table[eavi].arrival_timestamp)
              {
                  eavi = i;
              }
          }
          page_table[eavi].is_valid = 0;
          page_table[eavi].arrival_timestamp = 0;
          page_table[eavi].last_access_timestamp = 0;
          page_table[eavi].reference_count = 0;
          page_table[j].is_valid = 1;
          page_table[j].frame_number = page_table[eavi].frame_number;
          page_table[j].arrival_timestamp = timestamp;
          page_table[j].last_access_timestamp = timestamp;
          page_table[j].reference_count = 1;
          timestamp = timestamp + 1;
          faults = faults + 1;
        }
    }
    return faults;
}
          

    

int process_page_access_lru(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp)
{
  //if the PTE at page_number is in memory, return that frame number
  if (page_table[page_number].is_valid == 1)
    {
      page_table[page_number].last_access_timestamp = current_timestamp;
      page_table[page_number].reference_count = page_table[page_number].reference_count + 1;
      return page_table[page_number].frame_number;
    }
// the page being referenced is not in memory and there are free frames
  else if (*frame_cnt > 0)
    {
        int next_frame = frame_pool[*frame_cnt - 1];
        frame_pool[*frame_cnt - 1] = frame_pool[*frame_cnt];
        *frame_cnt = *frame_cnt - 1;
        page_table[page_number].is_valid = 1;
        page_table[page_number].frame_number = frame_pool[*frame_cnt];
        page_table[page_number].arrival_timestamp = current_timestamp;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        page_table[page_number].frame_number = next_frame;
        return page_table[page_number].frame_number;
    }
// the page being referenced is not in memory and there are no free frames
  else 
  {
      int eavi; // earliest arriving valid pte index
      int first_pass = 1; // marker for first pass through loop
      for (int i = 0; i < *table_cnt; i++)
      {
          if (page_table[i].is_valid && first_pass)
          {
              eavi = i;
              first_pass = 0;
          }
          else if (page_table[i].is_valid && page_table[i].last_access_timestamp < page_table[eavi].last_access_timestamp)
          {
              eavi = i;
          }
      }
      page_table[eavi].is_valid = 0;
      page_table[eavi].arrival_timestamp = 0;
      page_table[eavi].last_access_timestamp = 0;
      page_table[eavi].reference_count = 0;
      page_table[page_number].is_valid = 1;
      page_table[page_number].arrival_timestamp = current_timestamp;
      page_table[page_number].last_access_timestamp = current_timestamp;
      page_table[page_number].reference_count = 1;
      page_table[page_number].frame_number = page_table[eavi].frame_number;
      page_table[eavi].frame_number = -1;
      return page_table[page_number].frame_number;
  }
}




int count_page_faults_lru(struct PTE page_table[TABLEMAX],int table_cnt, int reference_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt)
{
    int faults = 0;
    int timestamp = 0;
    for (int i = 0; i < reference_cnt; i++ )
    {
        int j = reference_string[i]; // This variable holds the page number being looked up
        // page being referenced is in memory
        if (page_table[j].is_valid)
        {
            page_table[j].last_access_timestamp = timestamp;// timestamp 
            page_table[j].reference_count = page_table[j].reference_count + 1;
            timestamp = timestamp + 1;
        }
        // page being referenced is NOT in memory AND there ARE empty frames
        else if (frame_cnt > 0)
        {
            int next_frame = frame_pool[frame_cnt - 1];
            frame_pool[frame_cnt - 1] = frame_pool[frame_cnt];
            frame_cnt = frame_cnt - 1;
            page_table[j].is_valid = 1;
            page_table[j].frame_number = next_frame;
            page_table[j].arrival_timestamp = timestamp;
            page_table[j].last_access_timestamp = timestamp;
            page_table[j].reference_count = 1;
            timestamp = timestamp + 1;
            faults = faults + 1;
        }
        // page being referenced is NOT in memory AND there ARE NO empty frames
        else
        {
          int eavi = -1; // earliest arriving valid pte index
          int first_pass = 1; // marker for first pass through loop
          for (int i = 0; i < table_cnt; i++)
          {
              if (page_table[i].is_valid && first_pass)
              {
                  eavi = i;
                  first_pass = 0;
              }
              else if (page_table[i].is_valid && page_table[i].last_access_timestamp < page_table[eavi].last_access_timestamp)
              {
                  eavi = i;
                  
              }
          }
            page_table[eavi].is_valid = 0;
            page_table[eavi].arrival_timestamp = 0;
            page_table[eavi].last_access_timestamp = 0;
            page_table[eavi].reference_count = 0;
            page_table[j].is_valid = 1;
            page_table[j].frame_number = page_table[eavi].frame_number;
            page_table[j].arrival_timestamp = timestamp;
            page_table[j].last_access_timestamp = timestamp;
            page_table[j].reference_count = 1;
            timestamp = timestamp + 1;
            faults = faults + 1;
        }
    }
    return faults;
}

    

int process_page_access_lfu(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp) 
{
  //if the PTE at page_number is in memory, return that frame number
  if (page_table[page_number].is_valid == 1)
    {
      page_table[page_number].last_access_timestamp = current_timestamp;
      page_table[page_number].reference_count = page_table[page_number].reference_count + 1;
      return page_table[page_number].frame_number;
    }
// the page being referenced is not in memory and there are free frames
  else if (*frame_cnt > 0)
    {
        int next_frame = frame_pool[*frame_cnt - 1];
        frame_pool[*frame_cnt - 1] = frame_pool[*frame_cnt];
        *frame_cnt = *frame_cnt - 1;
        page_table[page_number].is_valid = 1;
        page_table[page_number].frame_number = frame_pool[*frame_cnt];
        page_table[page_number].arrival_timestamp = current_timestamp;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        page_table[page_number].frame_number = next_frame;
        return page_table[page_number].frame_number;
    }
// the page being referenced is not in memory and there are no free frames
  else 
  {
      int eavi; // earliest arriving valid pte index
      int first_pass = 1; // marker for first pass through loop
      for (int i = 0; i < *table_cnt; i++)
      {
          if (page_table[i].is_valid && first_pass)
          {
              eavi = i;
              first_pass = 0;
          }
          else if (page_table[i].is_valid && (page_table[i].reference_count <= page_table[eavi].reference_count && page_table[i].arrival_timestamp < page_table[eavi].arrival_timestamp))
          {
              eavi = i;
          }
      }
      page_table[eavi].is_valid = 0;
      page_table[eavi].arrival_timestamp = 0;
      page_table[eavi].last_access_timestamp = 0;
      page_table[eavi].reference_count = 0;
      page_table[page_number].is_valid = 1;
      page_table[page_number].arrival_timestamp = current_timestamp;
      page_table[page_number].last_access_timestamp = current_timestamp;
      page_table[page_number].reference_count = 1;
      page_table[page_number].frame_number = page_table[eavi].frame_number;
      page_table[eavi].frame_number = -1;
      return page_table[page_number].frame_number;
  }
}





int count_page_faults_lfu(struct PTE page_table[TABLEMAX],int table_cnt, int reference_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt)
{
    int faults = 0;
    int timestamp = 0;
    for (int i = 0; i < reference_cnt; i++ )
    {
        int j = reference_string[i]; // This variable holds the page number being looked up
        // page being referenced is in memory
        if (page_table[j].is_valid)
        {
            page_table[j].last_access_timestamp = timestamp;// timestamp 
            page_table[j].reference_count = page_table[j].reference_count + 1;
            timestamp = timestamp + 1;
        }
        // page being referenced is NOT in memory AND there ARE empty frames
        else if (frame_cnt > 0)
        {
            int next_frame = frame_pool[frame_cnt - 1];
            frame_pool[frame_cnt - 1] = frame_pool[frame_cnt];
            frame_cnt = frame_cnt - 1;
            page_table[j].is_valid = 1;
            page_table[j].frame_number = next_frame;
            page_table[j].arrival_timestamp = timestamp;
            page_table[j].last_access_timestamp = timestamp;
            page_table[j].reference_count = 1;
            timestamp = timestamp + 1;
            faults = faults + 1;
        }
        // page being referenced is NOT in memory AND there ARE NO empty frames
        else
        {
          int eavi = -1; // earliest arriving valid pte index
          int first_pass = 1; // marker for first pass through loop
          for (int i = 0; i < table_cnt; i++)
          {
              if (page_table[i].is_valid && first_pass)
              {
                  eavi = i;
                  first_pass = 0;
              }
              else if (page_table[i].is_valid && (page_table[i].reference_count <= page_table[eavi].reference_count && page_table[i].arrival_timestamp < page_table[eavi].arrival_timestamp))
              {
                  eavi = i;
                  
              }
          }
            page_table[eavi].is_valid = 0;
            page_table[eavi].arrival_timestamp = 0;
            page_table[eavi].last_access_timestamp = 0;
            page_table[eavi].reference_count = 0;
            page_table[j].is_valid = 1;
            page_table[j].frame_number = page_table[eavi].frame_number;
            page_table[j].arrival_timestamp = timestamp;
            page_table[j].last_access_timestamp = timestamp;
            page_table[j].reference_count = 1;
            timestamp = timestamp + 1;
            faults = faults + 1;
        }
    }
    return faults;
}

          

/*
int main()
{
    struct PTE zero = {0,-1,-1,-1,-1};
    struct PTE one = {0,-1,-1,-1,-1};
    struct PTE two = {1,10,3,3,1};
    struct PTE three = {0,-1,-1,-1,-1};
    struct PTE four = {0,-1,-1,-1,-1};
    struct PTE five = {1,20,2,4,2};
    struct PTE six = {0,-1,-1,-1,-1};
    struct PTE seven = {1,30,1,1,1};
    struct PTE page_table[] = {zero, zero, zero, zero, zero, zero, zero, zero};
    int reference_string[] = {0, 3, 2, 6, 3, 4, 5, 2, 6, 4, 5};
    int reference_cnt = 11;
    int table_cnt = 8;
    int page_number = 0;
    int frame_pool[] = {0,1,2};
    int frame_cnt = 3;
    int current_timestamp = 12;
    int faults = count_page_faults_lfu(page_table, table_cnt, reference_string, reference_cnt, frame_pool, frame_cnt);
    printf("The page table contains the following.\n");
    for (int i = 0; i < table_cnt; i++)
    {
        printf("Page #: %d IV: %d FN: %d ATS: %d LATS: %d RC: %d\n",i, page_table[i].is_valid, page_table[i].frame_number, page_table[i].arrival_timestamp, page_table[i].last_access_timestamp, page_table[i].reference_count);
    }
    printf("Table Count: %d, Frame Count: %d, \nWith Fault Count:  %d\n", table_cnt, frame_cnt, faults);

}
*/
