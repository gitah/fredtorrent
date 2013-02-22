test: bin/bencode.o
	g++ -I./lib -c tests/bencode_test.cpp -o bin/bencode_test.o
	g++ bin/bencode_test.o bin/bencode.o -o ./a.out
	./a.out

bin/bencode.o: lib/bencode.cpp lib/bencode.h | bin
	g++ -c lib/bencode.cpp -o bin/bencode.o

bin:
	mkdir ./bin

clean:
	rm -f ./bin/*
	rmdir ./bin
