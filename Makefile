
all: clean
	@-mkdir -p build && cd build && cmake .. && make -q -j 2

clean:
	@-rm -rf build
