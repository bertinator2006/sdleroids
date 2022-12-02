#compile mac
src=src/asteroids.cpp
bin=bin/debug
flags=-lSDL2
testsrc=src/test.cpp
testbin=bin/test
testflags=$(flags)

mac:
	g++ $(src)

test:
	g++ $(testsrc) -o $(testbin) $(testflags)

runmac:
	./$(bin)