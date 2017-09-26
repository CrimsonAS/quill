INCLUDES := -I../include
CFLAGS   := --std=c++11 -O2
DEFINES  :=
LIBS     :=
CC		 := g++

all: hello triangle strokelength mono_vs_lerp



hello: hello.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(DEFINES) hello.cpp $(LIBS) -o hello

triangle: triangle.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(DEFINES) triangle.cpp $(LIBS) -o triangle

strokelength: strokelength.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(DEFINES) strokelength.cpp $(LIBS) -o strokelength

mono_vs_lerp: mono_vs_lerp.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(DEFINES) -DNDEBUG mono_vs_lerp.cpp $(LIBS) -o mono_vs_lerp



clean:
	rm hello
	rm triangle
	rm strokelength
	rm mono_vs_lerp
