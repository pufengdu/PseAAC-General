CC=g++ -w -s -O3
bmc: bmc.o listparser.o tools.o
	$(CC) -o bmc bmc.o listparser.o tools.o -lm -lz -static
bmc.o: bmc.cpp bmc.h
	$(CC) -c bmc.cpp -o bmc.o
listparser.o: listparser.cpp listparser.h
	$(CC) -c listparser.cpp -o listparser.o
tools.o: ../pseb/tools.cpp ../pseb/tools.h
	$(CC) -c ../pseb/tools.cpp -o tools.o

.PHONY: clean
clean:
	@rm -f bmc *.o

