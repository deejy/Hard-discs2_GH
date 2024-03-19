CC = g++
CFLAGS = -Wall
LIB_FLAGS = 
EXEC_NAME = config_test \
	polygon_test \
	topology_test
SRC = $(wildcard *.cpp ../Classes/*.cpp)
OBJ = $(SRC:.cpp=.o)

all : $(EXEC_NAME)

config_test : $(OBJ)
	$(CC) -o $@ config_test.o ../Classes/atom.o ../Classes/molecule.o ../Classes/topology.o ../Classes/object.o ../Classes/polygon.o ../Classes/config.o ../Classes/force_field.o  

polygon_test : $(OBJ)
	$(CC) -o $@ polygon_test.o ../Classes/atom.o  ../Classes/force_field.o  ../Classes/polygon.o 

topology_test : $(OBJ)
	$(CC) -o $@ topology_test.o ../Classes/atom.o ../Classes/molecule.o ../Classes/topology.o 

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean :
	rm -f $(EXEC_NAME) $(OBJ)  

