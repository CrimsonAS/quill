INCLUDES := -I../include
CFLAGS   := --std=c++11 -O2 -Wall
DEFINES  :=
LIBS     :=
CC		 := g++



all: hello triangle strokelength mono_vs_lerp joinstyles



hello: hello.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(DEFINES) hello.cpp $(LIBS) -o hello

triangle: triangle.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(DEFINES) triangle.cpp $(LIBS) -o triangle

strokelength: strokelength.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(DEFINES) strokelength.cpp $(LIBS) -o strokelength

mono_vs_lerp: mono_vs_lerp.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(DEFINES) -DNDEBUG mono_vs_lerp.cpp $(LIBS) -o mono_vs_lerp

joinstyles: joinstyles.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(DEFINES) joinstyles.cpp $(LIBS) -o joinstyles



clean:
	@rm -f hello
	@rm -f triangle
	@rm -f strokelength
	@rm -f mono_vs_lerp
	@rm -f joinstyles
