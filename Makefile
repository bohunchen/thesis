CC = g++
Cflag = -Wall
OBJS = Main.o Topology.o EdgeNode.o Link.o VNF.o EdgeMap.o

all:	Main

Main:	$(OBJS)
	$(CC) $(Cflag) -o Main $^

Main.o: Main.cpp #Topology.h VNF.h EdgeNode.h
	$(CC) $(Cflag) -c Main.cpp

EdgeMap.o: EdgeMap.h Tenant.h
	$(CC) $(Cflag) -c EdgeMap.cpp

Topology.o: Topology.cpp Topology.h Link.h EdgeNode.h
	$(CC) $(Cflag) -c Topology.cpp

Link.o: EdgeNode.h Link.cpp Link.h
	$(CC) $(Cflag) -c Link.cpp

EdgeNode.o: EdgeNode.cpp EdgeNode.h VNF.h
	$(CC) $(Cflag) -c EdgeNode.cpp

VNF.o: VNF.h
	$(CC) $(Cflag) -c VNF.cpp



dep:
	$(CC) $(Cflag) -M *.cpp > dependenc.txt
clean:
	rm -f Main *.o *.~ new_* dependenc.txt
