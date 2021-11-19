.PHONY:all clean build install

VER=0.1

all:build

build:
	mkdir -p build
	(cd build && cmake .. && cmake --build .)

clean:
	-$(RM) -rf build

install:
	mkdir -p $(DESTDIR)/usr/lib64

	install -m 0644 build/librpmtest.$(VER).so $(DESTDIR)/usr/lib64
	(cd $(DESTDIR)/usr/lib64 && ln -s librpmtest.$(VER).so librpmtest.so)

	mkdir -p $(DESTDIR)/usr/include
	install -m 0644 rpm-test.h $(DESTDIR)/usr/include
