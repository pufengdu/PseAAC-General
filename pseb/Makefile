EXT_HEADER=../../lua/install/include
EXT_LIB=../../lua/install/lib
CC=g++ -I$(EXT_HEADER) -O3 -s -c
LD=g++ -L$(EXT_LIB)
# LIBS=../lib/liblua.a

OBJECTS=bems.o biofiles.o error.o fasta.o luavm.o parameters.o pcprop.o pseaac.o pseb.o tools.o

pseb: $(OBJECTS)
	$(LD) -o pseb $(OBJECTS) -dn -lz -lm -dy -llua -ldl
bems.o: bems.cpp bems.h
	$(CC) bems.cpp 
biofiles.o: biofiles.cpp biofiles.h
	$(CC) biofiles.cpp
error.o: error.cpp error.h
	$(CC) error.cpp 
fasta.o: fasta.cpp fasta.h
	$(CC) fasta.cpp
luavm.o: luavm.cpp luavm.h
	$(CC) luavm.cpp
parameters.o: parameters.cpp parameters.h
	$(CC) parameters.cpp
pcprop.o: pcprop.cpp pcprop.h
	$(CC) pcprop.cpp
pseaac.o: pseaac.cpp pseaac.h
	$(CC) pseaac.cpp
pseb.o: pseb.cpp pseb.h
	$(CC) pseb.cpp
tools.o: tools.cpp tools.h
	$(CC) tools.cpp

.PHONY:clean

clean:
	@rm -f $(OBJECTS)
	@rm -f pseb
	@rm -f pseb3

.PHONY:static

static: $(OBJECTS)
	$(LD) -o pseb3 $(OBJECTS) $(LIBS) -lz -lm -ldl --static
