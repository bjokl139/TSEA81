'CFLAGS=-O2 -g -Wall
LIBS=-lrt -lpthread -lm
CFILES=lift.c main.c si_ui.c si_comm.c draw.c debug.c
CFILES_CV=lift_cv_floor.c main_cv_floor.c si_ui.c si_comm.c draw.c debug.c
HEADERS=lift.h si_ui.h si_comm.h draw.h debug.h
HEADERS_CV=lift_cv_floor.h si_ui.h si_comm.h draw.h debug.h

lift_pthreads: $(CFILES) $(HEADERS)
	gcc $(CFLAGS) $(CFILES) -o lift_pthreads $(LIBS)
cv: $(CFILES_CV) $(HEADERS_CV)
	gcc $(CFLAGS) $(CFILES_CV) -o lift_pthreads_cv $(LIBS) 
clean:
	rm -f lift_pthreads lift_pthreads_cv *~ *.o
