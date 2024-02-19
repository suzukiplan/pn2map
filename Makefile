all: pn2tmx

test: pn2tmx
	./pn2tmx -d 16 -o 5 -s 555 -w 256 -h 256 -x 48 -y 48 > map.tmx
	tiled map.tmx

pn2tmx: pn2tmx.cpp
	gcc -c vgs0math.c
	g++ --std=c++17 -o pn2tmx pn2tmx.cpp vgs0math.o

clean:
	rm -f pn2tmx
