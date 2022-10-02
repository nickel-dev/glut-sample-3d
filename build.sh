clear && rm bin/debug.o
gcc -c src/*.c -std=c11 -m64 -g -Wall -Iinclude/ -Ires/ -L/usr/lib/x86_64-linux-gnu -lm
gcc *.o -L/usr/lib/x86_64-linux-gnu -lm -o bin/debug.o -lGL -lGLU -lglut
rm *.o && ./bin/debug.o
