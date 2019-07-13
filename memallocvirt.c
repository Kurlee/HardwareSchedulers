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
      *frame_cnt = *frame_cnt - 1;
      page_table[page_number].frame_number = frame_pool[*frame_cnt];
      return page_table[page_number].frame_number;
    }

  else 
  {
      struct PTE EAV; // earliest arriving valid pte
      int EAVI; // earliest arriving valid pte index
      int first_pass; // marker for first pass through loop
      for (int i = 0; i < *table_cnt; i++)
      {
          if (page_table[i].is_valid && first_pass)
          {
              EAV = page_table[i];
              EAVI = i;
              first_pass = 0;
          }
          else if (page_table[i].is_valid && page_table[i].arrival_timestamp < EAV.arrival_timestamp)
          {
              EAV = page_table[i];
              EAVI = i;
          }
      }
      page_table[EAVI].is_valid = 0;
      page_table[EAVI].arrival_timestamp = 0;
      page_table[EAVI].last_access_timestamp = 0;
      page_table[EAVI].reference_count = 0;
      page_table[page_number].frame_numner = page_table[page_number].frame_number;
      return page_table[page_number].frame_number;
  }
}




int count_page_faults_fifo(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt)
{

}
int process_page_access_lru(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp); 
int count_page_faults_lru(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt);
int process_page_access_lfu(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp); 
int count_page_faults_lfu(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt);

