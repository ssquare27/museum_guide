LIBS=`pkg-config gstreamer-0.10 --libs`
CFLAGS=`pkg-config gstreamer-0.10 --cflags`

all:
	g++ -I -Wall -o server Server.cpp -lpthread
	g++ -I -Wall -o client Client.cpp $(pkg-config --cflags --libs gstreamer-1.0)
clean:
	rm -f server
	rm -f client
