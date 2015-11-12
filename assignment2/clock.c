#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "si_ui.h"
#include "si_comm.h"
#include "clock.h"
#include "display.h"

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

int show_alarm_message = 0;

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

void alarm_set_time(int hours, int minutes, int seconds)
{
    pthread_mutex_lock(&Clock.mutex); 

    Clock.alarm_time.hours = hours; 
    Clock.alarm_time.minutes = minutes; 
    Clock.alarm_time.seconds = seconds; 
    Clock.alarm_enabled = 1;
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
  
  pthread_mutex_unlock(&Clock.mutex);
}

/* time_ok: returns nonzero if hours, minutes and seconds represents a valid time */ 
int time_ok(int hours, int minutes, int seconds)
{
    return hours >= 0 && hours <= 23 && minutes >= 0 && minutes <= 59 && 
        seconds >= 0 && seconds <= 59; 
}

void* message_thread(void* arg){
  /* message array */ 
  char message[SI_UI_MAX_MESSAGE_SIZE];
  int hours, minutes, seconds;
  
  while(1){
    si_ui_receive(message);
    
    if(strncmp(message, "set", 3) == 0){
      //set %d %d %d
      sscanf(message,"set %d %d %d", &hours, &minutes, &seconds); 
      if(time_ok(hours, minutes, seconds)){
	  clock_set_time(hours, minutes, seconds);
	}
	else{
	  si_ui_show_error("Illegal value for hours, minutes or seconds");
	}
    }
    else if(strncmp(message, "alarm", 5) == 0){
      //alarm %d %d %d
      sscanf(message,"alarm %d %d %d", &hours, &minutes, &seconds); 
      if(time_ok(hours, minutes, seconds)){
	  alarm_set_time(hours, minutes, seconds);
	  display_alarm_time(hours, minutes, seconds);
	}
	else{
	  si_ui_show_error("Illegal value for hours, minutes or seconds");
	}
    }
    else if(strcmp(message, "reset") == 0){
      pthread_mutex_lock(&Clock.mutex);
      Clock.alarm_enabled = 0;
      pthread_mutex_unlock(&Clock.mutex);
      erase_alarm_time();
      erase_alarm_text();
    }
    else if(strcmp(message, "exit") == 0){
      exit(0);
    }
  }
}

void* logic_thread(void* arg){
  time_data_type time, alarm_time;
  int alarm_enabled;
  while(1){
    sem_wait(&tick);
    
    increment_time();
    
    pthread_mutex_lock(&Clock.mutex);
    time = Clock.time;
    alarm_time = Clock.alarm_time;
    alarm_enabled = Clock.alarm_enabled;
    pthread_mutex_unlock(&Clock.mutex);
    
    display_time(time.hours, time.minutes, time.seconds);
    
    if(time.hours == alarm_time.hours &&
       time.minutes == alarm_time.minutes &&
       time.seconds == alarm_time.seconds &&
       alarm_enabled){
      sem_post(&Clock.start_alarm);
    }
  }
}

void* update_alarm_thread(void* arg){ 
  int alarm_enabled;
  while(1){
    sem_wait(&Clock.start_alarm);
    while(1){
      display_alarm_text();
      usleep(1500000);
      pthread_mutex_lock(&Clock.mutex);
      alarm_enabled = Clock.alarm_enabled;
      pthread_mutex_unlock(&Clock.mutex);
      
      if(!alarm_enabled){break;}
    }
  }
}


void* timekeeper_thread(void* arg){
    while(1){
    usleep(1000000);
    sem_post(&tick);  
    }
}

int main(void){

  si_ui_init();
  si_ui_set_size(400, 200);
  
  clock_init();
  display_init();
  
  sem_init(&tick,0,0);

  /* tasks */
  pthread_t timekeeper_thread_handle;
  pthread_t logic_thread_handle;
  pthread_t message_thread_handle;
  pthread_t update_alarm_thread_handle;

  pthread_create(&timekeeper_thread_handle, NULL, timekeeper_thread, 0);
  pthread_create(&logic_thread_handle, NULL, logic_thread, 0);
  pthread_create(&message_thread_handle, NULL, message_thread, 0);
  pthread_create(&update_alarm_thread_handle, NULL, update_alarm_thread, 0);

  pthread_join(timekeeper_thread_handle, NULL);
  pthread_join(logic_thread_handle, NULL);
  pthread_join(message_thread_handle, NULL);
  pthread_join(update_alarm_thread_handle, NULL);
  /* will never be here! */ 
  return 0;
}
