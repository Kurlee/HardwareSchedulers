#include "oslabs.h"
#include <stdlib.h>
#include <stdio.h>

struct PCB NULLPCB = {0, 0, 0, 0, 0, 0, 0};


int pcb_compare(struct PCB j, struct PCB k)
{
  return (
          j.process_id == k.process_id &&
          j.arrival_timestamp == k.arrival_timestamp &&
          j.total_bursttime ==  k.total_bursttime &&
          j.execution_starttime == k.execution_starttime &&
          j.execution_endtime == k.execution_endtime &&
          j.remaining_bursttime == k.remaining_bursttime &&
          j.process_priority == k.process_priority
          );
}


struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp)
{
  /* inputs
     1 the ready queue (array of pcb structs)
     2 the number of items in the ready queue
     3 the pcb of the currently running process
     4 the pcb of the newly arriving process
     5 the current timestamp

     struct PCB {
         int process_id;
         int arrival_timestamp;
         int total_bursttime;
         int execution_starttime;
         int execution_endtime;
         int remaining_bursttime;
         int process_priority;
     }
  */

  // if there is no current process, return the new process
  if (pcb_compare(NULLPCB, current_process))
   {
     new_process.execution_starttime = timestamp;
     new_process.execution_endtime = timestamp + new_process.total_bursttime;
     new_process.remaining_bursttime = new_process.total_bursttime;
     return new_process;
   }

  else if (new_process.process_priority > current_process.process_priority)
    {
      new_process.execution_starttime = 0;
      new_process.execution_endtime = 0;
      new_process.remaining_bursttime = new_process.total_bursttime;
      ready_queue[*queue_cnt] = new_process;
      *queue_cnt = *queue_cnt + 1;
      return current_process;
    }
  else
    {
      new_process.execution_starttime = timestamp;
      new_process.execution_endtime = timestamp + new_process.total_bursttime;
      new_process.remaining_bursttime = new_process.total_bursttime;
      current_process.execution_endtime = 0;
      current_process.remaining_bursttime = current_process.remaining_bursttime - (timestamp - current_process.execution_starttime);
      ready_queue[*queue_cnt] = current_process;
      *queue_cnt = *queue_cnt + 1;
      return new_process;
    }
}


struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp)
{
  if (*queue_cnt > 0)
    {
      int hi_pri = 0;
      for (int i = 1; i < *queue_cnt; i++)
        {
          if (ready_queue[hi_pri].process_priority > ready_queue[i].process_priority)
            {
              hi_pri = i;
            }
        }
      //we know the highest priority pcb location, return it
      struct PCB highest_priority = ready_queue[hi_pri];
        //shift all other elements up
      for (int i = hi_pri; i < *queue_cnt; i ++)
        {
          ready_queue[i] = ready_queue[i+1];
        }
      *queue_cnt = *queue_cnt - 1;
      highest_priority.execution_starttime = timestamp;
      highest_priority.execution_endtime = timestamp + highest_priority.remaining_bursttime;
      return highest_priority;
    }
  return NULLPCB;
}



struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp)
{
  //if no current process
  if (pcb_compare(current_process, NULLPCB))
    {
      new_process.execution_starttime = time_stamp;
      new_process.execution_endtime = time_stamp + new_process.total_bursttime;
      new_process.remaining_bursttime = new_process.total_bursttime;
      return new_process;
    }


  //if current process has shortest time
  else if (current_process.remaining_bursttime < new_process.remaining_bursttime)
    {
      new_process.execution_starttime = 0;
      new_process.execution_endtime = 0;
      new_process.remaining_bursttime = new_process.total_bursttime;
      ready_queue[*queue_cnt] = new_process;
      *queue_cnt = *queue_cnt + 1;
      return current_process;
    }

  //if new process has shortest time
  else
    {
      new_process.execution_starttime = time_stamp;
      new_process.execution_endtime = time_stamp + new_process.total_bursttime;
      new_process.remaining_bursttime = new_process.total_bursttime;
      current_process.execution_starttime = 0;
      current_process.execution_endtime = 0;
      ready_queue[*queue_cnt] = current_process;
      *queue_cnt = *queue_cnt + 1;
      return new_process;
    }
}



struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp)
{
  if (*queue_cnt > 0)
    {
      int srt = 0;
      for (int i = 1; i < *queue_cnt; i++)
        {
          if (ready_queue[srt].remaining_bursttime > ready_queue[i].remaining_bursttime)
            {
              srt = i;
            }
        }
      //we know the shortest reamining time pcb location, return it
      struct PCB highest_priority = ready_queue[srt];
      //shift all other elements up
      for (int i = srt; i < *queue_cnt; i ++)
        {
          ready_queue[i] = ready_queue[i+1];
        }
      *queue_cnt = *queue_cnt - 1;
      highest_priority.execution_starttime = timestamp;
      highest_priority.execution_endtime = timestamp + highest_priority.remaining_bursttime;
      return highest_priority;
    }
  return NULLPCB;
}


struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum)
{
  if (pcb_compare(current_process, NULLPCB))
    {
      new_process.execution_starttime = timestamp;
      if (new_process.total_bursttime > time_quantum)
        {
          new_process.execution_endtime = timestamp + time_quantum;
        }
      else
        {
          new_process.execution_endtime = timestamp + new_process.total_bursttime;
        }
      new_process.remaining_bursttime = new_process.total_bursttime;
      return new_process;
    }

  //if current process has shortest time
  else
    {
      new_process.execution_starttime = 0;
      new_process.execution_endtime = 0;
      new_process.remaining_bursttime = new_process.total_bursttime;
      ready_queue[*queue_cnt] = new_process;
      *queue_cnt = *queue_cnt + 1;
      return current_process;
    }
}



struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int time_stamp, int time_quantum)
{
  if (*queue_cnt > 0)
    {
      int rr = 0;
      for (int i = 1; i < *queue_cnt; i++)
        {
          if (ready_queue[rr].arrival_timestamp > ready_queue[i].arrival_timestamp)
            {
              rr = i;
            }
        }
      //we know the shortest reamining time pcb location, return it
      struct PCB highest_priority = ready_queue[rr];
      //shift all other elements up
      for (int i = rr; i < *queue_cnt; i ++)
        {
          ready_queue[i] = ready_queue[i+1];
        }
      *queue_cnt = *queue_cnt - 1;
      highest_priority.execution_starttime = time_stamp;
      if (highest_priority.total_bursttime > time_quantum)
        {
          highest_priority.execution_endtime = time_stamp + time_quantum;
        }
      else
        {
          highest_priority.execution_endtime = time_stamp + highest_priority.total_bursttime;
        }
      return highest_priority;
    }
  return NULLPCB;
}
/*
//this method compares two PCB structs, returns true if all values are equal
  int main(int argc, char *argv[])
  {
    struct PCB not_null = {1, 1, 4, 0, 0, 4, 23};
    struct PCB process1 = {2, 1, 4, 0, 0, 4, 22};
    struct PCB process2 = {3, 1, 4, 0, 0, 4, 24};
    struct PCB test1[10] = {not_null, process1, process2};
    struct PCB test2[10] = {process2, not_null, process1};
    struct PCB test3[10] = {process1, process2, not_null};
    struct PCB test4[10] = {not_null, process2, process1};
    struct PCB test5[10] = {process1, not_null, process2};
    struct PCB test6[10] = {process2, process1, not_null};
    int queue_count = 3;
    struct PCB next = handle_process_completion_pp(empty, &queue_count, 2);
    printf("The next process is :PID:%d, AT:%d, TBT:%d, EST:%d, EET:%d, RBT:%d, PRI:%d\n", next.process_id, next.arrival_timestamp, next.total_bursttime, next.execution_starttime, next.execution_endtime, next.remaining_bursttime, next.process_priority);
    printf("The number of items in the queue is: %d\n", queue_count);
    printf("Structs Left in queue\n");
    for (int i = 0; i < queue_count; i++)
      {
        printf("PID:%d, AT:%d, TBT:%d, EST:%d, EET:%d, RBT:%d, PRI:%d\n", empty[i].process_id, empty[i].arrival_timestamp, empty[i].total_bursttime, empty[i].execution_starttime, empty[i].execution_endtime, empty[i].remaining_bursttime, empty[i].process_priority);
      }
    return 0;
}
*/
