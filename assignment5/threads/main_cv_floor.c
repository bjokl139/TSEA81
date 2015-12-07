#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "lift_cv_floor.h"
#include "si_ui.h"
#include <sys/time.h>
#include <math.h>

#define ITERATIONS 1000
pthread_t passenger_thread_handles[MAX_N_PERSONS];
int passenger_ids[MAX_N_PERSONS];
long long int travel_time_array[MAX_N_PERSONS][ITERATIONS];

// Unfortunately the rand() function is not thread-safe. However, the
// rand_r() function is thread-safe, but need a pointer to an int to
// store the current state of the pseudo-random generator.  This
// pointer needs to be unique for every thread that might call
// rand_r() simultaneously. The functions below are wrappers around
// rand_r() which should work in the environment encountered in
// assignment 3.
//

void write_to_file(){
  int i,j;
  FILE *f = fopen("many_cv.txt", "w");
  if (f == NULL)
    {
      printf("Error opening file!\n");
      exit(1);
    }
  for (i = 0; i <  MAX_N_PERSONS; i++){
    for(j = 0; j <ITERATIONS; j++){
      fprintf(f, "%d ", travel_time_array[i][j]);
    }
    fprintf(f, "\n");
  }
  fclose(f);
}

static unsigned int rand_r_state[MAX_N_PERSONS];
// Get a random value between 0 and maximum_value. The passenger_id
// parameter is used to ensure that the rand_r() function is called in
// a thread-safe manner.

static int get_random_value(int passenger_id, int maximum_value)
{
	return rand_r(&rand_r_state[passenger_id]) % (maximum_value + 1);
}

static lift_type Lift;

// Initialize the random seeds used by the get_random_value() function
// above.
static void init_random(void)
{
	int i;
	for(i=0; i < MAX_N_PERSONS; i++){
		// Use this statement if the same random number sequence
		// shall be used for every execution of the program.
		rand_r_state[i] = i;

		// Use this statement if the random number sequence
		// shall differ between different runs of the
		// program. (As the current time (as measured in
		// seconds) is used as a seed.)
		rand_r_state[i] = i+time(NULL);
	}
}



static void *lift_thread(void *unused)
{
  int next_floor;
  int change_direction;
	while(1){
	  lift_next_floor(Lift, &next_floor, &change_direction);
	  lift_move(Lift, next_floor, change_direction);
	  pthread_mutex_lock(&Lift->mutex);
	  lift_has_arrived(Lift);
	  pthread_mutex_unlock(&Lift->mutex);
	}
	return NULL;
}

static void *passenger_thread(void *idptr)
{
	// Code that reads the passenger ID from the idptr pointer
	// (due to the way pthread_create works we need to first cast
	// the void pointer to an int pointer).

	int *tmp = (int *) idptr;
	int id = *tmp;
	int travel_counter = 0;
	
	for(travel_counter = 0; travel_counter < ITERATIONS; travel_counter++){
	  // * Select random floors
	  int start_floor = get_random_value(id, N_FLOORS-1);
	  int destination_floor = start_floor;
	  while(destination_floor == start_floor){
	    destination_floor = get_random_value(id, N_FLOORS-1);
	  }
	  
	  // * Travel between these floors
	  struct timeval starttime;
	  struct timeval stoptime;
	  long long int timediff;
	  int time_pos = 0;
	  
	  gettimeofday(&starttime, NULL);

	  lift_travel(Lift, id, start_floor, destination_floor);
	 
	  gettimeofday(&stoptime, NULL);
	  timediff = (stoptime.tv_sec*1000000ULL + stoptime.tv_usec) -
	    (starttime.tv_sec*1000000ULL + starttime.tv_usec);
	  travel_time_array[id][travel_counter] = timediff;
	  
	  // * Wait a little while
	  //usleep(5000000);
	  	
	  }
	  return NULL;
}

static void *user_thread(void *unused)
{
	int current_passenger_id = 0;
	char message[SI_UI_MAX_MESSAGE_SIZE]; 

	si_ui_set_size(670, 700); 
	
	while(1){
		// Read a message from the GUI
		si_ui_receive(message);
		if(!strcmp(message, "new")){
		  if(current_passenger_id < MAX_N_PERSONS){
		    pthread_create(&passenger_thread_handles[current_passenger_id], NULL, passenger_thread, &passenger_ids[current_passenger_id]);
		    current_passenger_id++;
		  }else{
		    si_ui_show_error("Person overflow! Building full!");
		  }

		}else if(!strcmp(message, "test")){
		  int i;
		  for (i = current_passenger_id; i < MAX_N_PERSONS; i++){
		    pthread_create(&passenger_thread_handles[current_passenger_id], NULL, passenger_thread, &passenger_ids[current_passenger_id]);
		    current_passenger_id++;
		  }
		}else if(!strcmp(message, "exit")){
			lift_delete(Lift);
			exit(0);
		}
	}
	return NULL;
}


int main(int argc, char **argv)
{
  si_ui_init();
  init_random();
  Lift = lift_create();
  
  int i;
  for(i = 0; i < MAX_N_PERSONS; i++){
    passenger_ids[i] = i;
  }
  
  /* set size of GUI window */ 
  si_ui_set_size(670, 700); 
  
  //draw_lift(Lift);
  
  pthread_t lift_thread_handle;
  pthread_t user_thread_handle;
  
  pthread_create(&lift_thread_handle,NULL,lift_thread,0);
  pthread_create(&user_thread_handle,NULL,user_thread,0);
  
  int current_passenger_id = 0;
  for (i = current_passenger_id; i < MAX_N_PERSONS; i++){
    pthread_create(&passenger_thread_handles[current_passenger_id], NULL, passenger_thread, &passenger_ids[current_passenger_id]);
    current_passenger_id++;
  }

  for(i = 0; i < MAX_N_PERSONS; i++){
    pthread_join(passenger_thread_handles[i],NULL);
  }

  write_to_file();
  exit(0);

  pthread_join(lift_thread_handle,NULL);
  pthread_join(user_thread_handle,NULL);
  return 0;
}
