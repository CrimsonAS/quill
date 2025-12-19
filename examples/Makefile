INCLUDES := -I../include
CFLAGS   := --std=c++20 -O2 -Wall
DEFINES  :=
LIBS     :=
CC		 := g++



all: hello triangle strokelength mono_vs_lerp joinstyles rectclip triangles_in_line slant point_culling



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

rectclip: rectclip.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(DEFINES) rectclip.cpp $(LIBS) -o rectclip

triangles_in_line: triangles_in_line.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(DEFINES) triangles_in_line.cpp $(LIBS) -o triangles_in_line

slant: slant.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(DEFINES) slant.cpp $(LIBS) -o slant

point_culling: point_culling.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(DEFINES) point_culling.cpp $(LIBS) -o point_culling

clean:
	@rm -f hello
	@rm -f triangle
	@rm -f strokelength
	@rm -f mono_vs_lerp
	@rm -f joinstyles
	@rm -f rectclip
	@rm -f triangles_in_line
	@rm -f slant
	@rm -f point_culling
