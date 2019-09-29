CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: walk 


walk: walk.cpp codyD.cpp tawficJ.cpp alejandroH.cpp drakeY.cpp
	g++ $(CFLAGS) walk.cpp codyD.cpp tawficJ.cpp alejandroH.cpp drakeY.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -owalk


clean:
	rm -f drakeY
	rm -f walk
	rm -f *.o

