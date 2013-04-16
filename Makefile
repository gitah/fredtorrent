main: bin/btsession.o
	g++ bin/peerwire.o bin/piece.o bin/tracker.o bin/peer.o bin/metainfo.o bin/bencode.o bin/btsession.o -lcurl -o ./a.out
	#./a.out

test_peerwire: bin/peerwire.o
	g++ -I./lib -g -c tests/peerwire_test.cpp -o bin/peerwire_test.o
	g++ bin/peerwire.o bin/peerwire_test.o -o ./a.out
	./a.out

test_tracker: bin/tracker.o
	g++ -I./lib -g -c tests/tracker_test.cpp -o bin/tracker_test.o
	g++ bin/tracker.o bin/peer.o bin/metainfo.o bin/bencode.o bin/tracker_test.o -lcurl -o ./a.out
	./a.out

test_metainfo: bin/metainfo.o
	g++ -I./lib -g -c tests/metainfo_test.cpp -o bin/metainfo_test.o
	g++ bin/metainfo_test.o bin/metainfo.o bin/bencode.o -o ./a.out
	./a.out

test_bencode: bin/bencode.o
	g++ -I./lib -c tests/bencode_test.cpp -o bin/bencode_test.o
	g++ bin/bencode_test.o bin/bencode.o -o ./a.out
	./a.out

bin/btsession.o: lib/btsession.cpp bin/piece.o bin/peerwire.o bin/tracker.o bin/metainfo.o | bin
	g++ -g -c lib/btsession.cpp -o bin/btsession.o

bin/peerwire.o: lib/peerwire.cpp | bin
	g++ -g -c lib/peerwire.cpp -o bin/peerwire.o

bin/piece.o: lib/piece.cpp | bin
	g++ -g -c lib/piece.cpp -o bin/piece.o

bin/tracker.o: lib/tracker.cpp bin/peer.o bin/metainfo.o | bin
	g++ -g -c lib/tracker.cpp -o bin/tracker.o

bin/peer.o: lib/peer.cpp | bin
	g++ -g -c lib/peer.cpp -o bin/peer.o

bin/metainfo.o: lib/metainfo.cpp bin/bencode.o | bin
	g++ -g -c lib/metainfo.cpp -o bin/metainfo.o

bin/bencode.o: lib/bencode.cpp | bin
	g++ -g -c lib/bencode.cpp -o bin/bencode.o

bin:
	mkdir ./bin

clean:
	rm -f ./bin/*
	rmdir ./bin
