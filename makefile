# CC and CFLAGS are varilables
CC = g++ 
CFLAGS = -c
OPTFLAGS = -O2

DBGFLAGS = -g -D_DEBUG_ON_
# make all
all	: bin/partition
	@echo -n "make complete!"

# optimized version
bin/partition: main_opt.o module.o FM.o partition.o 
	$(CC) $(OPTFLAGS) main_opt.o FM.o partition.o module.o -o bin/partition
main_opt.o: src/main.cpp  src/FM_alg.h src/partition.h src/module.h
	$(CC) $(CFLAGS) $< -o $@

FM.o: src/FM_alg.cpp src/FM_alg.h
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@
partition.o: src/partition.cpp src/partition.h
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@
module.o: src/module.cpp src/module.h
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

dbg: bin/partition_dbg
	@echo -n "make complete! (debug version)"

# debug version
bin/partition_dbg: main_dbg.o module_dbg.o FM_dbg.o partition_dbg.o 
	$(CC) $(DBGFLAGS) main_dbg.o FM_dbg.o partition_dbg.o module_dbg.o -o bin/partition_dbg
main_dbg.o: src/main.cpp  src/FM_alg.h src/partition.h src/module.h
	$(CC) $(CFLAGS) $< -o $@

FM_dbg.o: src/FM_alg.cpp src/FM_alg.h
	$(CC) $(CFLAGS) $(DBGFLAGS) $< -o $@
partition_dbg.o: src/partition.cpp src/partition.h
	$(CC) $(CFLAGS) $(DBGFLAGS) $< -o $@
module_dbg.o: src/module.cpp src/module.h
	$(CC) $(CFLAGS) $(DBGFLAGS) $< -o $@


# clean all the .o and executable files
clean:
		rm -rf *.o lib/*.a lib/*.o bin/*