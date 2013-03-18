test_metainfo: bin/metainfo.o bin/bencode.o
	g++ -I./lib -g -c tests/metainfo_test.cpp -o bin/metainfo_test.o
	g++ bin/metainfo_test.o bin/metainfo.o bin/bencode.o -o ./a.out
	./a.out

test_bencode: bin/bencode.o
	g++ -I./lib -c tests/bencode_test.cpp -o bin/bencode_test.o
	g++ bin/bencode_test.o bin/bencode.o -o ./a.out
	./a.out

bin/metainfo.o: | bin
	g++ -g -c lib/metainfo.cpp -o bin/metainfo.o

bin/bencode.o: lib/bencode.cpp lib/bencode.h | bin
	g++ -g -c lib/bencode.cpp -o bin/bencode.o

bin:
	mkdir ./bin

clean:
	rm -f ./bin/*
	rmdir ./bin
