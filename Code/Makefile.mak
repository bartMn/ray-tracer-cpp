CXX = g++
CXXFLAGS = -std=c++11 -Wall

SRC = raytracer.cpp vector.cpp Ray.cpp Camera.cpp color.cpp Sphere.cpp world.cpp triangle.cpp cylinder.cpp circle.cpp Material.cpp tonemapping.cpp texture.cpp combine_ppms.cpp
TARGET = a

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
