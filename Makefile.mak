#CXX = g++
#CXXFLAGS = -std=c++11 -Wall
#
#SRC = main.cpp vector.cpp
#OBJS = $(SRC:.cpp=.o)
#
#TARGET = myprogram
#
#all: $(TARGET)
#
#$(TARGET): $(OBJS)
#	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)
#
#%.o: %.cpp
#	$(CXX) $(CXXFLAGS) -c $< -o $@
#
#clean:
#	rm -f $(OBJS) $(TARGET)
#
#.PHONY: all clean

CXX = g++
CXXFLAGS = -std=c++11 -Wall

SRC = main.cpp vector.cpp Ray.cpp Camera.cpp color.cpp Sphere.cpp World.cpp
TARGET = a

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean