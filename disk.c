#include "oslabs.h"
#include <stdlib.h>
#include <stdio.h>

struct RCB NULLRCB = {0, 0, 0, 0, 0};




int rcb_compare(struct RCB j, struct RCB k)
{
  return (
          j.request_id == k.request_id &&
          j.arrival_timestamp == k.arrival_timestamp &&
          j.cylinder ==  k.cylinder &&
          j.address ==  k.address &&
          j.process_id == k.process_id
          );
}




struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int time_stamp)
{
  if (rcb_compare(current_request, NULLRCB))
    {
      return new_request;
    }
    request_queue[*queue_cnt] = new_request;
    *queue_cnt = *queue_cnt + 1;
    return current_request;
}




struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX],int *queue_cnt)
{
  if (*queue_cnt == 0)
    return NULLRCB;
  int eai = 0;
  for (int i = 1; i < *queue_cnt; i++)
    {
      if (request_queue[i].arrival_timestamp < request_queue[eai].arrival_timestamp)
        {
          eai = i;
        }
    }
  struct RCB next_rcb = request_queue[eai];
  for (int i = eai - 1; i < *queue_cnt - 1; i ++)
    {
      request_queue[i] = request_queue[i+1];
    }
  *queue_cnt = *queue_cnt - 1;
  return next_rcb;
}




struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int time_stamp)
{
  if (rcb_compare(current_request, NULLRCB))
    {
      return new_request;
    }
  request_queue[*queue_cnt] = new_request;
  *queue_cnt = *queue_cnt + 1;
  return current_request;
}




struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt,int current_cylinder)
{
  if (*queue_cnt == 0)
    return NULLRCB;


  struct REL_REQ { // struct to track rcbs in queue and their relative closeness to the current cylinder
	  struct RCB rcb; // an RCB in the queue
	  int dist_cylinder; // distance from the requested cylinder to the current cylinder
	  int is_closest; // marked as one of the rcb's requesting the next closest cylinder
  };
  struct REL_REQ rr[*queue_cnt];

  int cr; // closest request made by any rcb
  // calculates the distance to the next cylinder for each rcb
  // marks the shortest distance for use later
  for (int i = 0; i < *queue_cnt; i++)
  {
	  rr[i].rcb = request_queue[i];
	  rr[i].is_closest = 0;
	  // This is kinda hackey. the online compiler does not include math.h.
	  // so this will effectively be distance squared, but it shouldnt matter
	  // larger numbers mean farther away, smaller means closer.
	  rr[i].dist_cylinder = (request_queue[i].cylinder - current_cylinder) * (request_queue[i].cylinder - current_cylinder);
	  if (i == 0)
		  cr = rr[i].dist_cylinder;
	  else if (cr > rr[i].dist_cylinder)
		  cr = rr[i].dist_cylinder;

  }

  // marks all rcb's which are requesting the next closest rcb
  // the earliest arriving of these will be the next chosen
  for (int i = 0; i < *queue_cnt; i++)
  {
	  if (rr[i].dist_cylinder == cr)
		  rr[i].is_closest = 1;
  }


  // of the rcb's marked as requesting closest, determine which arrived first
  int firsthit = 1;
  struct RCB nextrcb;
  int index_of_nextrcb;
  for (int i = 0; i < *queue_cnt; i++)
  {
	  if (firsthit == 1 && rr[i].is_closest == 1)
	  {
		  nextrcb = rr[i].rcb;
		  firsthit = 0;
		  index_of_nextrcb = i;
	  }
	  else if (rr[i].is_closest == 1 && rr[i].rcb.arrival_timestamp < nextrcb.arrival_timestamp)
	  {
		  nextrcb = rr[i].rcb;
		  index_of_nextrcb = i;
	  }
          
  }


  // we know which rcb will be returned and its position in the array. remove it from the array
  for (int i = index_of_nextrcb; i < *queue_cnt - 1; i ++)
    {
      request_queue[i] = request_queue[i+1];
    }
  *queue_cnt = *queue_cnt - 1;
  return nextrcb;
}





struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int time_stamp)
{
    if (rcb_compare(current_request, NULLRCB))
      {
        return new_request;
      }
    request_queue[*queue_cnt] = new_request;
    *queue_cnt = *queue_cnt + 1;
    return current_request;
}




struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX],int  *queue_cnt, int current_cylinder, int scan_direction)
{
    if (*queue_cnt == 0)
        return NULLRCB;
    
    
    struct REL_REQ { // struct to track rcbs in queue and their relative closeness to the current cylinder
        struct RCB rcb; // an RCB in the queue
        int dist_cylinder; // distance from the requested cylinder to the current cylinder
        int is_closest; // marked as one of the rcb's requesting the next closest cylinder
    };
    struct REL_REQ rr[*queue_cnt];
    
    int cr; // closest request made by any rcb
    // calculates the distance to the next cylinder for each rcb
    // marks the shortest distance for use later
    for (int i = 0; i < *queue_cnt; i++)
    {
        rr[i].rcb = request_queue[i];
        rr[i].is_closest = 0;
        // This is kinda hackey. the online compiler does not include math.h.
        // so this will effectively be distance squared, but it shouldnt matter
        // larger numbers mean farther away, smaller means closer.
        rr[i].dist_cylinder = (request_queue[i].cylinder - current_cylinder) * (request_queue[i].cylinder - current_cylinder);
        if (i == 0)
            cr = rr[i].dist_cylinder;
        else if (cr > rr[i].dist_cylinder)
            cr = rr[i].dist_cylinder;
        
    }
    
    // marks all rcb's which are requesting the next closest rcb
    // the earliest arriving of these will be the next chosen
    for (int i = 0; i < *queue_cnt; i++)
    {
        if (rr[i].dist_cylinder == cr)
            rr[i].is_closest = 1;
    }
    
    
    // of the rcb's marked as requesting closest, determine which arrived first
    int firsthit = 1;
    struct RCB nextrcb;
    int index_of_nextrcb;
    for (int i = 0; i < *queue_cnt; i++)
    {
        if (firsthit == 1 && rr[i].is_closest == 1)
        {
            nextrcb = rr[i].rcb;
            firsthit = 0;
            index_of_nextrcb = i;
        }
        else if (rr[i].is_closest == 1 && rr[i].rcb.arrival_timestamp < nextrcb.arrival_timestamp)
        {
            nextrcb = rr[i].rcb;
            index_of_nextrcb = i;
        }
        
    }
    
    
    // we know which rcb will be returned and its position in the array. remove it from the array
    for (int i = index_of_nextrcb; i < *queue_cnt - 1; i ++)
    {
        request_queue[i] = request_queue[i+1];
    }
    *queue_cnt = *queue_cnt - 1;
    return nextrcb;
}
/*
int main (int argc, char *argvp[])
{
    struct RCB first = {1, 72, 45, 45, 1};
    struct RCB second = {2, 71, 47, 47, 2};
    struct RCB third = {3, 73,43, 43, 3};
    struct RCB request_queue[10] = {first, second, third};
    int queue_cnt = 3;

    struct RCB nextrcb = handle_request_completion_sstf(request_queue, &queue_cnt, 48);
    printf("The RID of the next process is: %d\n", nextrcb.request_id);
    for (int i = 0; i < queue_cnt; i++)
    {
        printf("RID: %d\n", request_queue[i].request_id);
    }
    return 0;

}
*/
