raytracer: ./src/*.cpp ./src/EasyBMP/*.cpp
	g++ -std=c++11 -o bin/raytracer src/*.cpp src/EasyBMP/*.cpp -lpthread
