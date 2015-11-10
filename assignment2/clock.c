#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "si_ui.h"
#include "clock.h"

/* semaphores */

sem_t tick;

/* time data type */ 
typedef struct 
{
    int hours; 
    int minutes; 
    int seconds; 
} time_data_type;

/* clock data type */ 
typedef struct
{
    /* the current time */ 
    time_data_type time; 
    /* alarm time */ 
    time_data_type alarm_time; 
    /* alarm enabled flag */ 
    int alarm_enabled; 

    /* semaphore for mutual exclusion */ 
    pthread_mutex_t mutex; 

    /* semaphore for alarm activation */ 
    sem_t start_alarm; 

} clock_data_type; 

/* the actual clock */ 
static clock_data_type Clock; 

/* clock_init: initialise clock */ 
void clock_init(void)
{
    /* initialise time and alarm time */ 

    Clock.time.hours = 0; 
    Clock.time.minutes = 0; 
    Clock.time.seconds = 0; 

    Clock.alarm_time.hours = 0; 
    Clock.alarm_time.minutes = 0;
    Clock.alarm_time.seconds = 0; 
    
    /* alarm is not enabled */ 
    Clock.alarm_enabled = 0; 

    /* initialise semaphores */ 
    pthread_mutex_init(&Clock.mutex, NULL);
    sem_init(&Clock.start_alarm, 0, 0); 
}

/* clock_set_time: set current time to hours, minutes and seconds */ 
void clock_set_time(int hours, int minutes, int seconds)
{
    pthread_mutex_lock(&Clock.mutex); 

    Clock.time.hours = hours; 
    Clock.time.minutes = minutes; 
    Clock.time.seconds = seconds; 

    pthread_mutex_unlock(&Clock.mutex); 
}

void increment_time(){
  pthread_mutex_lock(&Clock.mutex);
  
  Clock.time.seconds++; 
  if (Clock.time.seconds > 59){
    Clock.time.seconds = 0; 
    Clock.time.minutes++; 
    if (Clock.time.minutes > 59)
      {
	Clock.time.minutes = 0; 
	Clock.time.hours++; 
	if (Clock.time.hours > 23)
	  {
	    Clock.time.hours = 0; 
	  }
      }
  }

  pthread_mutex_unlock(&Clock.mutex)
  
}

void check_alarm(){
  if (Clock.time.seconds == clock.alarm_time.seconds && 
      Clock.time.minutes == Clock.alarm_time.minutes && 
      Clock.time.hours == Clock.alarm_time.hours){
    sem_post(&Clock.start_alarm);
  }
}

void ui_thread(){
  /* message array */ 
    char message[SI_UI_MAX_MESSAGE_SIZE];
    si_ui_set_size(400, 200);
    
    /* time read from user interface */ 
    int hours, minutes, seconds; 
    
    while(1){
      si_ui_recieve(message);

      if(strncmp(message, "set", 3) == 0){
	//set %d %d %d
      }
      else if(strncmp(message, "alarm", 5) == 0){
	//alarm %d %d %d
      }
      else if(strncmp(message, "reset") == 0){
	//reset
      }
      else if(strncmp(message, "exit") == 0){
	exit(0);
      }
    }
}

void update_alarm_thread(){ 
}

void timekeeper_thread(){
  while(1){
    usleep(1000000); //1Hz
    sem_post(&tick);
  }
}



int main(void){

  si_ui_init();

  clock_init();
  
  sem_init(&tick,0,0);

  /* tasks */

}
