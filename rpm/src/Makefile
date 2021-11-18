all:cello

cello:
	gcc -g -o cello cello.c
clean:
	-rm -rf cello *.o

install:
	mkdir -p $(DESTDIR)/usr/bin
	install -m 0755 cello $(DESTDIR)/usr/bin/cello
