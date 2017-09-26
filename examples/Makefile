INCLUDES := -I../include
LIBS     := 
CC		 := g++

all: hello triangle strokelength

hello: hello.cpp
	$(CC) $(INCLUDES) --std=c++11 -O2 hello.cpp $(LIBS) -o hello

triangle: triangle.cpp
	$(CC) $(INCLUDES) --std=c++11 -O2 triangle.cpp $(LIBS) -o triangle

strokelength: strokelength.cpp 
	$(CC) $(INCLUDES) --std=c++11 -O2 strokelength.cpp $(LIBS) -o strokelength

clean:
	rm hello
	rm triangle
