test_tracker: bin/tracker.o
	g++ -I./lib -g -c tests/tracker_test.cpp -o bin/tracker_test.o
	g++ bin/tracker.o bin/peer.o bin/metainfo.o bin/bencode.o bin/tracker_test.o -lcurl -o ./a.out
	#./a.out

test_metainfo: bin/metainfo.o
	g++ -I./lib -g -c tests/metainfo_test.cpp -o bin/metainfo_test.o
	g++ bin/metainfo_test.o bin/metainfo.o bin/bencode.o -o ./a.out
	./a.out

test_bencode: bin/bencode.o
	g++ -I./lib -c tests/bencode_test.cpp -o bin/bencode_test.o
	g++ bin/bencode_test.o bin/bencode.o -o ./a.out
	./a.out

bin/btsession.o: lib/btsession.cpp bin/tracker.o bin/metainfo.o | bin
	g++ -g -c lib/btsession.cpp -o bin/tracker.o

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
