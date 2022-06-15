# CC and CFLAGS are varilables
CC = g++ 
CFLAGS = -c
OPTFLAGS = -O2
DBGFLAGS = -g -D_DEBUG_ON_
# make all
all	: bin/partition
	@echo -n "make complete!"

# optimized version
bin/partition: main_opt.o FM.o partition.o module.o Kraftwerk2.o LA.o Neighborhood.o
	$(CC) $(OPTFLAGS) main_opt.o FM.o partition.o module.o Kraftwerk2.o LA.o Neighborhood.o -o bin/partition
main_opt.o: src/main.cpp src/FM_alg.h src/partition.h src/module.h src/Neighborhood.h src/Kraftwerk2.h
	$(CC) $(CFLAGS) $< -o $@ 

FM.o: src/FM_alg.cpp src/FM_alg.h src/extern.h
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@
partition.o: src/partition.cpp src/partition.h src/extern.h src/module.h
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@
Kraftwerk2.o: src/Kraftwerk2.cpp src/Kraftwerk2.h src/extern.h src/module.h src/LA.h
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@
LA.o: src/LA.cpp src/LA.h
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@
module.o: src/module.cpp src/module.h
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@
Neighborhood.o: src/Neighborhood.cpp src/Neighborhood.h
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

# clean all the .o and executable files
clean:
	rm -rf *.o lib/*.a lib/*.o bin/*