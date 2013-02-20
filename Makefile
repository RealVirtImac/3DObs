CXX = g++
CFLAGS = -W -Wall -ansi
LDFLAGS = -lSDL -lGL -lGLEW
EXEC = 3DObs

all:	$(EXEC)
	

$(EXEC): bin/Application.o bin/Object.o bin/Renderer.o bin/Camera.o bin/ObjLoader.o bin/Rig.o bin/main.o
	@echo "\033[33;33m \t Linking \033[m\017" 
	@$(CXX) -o $(EXEC) bin/Application.o bin/Object.o bin/Renderer.o bin/Camera.o bin/ObjLoader.o bin/Rig.o bin/main.o $(CFLAGS) $(LDFLAGS)
	@echo "\033[33;34m \t Done : type : ./3DObs to run \033[m\017"

bin/Application.o: src/Application.cpp include/Application.hpp
	@mkdir -p bin
	@echo ""
	@echo "------------------- 3DObs --------------------"
	@echo "\033[33;32m \t Compiling" $< "\033[m\017" 
	@$(CXX) -c src/Application.cpp $(CFLAGS)
	@mv Application.o bin/

bin/ObjLoader.o: src/ObjLoader.cpp include/ObjLoader.hpp
	@echo "\033[33;32m \t Compiling" $< "\033[m\017" 
	@$(CXX) -c src/ObjLoader.cpp $(CFLAGS)
	@mv ObjLoader.o bin/

bin/Camera.o: src/Camera.cpp include/Camera.hpp
	@echo "\033[33;32m \t Compiling" $< "\033[m\017" 
	@$(CXX) -c src/Camera.cpp $(CFLAGS)
	@mv Camera.o bin/

bin/Object.o: src/Object.cpp include/Object.hpp
	@echo "\033[33;32m \t Compiling" $< "\033[m\017" 
	@$(CXX) -c src/Object.cpp $(CFLAGS)
	@mv Object.o bin/

bin/Renderer.o: src/Renderer.cpp include/Renderer.hpp
	@echo "\033[33;32m \t Compiling" $< "\033[m\017" 
	@$(CXX) -c src/Renderer.cpp $(CFLAGS)
	@mv Renderer.o bin/

bin/Rig.o: src/Rig.cpp include/Rig.hpp
	@echo "\033[33;32m \t Compiling" $< "\033[m\017" 
	@$(CXX) -c src/Rig.cpp $(CFLAGS)
	@mv Rig.o bin/

bin/main.o: src/main.cpp
	@echo "\033[33;32m \t Compiling" $< "\033[m\017" 
	@$(CXX) -c src/main.cpp $(CFLAGS)
	@mv main.o bin/

clean:
	@rm -rf bin/*.o
	@echo "\033[33;31m \t Cleaning '.o' \033[m\017" 

mrproper: clean
	@rm -rf $(EXEC)
	@echo "\033[33;31m \t Cleaning the executable \033[m\017" 
