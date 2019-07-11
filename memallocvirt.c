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
      frame_pool[*frame_cnt]
    }
int count_page_faults_fifo(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt);
int process_page_access_lru(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp); 
int count_page_faults_lru(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt);
int process_page_access_lfu(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp); 
int count_page_faults_lfu(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt);
