CC=g++ -I../pseb -s -O3

bmv: bmv.o binmodview.o
	$(CC) -o bmv bmv.o binmodview.o -lz -lm -static
bmv.o: bmv.cpp bmv.h
	$(CC) -c bmv.cpp -o bmv.o
binmodview.o: binmodview.cpp binmodview.h
	$(CC) -c binmodview.cpp -o binmodview.o

.PHONY: clean
clean:
	@rm -f bmv *.o

