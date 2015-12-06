#include "lift.h"

/* Simple_OS include */ 
#include <pthread.h>

/* drawing module */ 
#include "draw.h"

/* standard includes */ 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>

#define ITERATIONS 10000

/* Statistics*/
int travel_count = 0;
long long int travel_times[ITERATIONS];

void do_math(){
  int i;
  long long int average_time = 0;
  long long int min = travel_times[0];
  long long int max = travel_times[0];
  
  for(i = 0; i < ITERATIONS; i++){
    average_time += travel_times[i];
    if(min > travel_times[i])
      min = travel_times[i];
    if(max < travel_times[i])
      max = travel_times[i];
  }
  average_time = average_time/ITERATIONS;
  
  long long int stddev_iter = 0;
  for(i = 0; i < ITERATIONS; i++){
    stddev_iter += pow(average_time - travel_times[i],2); 
  }
  int stddev = sqrt(stddev_iter/ITERATIONS);
  
  printf("Mean: %d\n", average_time);
  printf("Stddev: %d\n", stddev);
  printf("Min: %d\n", min);
  printf("Max: %d\n", max);

  FILE *f = fopen("one_cv.txt", "w");
  if (f == NULL)
    {
      printf("Error opening file!\n");
      exit(1);
    }
  for (i = 0; i < ITERATIONS; i++){
    fprintf(f, "%d\n", travel_times[i]);
  }
  fclose(f);
}

/* panic function, to be called when fatal errors occur */ 
static void lift_panic(const char message[])
{
    printf("LIFT_PANIC!!! "); 
    printf("%s", message); 
    printf("\n"); 
    exit(0); 
}

/* --- monitor data type for lift and operations for create and delete START --- */

/* lift_create: creates and initialises a variable of type lift_type */
lift_type lift_create(void) 
{
    /* the lift to be initialised */
    lift_type lift;

    /* floor counter */ 
    int floor; 

    /* loop counter */
    int i;

    /* allocate memory */
    lift = (lift_type) malloc(sizeof(lift_data_type));

    /* initialise variables */

    /* initialise floor */
    lift->floor = 0; 
    
    /* set direction of lift travel to up */

    lift->up = 1;

    /* the lift is not moving */ 
    lift->moving = 0; 

    /* initialise person information */
    for (floor = 0; floor < N_FLOORS; floor++)
    {
        for (i = 0; i < MAX_N_PERSONS; i++)
        {
            lift->persons_to_enter[floor][i].id = NO_ID; 
            lift->persons_to_enter[floor][i].to_floor = NO_FLOOR; 
        }
    }

    /* initialise passenger information */
    for (i = 0; i < MAX_N_PASSENGERS; i++) 
    {
        lift->passengers_in_lift[i].id = NO_ID; 
        lift->passengers_in_lift[i].to_floor = NO_FLOOR; 
    }

    /* initialise mutex and event variable */
    pthread_mutex_init(&lift->mutex,NULL);
    pthread_cond_init(&lift->change,NULL);

    return lift;
}

/* lift_delete: deallocates memory for lift */
void lift_delete(lift_type lift)
{
    free(lift);
}


/* --- monitor data type for lift and operations for create and delete END --- */


/* --- functions related to lift task START --- */

/* MONITOR function lift_next_floor: computes the floor to which the lift 
   shall travel. The parameter *change_direction indicates if the direction 
   shall be changed */
void lift_next_floor(lift_type lift, int *next_floor, int *change_direction)
{
  int floor;
  int up;
  
  pthread_mutex_lock(&lift->mutex);
  
  floor = lift->floor;
  up = lift->up;
  
  pthread_mutex_unlock(&lift->mutex);

  if(floor == N_FLOORS-1 && !up){
    *next_floor = floor - 1;
    *change_direction = 1;
  } else if (floor == 0 && up){
    *next_floor = floor + 1;
    *change_direction = 1;
  } else {
    *next_floor = floor - 2*up + 1;
    *change_direction = 0;
  }
}

void lift_move(lift_type lift, int next_floor, int change_direction)
{
    /* reserve lift */ 
    pthread_mutex_lock(&lift->mutex); 

    /* the lift is moving */ 
    lift->moving = 1; 
        
    /* release lift */ 
    pthread_mutex_unlock(&lift->mutex); 
        
    /* it takes two seconds to move to the next floor */ 
    //usleep(2000000);
        
    /* reserve lift */ 
    pthread_mutex_lock(&lift->mutex); 
        
    /* the lift is not moving */ 
    lift->moving = 0; 

    /* the lift has arrived at next_floor */ 
    lift->floor = next_floor; 

    /* check if direction shall be changed */ 
    if (change_direction)
    {
        lift->up = !lift->up; 
    }

    /* draw, since a change has occurred */ 
    //draw_lift(lift);
    
    /* release lift */ 
    pthread_mutex_unlock(&lift->mutex);
}

/* this function is used also by the person tasks */ 
static int n_passengers_in_lift(lift_type lift)
{
    int n_passengers = 0; 
    int i; 
        
    for (i = 0; i < MAX_N_PASSENGERS; i++)
    {
        if (lift->passengers_in_lift[i].id != NO_ID)
        {
            n_passengers++; 
        }
    }
    return n_passengers; 
}

/* MONITOR function lift_has_arrived: shall be called by the lift task
   when the lift has arrived at the next floor. This function indicates
   to other tasks that the lift has arrived, and then waits until the lift
   shall move again. */

int lift_should_stop(lift_type lift)
{
  int should_stop = 0;
  int i;

  for (i = 0; i < MAX_N_PASSENGERS; i ++){
    if (lift->passengers_in_lift[i].id != NO_ID)
      if(lift->passengers_in_lift[i].to_floor == lift->floor)
	should_stop = 1;
  }
  for (i = 0; i < MAX_N_PASSENGERS; i ++){
    if ((lift->persons_to_enter[lift->floor][i].id != NO_ID) && n_passengers_in_lift(lift) < MAX_N_PASSENGERS){
      should_stop = 1;
    }
  }

  return should_stop;
}

void lift_has_arrived(lift_type lift)
{
  pthread_cond_broadcast(&lift->change);
  while(lift_should_stop(lift)){
    pthread_cond_wait(&lift->change, &lift->mutex);
  }
}


/* --- functions related to lift task END --- */


/* --- functions related to person task START --- */

/* passenger_wait_for_lift: returns non-zero if the passenger shall
   wait for the lift, otherwise returns zero */
static int passenger_wait_for_lift(lift_type lift, int wait_floor)
{
  int waiting_ready =
    /* the lift is not moving */ 
    !lift->moving && 
    /* and the lift is at wait_floor */ 
    lift->floor == wait_floor && 
    /* and the lift is not full */ 
    n_passengers_in_lift(lift) < MAX_N_PASSENGERS; 
  
  return !waiting_ready;
}

static int passenger_should_leave_lift(lift_type lift, int destination_floor){
  return !lift->moving && lift->floor == destination_floor;
}

/* enter_floor: makes a person with id id stand at floor floor */ 
static void enter_floor(
    lift_type lift, int id, int floor)
{
    int i; 
    int floor_index; 
    int found; 

    /* stand at floor */ 
    found = 0; 
    for (i = 0; i < MAX_N_PERSONS && !found; i++)
    {
        if (lift->persons_to_enter[floor][i].id == NO_ID)
        {
            found = 1; 
            floor_index = i; 
        }
    }
        
    if (!found)
    {
        lift_panic("cannot enter floor"); 
    }

    /* enter floor at index floor_index */ 
    lift->persons_to_enter[floor][floor_index].id = id; 
    lift->persons_to_enter[floor][floor_index].to_floor = NO_FLOOR; 
}

/* leave_floor: makes a person with id id at enter_floor leave 
   enter_floor */ 
static void leave_floor(lift_type lift, int id, int enter_floor)

/* fig_end lift_c_prot */ 
{
    int i; 
    int floor_index; 
    int found; 

    /* leave the floor */
    found = 0; 
    for (i = 0; i < MAX_N_PERSONS && !found; i++)
    {
        if (lift->persons_to_enter[enter_floor][i].id == id)
        {
            found = 1; 
            floor_index = i; 
        }
    }
        
    if (!found)
    {
        lift_panic("cannot leave floor"); 
    }

    /* leave floor at index floor_index */ 
    lift->persons_to_enter[enter_floor][floor_index].id = NO_ID; 
    lift->persons_to_enter[enter_floor][floor_index].to_floor = NO_FLOOR; 
}

static void enter_lift(lift_type lift, int id, int destination_floor)
{
  int i;
  int sucess = 0;
  for(i = 0; i < MAX_N_PASSENGERS; i++){
    if(lift->passengers_in_lift[i].id == NO_ID){
      lift->passengers_in_lift[i].id = id;
      lift->passengers_in_lift[i].to_floor = destination_floor;
      sucess = 1;
      break;
    }
  }
  
  if(!sucess){lift_panic("Could not enter lift!");}
}

static void exit_lift(lift_type lift, int id){
  int i;
  int sucess = 0;
  for(i=0; i < MAX_N_PASSENGERS; i++){
    if(lift->passengers_in_lift[i].id == id){
      lift->passengers_in_lift[i].id = NO_ID;
      lift->passengers_in_lift[i].to_floor = NO_FLOOR;
      sucess = 1;
      break;
    }
  }
  if(!sucess){lift_panic("Could not exit lift!");}
}

/* MONITOR function lift_travel: performs a journey with the lift
   starting at from_floor, and ending at to_floor */ 
void lift_travel(lift_type lift, int id, int from_floor, int to_floor)
{
  struct timeval starttime;
  struct timeval stoptime;
  long long int timediff;
  int time_pos = 0;
  
  gettimeofday(&starttime, NULL);
  pthread_mutex_lock(&lift->mutex);
  enter_floor(lift, id, from_floor);
  //draw_lift(lift);

  while(passenger_wait_for_lift(lift, from_floor)){
    pthread_cond_wait(&lift->change, &lift->mutex);
  }
  leave_floor(lift, id, from_floor);
  enter_lift(lift, id, to_floor);
  //draw_lift(lift);
  pthread_cond_broadcast(&lift->change);
  
  while(!passenger_should_leave_lift(lift, to_floor)){
     pthread_cond_wait(&lift->change, &lift->mutex);
  }
  exit_lift(lift, id);

  //draw_lift(lift);
  pthread_cond_broadcast(&lift->change);

  if(travel_count<ITERATIONS){
    
    time_pos = travel_count;
    travel_count++;
  }else{
    while(1);
  }

  pthread_mutex_unlock(&lift->mutex);
  
  
  gettimeofday(&stoptime, NULL);
  timediff = (stoptime.tv_sec*1000000ULL + stoptime.tv_usec) -
    (starttime.tv_sec*1000000ULL + starttime.tv_usec);
  travel_times[time_pos] = timediff;
  
  if(time_pos == ITERATIONS - 1){
    do_math();
    lift_delete(lift);
    exit(0);
  }
}


/* --- functions related to person task END --- */
