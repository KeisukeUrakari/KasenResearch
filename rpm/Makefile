.PHONY: all clean install uninstall version dirs targz

NAME=cello
REL=1

all:version dirs targz
	./spec.py $(REL)
	mv $(NAME)-*.tar.gz rpmbuild/SOURCES
	rpmbuild -D "_topdir ${PWD}/rpmbuild" -bb ./rpmbuild/SPECS/$(NAME).spec

version:
	$(eval VER := $(shell ./version.py))

dirs:
	mkdir -p rpmbuild/{BUILD,BUILDROOT,RPMS,SOURCES,SPECS,SRPMS}

targz:
	mkdir -p $(NAME)-$(VER)
	cp src/*.c src/Makefile src/LICENSE $(NAME)-$(VER)
	tar cfz $(NAME)-$(VER).tar.gz $(NAME)-$(VER)
	-$(RM) -rf $(NAME)-$(VER)

clean:
	-$(RM) -rf rpmbuild *.tar.gz $(NAME)-$(VER)*

install:version
	sudo dnf install rpmbuild/RPMS/x86_64/$(NAME)-$(VER)-$(REL).el8.x86_64.rpm

uninstall:
	sudo dnf remove $(NAME)