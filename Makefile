CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: walk 


walk: walk.cpp codyD.cpp tawficJ.cpp alejandroH.cpp drakeY.cpp
	g++ $(CFLAGS) walk.cpp codyD.cpp tawficJ.cpp alejandroH.cpp drakeY.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -owalk
	g++ lab3sget.cpp -Wall -olab3sget -lssl -lcrypto

clean:
	rm -f codyD
	rm -f tawficJ
	rm -f drakeY
	rm -f walk
	rm -f lab3sget
	rm -f *.o

