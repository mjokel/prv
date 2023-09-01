# Variables
CC=clang++
CFLAGS=-std=c++17 -stdlib=libc++

# Default target
all: 
	$(CC) $(CFLAGS) main.cpp utils.cpp classes.cpp logger.cpp -llog4cxx -I/usr/local/include/log4cxx -L/usr/local/lib -o prvh
	

clean:
	rm -f *.o 


# old target: $(CC) $(CFLAGS) main.cpp utils.cpp classes.cpp -llog4cxx -I/usr/local/include/log4cxx -L/usr/local/lib -o prvh
# old target: $(CC) $(CFLAGS) main.cpp utils.cpp classes.cpp -o prvh
