# CC and CFLAGS are varilables
CC = g++ 
CFLAGS = -c
OPTFLAGS = -O2
DBGFLAGS = -g -D_DEBUG_ON_
INCLUDE = -I /usr/local/include/Eigen
# make all
all	: bin/partition
	@echo -n "make complete!"

# optimized version
bin/partition: main_opt.o FM.o partition.o module.o Kraftwerk2.o LA.o Neighborhood.o Annealing.o Legalization.o place_terminal.o
	$(CC) $(OPTFLAGS) main_opt.o FM.o partition.o module.o Kraftwerk2.o LA.o Neighborhood.o Annealing.o Legalization.o place_terminal.o -o bin/partition
main_opt.o: src/main.cpp src/FM_alg.h src/partition.h src/module.h src/Neighborhood.h src/Kraftwerk2.h src/Annealing.h src/Legalization.h src/place_terminal.h
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@ 

FM.o: src/FM_alg.cpp src/FM_alg.h src/extern.h
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@
partition.o: src/partition.cpp src/partition.h src/extern.h src/module.h
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@
Kraftwerk2.o: src/Kraftwerk2.cpp src/Kraftwerk2.h src/extern.h src/module.h src/LA.h
	$(CC) $(CFLAGS) $(INCLUDE) $(OPTFLAGS) $< -o $@
LA.o: src/LA.cpp src/LA.h
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@
module.o: src/module.cpp src/module.h
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@
Neighborhood.o: src/Neighborhood.cpp src/Neighborhood.h
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@
Annealing.o: src/Annealing.cpp src/Annealing.h
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@
Legalization.o: src/Legalization.cpp src/Legalization.h
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@
place_terminal.o: src/place_terminal.cpp src/place_terminal.h
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

# clean all the .o and executable files
clean:
	rm -rf *.o lib/*.a lib/*.o bin/*