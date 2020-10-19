run: src/main.cpp
	g++ -o qrune src/main.cpp -std=c++17 -Wall -lssl -lcrypto -lstdc++fs
	./qrune add Makefile

build:
	g++ -c src/main.cpp -std=c++17 -Wall -lstdc++fs
	g++ -c src/blob.cpp -std=c++17 -Wall -lstdc++fs
	g++ -c src/file_io.cpp -std=c++17 -Wall -lstdc++fs
	g++ -c src/zlib.cpp -std=c++17 -Wall -lz
	g++ -c src/qrune.cpp -std=c++17 -Wall -lstdc++fs
	g++ -c src/sha.cpp -std=c++17 -Wall -lssl -lcrypto
	@g++ -o qrune blob.o qrune.o zlib.o file_io.o sha.o main.o -std=c++17 -Wall -lssl -lcrypto -lstdc++fs -lz
	@rm -rf *.o

clean:
	rm -rf *.o

watch:
	find .qrune/objects -type f | xargs cat
