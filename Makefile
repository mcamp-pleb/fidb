CC	:= g++
LINK	:= -std=c++17

fidb: Main.o Entry.o IOData.o
	${CC} Entry.o IOData.o Main.o -o ./bin/fidb

debug: Main.o Entry.o IOData.o
	${CC} -g Entry.o IOData.o Main.o -o ./bin/fidb_d

Main.o: Main.cpp
	${CC} ${LINK} -c Main.cpp

Entry.o: Entry.hpp Entry.cpp
	${CC} -c Entry.cpp

IOData.o: IOData.hpp IOData.cpp
	${CC} ${LINK} -c IOData.cpp

clean:
	rm *.o ./bin/fidb
