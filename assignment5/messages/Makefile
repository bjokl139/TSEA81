CFLAGS=-O2 -g -Wall
LIBS=-lrt -lpthread
CFILES=lift.c main.c si_ui.c si_comm.c draw.c messages.c
CFILES_MULTI = lift_multi.c main_multi.c si_ui.c si_comm.c draw.c messages.c
HEADERS=lift.h si_ui.h si_comm.h draw.h messages.h
HEADERS_MULTI=lift.h si_ui.h si_comm.h draw.h messages.h
lift_messages: $(CFILES) $(HEADERS)
	gcc $(CFLAGS) $(CFILES) -o lift_messages $(LIBS) 
multi: $(CFILES_MULTI) $(HEADERS_MULTI)
	gcc $(CFLAGS) $(CFILES_MULTI) -o lift_messages_multi $(LIBS) 
clean:
	rm -f lift_messages lift_messages_multi *~ *.o
