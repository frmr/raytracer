raytracer: ./src/*.cpp ./src/EasyBMP/*.cpp
	g++ -std=c++11 -o bin/raytracer *.cpp ./src/EasyBMP/*.cpp