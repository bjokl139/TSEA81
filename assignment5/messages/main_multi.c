#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <unistd.h>
#include "lift.h"
#include "si_ui.h"
#include "messages.h"

#include "draw.h"

#define QUEUE_UI 0
#define QUEUE_LIFT 1
#define QUEUE_RESULT 2
#define QUEUE_FIRSTPERSON 10

#define NUMBER_MESSAGES 5

#define ITERATIONS 10
// These variables keeps track of the process IDs of all processes
// involved in the application so that they can be killed when the
// exit command is received.
static pid_t lift_pid;
static pid_t uidraw_pid;
static pid_t liftmove_pid;
static pid_t result_pid;
static pid_t person_pid[MAX_N_PERSONS];

long long int travel_time_array[MAX_N_PERSONS][ITERATIONS];

typedef enum {LIFT_TRAVEL, // A travel message is sent to the list process when a person would
	                   // like to make a lift travel
	      LIFT_TRAVEL_DONE, // A travel done message is sent to a person process when a
	                        // lift travel is finished
	      LIFT_MOVE,         // A move message is sent to the lift task when the lift shall move to the next floor
	      PROCESS_DONE,
	      WRITE_TO_FILE,
	      WRITE_DONE,

} lift_msg_type; 

struct lift_msg{
  lift_msg_type type;  // Type of message
  int person_id;       // Specifies the person
  int from_floor[NUMBER_MESSAGES];      // Specify source and destion for the LIFT_TRAVEL message.
  int to_floor[NUMBER_MESSAGES];
};

void write_to_file(){
  int i,j;
  FILE *f = fopen("one_cv.txt", "w");
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


// Since we use processes now and not 
static int get_random_value(int person_id, int maximum_value)
{
	return rand() % (maximum_value + 1);
}


// Initialize the random seeds used by the get_random_value() function
// above.
static void init_random(void)
{
	srand(getpid()); // The pid should be a good enough initialization for
                         // this case at least.
}


static void liftmove_process(void)
{
	struct lift_msg m;
	while(1){
		// TODO:
		//    Sleep 2 seconds
                //    Send a message to the lift process to move the lift.
	  usleep(2000000);
	  m.type = LIFT_MOVE;	
	  message_send(&m, sizeof(struct lift_msg), QUEUE_LIFT, 0);
	}
}


static void lift_process(void)
{
        lift_type Lift;
	Lift = lift_create();
	int change_direction, next_floor;
	int temp_id;
	int temp_to_floor;
	int to_floors_all[MAX_N_PERSONS][NUMBER_MESSAGES];
	int from_floors_all[MAX_N_PERSONS][NUMBER_MESSAGES];
	int person_message_iterator[MAX_N_PERSONS];
	char msgbuf[4096];
	while(1){
		int i;
		struct lift_msg reply;
		struct lift_msg *m;
		message_send((char *) Lift, sizeof(*Lift), QUEUE_UI,0); // Draw the lift
		int len = message_receive(msgbuf, 4096, QUEUE_LIFT); // Wait for a message
		if(len < sizeof(struct lift_msg)){
			fprintf(stderr, "Message too short\n");
			continue;
		}
		
		m = (struct lift_msg *) msgbuf;
		switch(m->type){
		case LIFT_MOVE:
		  
		  for (i = 0; i < MAX_N_PASSENGERS; i ++){
		    if (Lift->passengers_in_lift[i].id != NO_ID){
		      if(Lift->passengers_in_lift[i].to_floor == Lift->floor){
			temp_id = Lift->passengers_in_lift[i].id;
			Lift->passengers_in_lift[i].id = NO_ID;
			Lift->passengers_in_lift[i].to_floor = NO_FLOOR;
			
			if(person_message_iterator[temp_id] < NUMBER_MESSAGES){
			  enter_floor(Lift,temp_id,from_floors_all[temp_id][person_message_iterator[temp_id]],to_floors_all[temp_id][person_message_iterator[temp_id]]);
			  message_send((char *) Lift, sizeof(*Lift), QUEUE_UI,0); // Draw the lift
			  person_message_iterator[temp_id]++;
			}else{
			  
			  reply.type = LIFT_TRAVEL_DONE;
			  
			message_send((char *) Lift, sizeof(*Lift), QUEUE_UI,0); // Draw the lift
			message_send(&reply, sizeof(reply), QUEUE_FIRSTPERSON + temp_id, 0);
			}
		      }
		    }
		  }

		  /* Check if passengers want to enter the lift */
		  for (i = 0; i < MAX_N_PERSONS; i ++){
		      temp_id = Lift->persons_to_enter[Lift->floor][i].id;
		      temp_to_floor = Lift->persons_to_enter[Lift->floor][i].to_floor;
		      if ((temp_id != NO_ID) && n_passengers_in_lift(Lift) < MAX_N_PASSENGERS){
			leave_floor(Lift, temp_id, Lift->floor); 
			enter_lift(Lift, temp_id, temp_to_floor);
			message_send((char *) Lift, sizeof(*Lift), QUEUE_UI,0); // Draw the lift
		      }
		    }
		  
		  lift_next_floor(Lift, &next_floor, &change_direction);
		  lift_move(Lift, next_floor, change_direction);
		  break;
		case LIFT_TRAVEL:
                  
		  for(i = 0; i < NUMBER_MESSAGES; i++){
		    to_floors_all[m->person_id][i] = m->to_floor[i];
		    from_floors_all[m->person_id][i] = m->from_floor[i];
		  }
		  person_message_iterator[m->person_id] = 1;
		  enter_floor(Lift, m->person_id, m->from_floor[0], m->to_floor[0]);
		  message_send((char *) Lift, sizeof(*Lift), QUEUE_UI,0); // Draw the lift
		  
		  break;
		}
	}
	return;
}

static void person_process(int id)
{
	init_random();
	
	int start_floor;
	int destination_floor;
	 
	char buf[4096];
	struct lift_msg m;
	struct lift_msg* reply;
	int travel_count;
	for(travel_count = 0; travel_count < ITERATIONS; travel_count += NUMBER_MESSAGES){
	  /* Create a LIFT_TRAVEL message and send it */
	  m.type = LIFT_TRAVEL;
	  m.person_id = id;
	  int i;
	  for(i  = 0;i < NUMBER_MESSAGES; i++){
	    /* Generate ranom floors */
	    start_floor = get_random_value(id, N_FLOORS-1);
	    destination_floor = start_floor;
	    while(destination_floor == start_floor){
	      destination_floor = get_random_value(id, N_FLOORS-1);
	    }

	    m.from_floor[i] = start_floor;
	    m.to_floor[i] = destination_floor;
	  }
	  message_send((char *)&m, sizeof(struct lift_msg), QUEUE_LIFT, 0);
	  
	  /* Wait for LIFT_TRAVEL_DONE message*/
	  message_receive(buf, 4096, QUEUE_FIRSTPERSON + id);
	  
	  reply = (struct lift_msg *)buf;
	  //if(reply->type != LIFT_TRAVEL_DONE)
	  //lift_panic("Recieved unexpected message in person_process");
	  /* Wait a little while */
	  usleep(5000000);
	  
	}
}

// This is the final process called by main()
// It is responsible for:
//   * Receiving and executing commands from the java GUI
//   * Killing off all processes when exiting the application
void uicommand_process(void)
{
	int i;
	int current_person_id = 0;
	char message[SI_UI_MAX_MESSAGE_SIZE]; 
	while(1){
		// Read a message from the GUI
		si_ui_receive(message);
		if(!strcmp(message, "new")){
		  if(current_person_id < MAX_N_PERSONS){
		    
		    pid_t tmp_pid = fork();
		    if (tmp_pid == 0){
		      person_process(current_person_id);
		    } else{
		      person_pid[current_person_id] = tmp_pid;
		      current_person_id ++;
		    }
		  }
			// TODO:
			// * Check that we don't create too many persons
			// * fork and create a new person process (and
			// *  record the new pid in person_pid[])
		}else if(!strcmp(message,"test")){
		  int j;
		  for(j = 0; j<MAX_N_PERSONS; j++){
		   pid_t tmp_pid = fork();
		    if (tmp_pid == 0){
		      person_process(current_person_id);
		      exit(0);
		    } else{
		      person_pid[current_person_id] = tmp_pid;
		      current_person_id ++;
		    }
		  }
		}else if(!strcmp(message, "exit")){
		  // The code below sends the SIGINT signal to
		  // all processes involved in this application
		  // except for the uicommand process itself
		  // (which is exited by calling exit())
		  kill(uidraw_pid, SIGINT);
		  kill(lift_pid, SIGINT);
		  kill(liftmove_pid, SIGINT);
		  kill(result_pid,SIGINT);
		  for(i=0; i < MAX_N_PERSONS; i++){
		    if(person_pid[i] > 0){
		      kill(person_pid[i], SIGINT);
		    }
		  }
		  exit(0);
		}
	}
}

// This process is responsible for drawing the lift. Receives lift_type structures
// as messages.
void uidraw_process(void)
{
	char msg[1024];
	si_ui_set_size(670, 700); 
	while(1){
		message_receive(msg, 1024, QUEUE_UI);
		lift_type Lift = (lift_type) &msg[0];
		draw_lift(Lift);
	}
}

void result_process(void)
{
  int processes_done = 0;
  char buf[1024];
  struct lift_msg* msg;
  while(1){
    message_receive(buf, 1024, QUEUE_RESULT);
    msg = (struct lift_msg *)buf;
    if(msg->type == PROCESS_DONE){
      processes_done++;
    }
    if(processes_done == MAX_N_PERSONS){
      int i;
      for(i = 0; i < MAX_N_PERSONS; i++){
	msg->type = WRITE_TO_FILE;
	message_send((char *)msg, sizeof(struct lift_msg), QUEUE_FIRSTPERSON+i, 0);
	message_receive(buf, 1024, QUEUE_RESULT);
      }
      break;
    }
  }
  
}

int main(int argc, char **argv)
{
	message_init();
        si_ui_init(); // Initialize user interface. (Must be done
		      // here!)

	lift_pid = fork();
	if(!lift_pid) {
		lift_process();
	}
	uidraw_pid = fork();
	if(!uidraw_pid){
		uidraw_process();
	}
	liftmove_pid = fork();
	if(!liftmove_pid){
		liftmove_process();
	}
	result_pid = fork();
	if(!result_pid){
		result_process();
	}
	uicommand_process();

	return 0;
}
