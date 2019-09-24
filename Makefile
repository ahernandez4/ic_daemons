CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: walk 

walk: walk.cpp drakeY.cpp
	g++ $(CFLAGS) walk.cpp drakeY.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o walk drakeY

clean:
	rm -f drakeY
	rm -f walk
	rm -f *.o

