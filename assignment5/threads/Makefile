'CFLAGS=-O2 -g -Wall
LIBS=-lrt -lpthread -lm
CFILES=lift.c main.c debug.c
CFILES_MANY=lift_cv_floor.c main_cv_floor.c debug.c
HEADERS=lift.h debug.h
HEADERS_MANY=lift_cv_floor.h debug.h

one_cv: $(CFILES) $(HEADERS)
	gcc $(CFLAGS) $(CFILES) -D MAX_N_PERSONS=10 -D ITERATIONS=10 -o lift_pthreads_one_cv $(LIBS)
one_cv_auto: $(CFILES) $(HEADERS)
	gcc $(CFLAGS) $(CFILES) -D MAX_N_PERSONS=${MAX_N_PERSONS} -D ITERATIONS=${ITERATIONS} -o lift_pthreads_one_cv $(LIBS)
many_cv: $(CFILES_MANY) $(HEADERS_MANY)
	gcc $(CFLAGS_MANY) $(CFILES_MANY) -D MAX_N_PERSONS=10 -D ITERATIONS=10 -o lift_pthreads_many_cv $(LIBS) 
many_cv_auto: $(CFILES_MANY) $(HEADERS_MANY)
	gcc $(CFLAGS_MANY) $(CFILES_MANY) -D MAX_N_PERSONS=${MAX_N_PERSONS} -D ITERATIONS=${ITERATIONS} -o lift_pthreads_many_cv $(LIBS) 
clean:
	rm -f lift_pthreads lift_pthreads_cv *~ *.o
