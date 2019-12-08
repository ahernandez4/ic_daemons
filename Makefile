#g++ lab3sget.cpp -Wall -olab3sget -lssl -lcrypto -g

CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: walk 


walk: walk.cpp codyD.cpp tawficJ.cpp alejandroH.cpp drakeY.cpp lab3sget.cpp
	g++ $(CFLAGS) walk.cpp lab3sget.cpp codyD.cpp tawficJ.cpp alejandroH.cpp drakeY.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -owalk -lcrypto -lssl -DWORLDRAIN
	

clean:
	rm -f codyD
	rm -f tawficJ
	rm -f drakeY
	rm -f walk
	rm -f lab3sget
	rm -f *.o

