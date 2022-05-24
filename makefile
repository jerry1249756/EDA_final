# CC and CFLAGS are varilables
CC = g++
CFLAGS = -c
OPTFLAGS = -O2
# make all
all	: bin/partition
	@echo -n "make complete!"

# optimized version
bin/partition: main_opt.o FM.o partition.o
	$(CC) $(OPTFLAGS) main_opt.o FM.o partition.o -o bin/partition
main_opt.o: src/test_FM.cpp  src/FM_alg.h src/partition.h
	$(CC) $(CFLAGS) $< -o $@
FM.o: src/FM_alg.cpp src/FM_alg.h
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@
partition.o: src/partition.cpp src/partition.h
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

# clean all the .o and executable files
clean:
		rm -rf *.o lib/*.a lib/*.o bin/*