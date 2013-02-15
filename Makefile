CXX = g++
CFLAGS = -W -Wall -ansi
LDFLAGS = -lSDL -lGL -lGLEW
EXEC = 3DObs

all: $(EXEC)

$(EXEC): Application.o Object.o Renderer.o Camera.o ObjLoader.o Rig.o main.o
	$(CXX) -o $(EXEC) Application.o Object.o Renderer.o Camera.o ObjLoader.o Rig.o main.o $(CFLAGS) $(LDFLAGS)
	rm -rf *.o

ObjLoader.o: src/ObjLoader.cpp include/ObjLoader.hpp
	$(CXX) -c src/ObjLoader.cpp $(CFLAGS)

Camera.o: src/Camera.cpp include/Camera.hpp
	$(CXX) -c src/Camera.cpp $(CFLAGS)

Object.o: src/Object.cpp include/Object.hpp
	$(CXX) -c src/Object.cpp $(CFLAGS)

Renderer.o: src/Renderer.cpp include/Renderer.hpp
	$(CXX) -c src/Renderer.cpp $(CFLAGS)

Rig.o: src/Rig.cpp include/Rig.hpp
	$(CXX) -c src/Rig.cpp $(CFLAGS)

Application.o: src/Application.cpp include/Application.hpp
	$(CXX) -c src/Application.cpp $(CFLAGS)

main.o: src/main.cpp
	$(CXX) -c src/main.cpp $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
