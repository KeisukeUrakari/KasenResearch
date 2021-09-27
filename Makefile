.PHONY:all build clean run run-test run-leak-check doxygen pdf

TARGET=app

all:build

build:
	mkdir -p build
	(cd build && cmake .. && make -j4)
#	(cd build && cmake -G Ninja .. && ninja -j4)

run:build
	./build/src/$(TARGET)

run-test:build
	./build/lib/test/datediff-test
	./build/test/$(TARGET)-test

run-leak-check:build
	valgrind --leak-check=full ./build/lib/test/datediff-test
	valgrind --leak-check=full ./build/test/$(TARGET)-test

doxygen:
	(cd doxygen && make)

pdf:doxygen
	(cd doxygen && make pdf)

clean:
	-$(RM) -rf build
	(cd doxygen && make clean)