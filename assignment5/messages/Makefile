CFLAGS=-O2 -g -Wall
LIBS=-lrt -lpthread
CFILES=lift.c main.c messages.c
CFILES_MULTI = lift_multi.c main_multi.c messages.c
HEADERS=lift.h messages.h
HEADERS_MULTI=lift.h messages.h
single: $(CFILES) $(HEADERS)
	gcc $(CFLAGS) $(CFILES) -D MAX_N_PERSONS=10 -D ITERATIONS=10 -o lift_messages_single $(LIBS) 
single_auto: $(CFILES) $(HEADERS)
	gcc $(CFLAGS) $(CFILES) -D MAX_N_PERSONS=${MAX_N_PERSONS} -D ITERATIONS=${ITERATIONS} -o lift_messages_single $(LIBS) 
multi: $(CFILES_MULTI) $(HEADERS_MULTI)
	gcc $(CFLAGS) $(CFILES_MULTI) -D MAX_N_PERSONS=10 -D ITERATIONS=10 -D NUMBER_MESSAGES=10 -o lift_messages_multi $(LIBS)
multi_auto: $(CFILES_MULTI) $(HEADERS_MULTI)
	gcc $(CFLAGS) $(CFILES_MULTI) -D MAX_N_PERSONS=${MAX_N_PERSONS} -D ITERATIONS=${ITERATIONS} -D NUMBER_MESSAGES=${NUMBER_MESSAGES} -o lift_messages_multi $(LIBS)	
clean:
	rm -f lift_messages lift_messages_multi *~ *.o
