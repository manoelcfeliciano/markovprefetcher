#include "interface.hh"

const unsigned int MISS_STORE_SIZE = 50;
Addr miss_store[MISS_STORE_SIZE];
unsigned int last_index = 0;
const unsigned int FETCH_NUMBER = 3;

void insert_miss_store(Addr miss_addr)
{
  if (last_index == MISS_STORE_SIZE) { //If the array is full, reset last_index
    last_index = 0;
  }
  if (last_index == 0) { //Check to see if it is the first entry at the array
    miss_store[last_index] = miss_addr; //Insert the missed address at the array (first entry)
  } else { // Ain't the first entry
    miss_store[last_index++] = miss_addr; //Insert the missed address at the array
  }
}

unsigned int verify_miss_store(Addr addr)
{
  // Searches to see if the missed address is inside the array
  unsigned int i = 0; // Start value for i
  while(i <= MISS_STORE_SIZE) {
    if(miss_store[i] == addr) { //Check if the missed address is at the current position (iteration) of the array
      return i; //Return the index of the array (current value of i - iteration)
    }
    i++;
  }
  return -1; //Not found
}

void verify_and_prefetch(int index_miss)
{
  unsigned int i = index_miss + 1; //The first i is the index after the missed address index
  while (i<= last_index && i < index_miss + FETCH_NUMBER) { //FETCH_NUMBER determines how many addresses ahead will be prefetched
    if(!in_cache(miss_store[i])) { //Check if the address is already in cache
      issue_prefetch(miss_store[i]); //If not => prefetch
    }
    i++; //Iterate
  }
}

void prefetch_init(void)
{
    /* Called before any calls to prefetch_access. */
    /* This is the place to initialize data structures. */

    DPRINTF(HWPrefetch, "Initialized Markov prefetcher\n");
}

void prefetch_access(AccessStat stat)
{
  if (stat.miss) { // Miss detected
    int index_miss = verify_miss_store(stat.mem_addr); //Search the missed address in the array
    if (index_miss != -1) { //Check if the missed address is inside the array
      verify_and_prefetch(index_miss); //Missed address is inside the array
    }
    else { //Missed address is NOT inside the array
      insert_miss_store(stat.mem_addr); //Inserting the missed address in the array...
    }
  }
}

void prefetch_complete(Addr addr) {
}
