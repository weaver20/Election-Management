CC = gcc
OBJS = elections.o map.o voteMap.o electionTestsExample.o
EXEC = election
DEBUG_FLAG = -g	# now empty, assign -g for debug
COMP_FLAG = -std=c99 -Wall -Werror -pedantic-errors -DNDEBUG

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@

electionTestsExample.o: electionTestsExample.c election.h mtm_map/map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

elections.o : elections.c election.h mtm_map/map.h voteMap.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

voteMap.o : voteMap.c voteMap.h mtm_map/map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

map.o : mtm_map/map.c mtm_map/map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG)  mtm_map/$*.c

clean:
	rm -f $(OBJS) $(EXEC)



