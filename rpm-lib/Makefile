.PHONY: all clean install uninstall version dirs targz

NAME=librpmtest
VER=0.1
REL=1

all:dirs targz
	./spec.py $(NAME) $(VER) $(REL)
	mv $(NAME)-*.tar.gz rpmbuild/SOURCES
	rpmbuild -bb -D "_topdir ${PWD}/rpmbuild" ./rpmbuild/SPECS/$(NAME).spec

dirs:
	mkdir -p rpmbuild/{BUILD,BUILDROOT,RPMS,SOURCES,SPECS,SRPMS}

targz:
	mkdir -p $(NAME)-$(VER)
	cp src/{*.h,*.cpp,Makefile,CMakeLists.txt,LICENSE} $(NAME)-$(VER)
	tar cfz $(NAME)-$(VER).tar.gz $(NAME)-$(VER)
	-$(RM) -rf $(NAME)-$(VER)

clean:
	-$(RM) -rf rpmbuild *.tar.gz $(NAME)-$(VER)*
	(cd src && make clean)

install:
	sudo dnf install rpmbuild/RPMS/x86_64/$(NAME)-$(VER)-$(REL).el8.x86_64.rpm

uninstall:
	sudo dnf remove $(NAME)