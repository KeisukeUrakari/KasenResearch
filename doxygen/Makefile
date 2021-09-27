.PHONY: all clean pdf doxygen

all:doxygen

doxygen:
	doxygen

pdf:doxygen
	sed -f latex.sed -i latex/refman.tex
	(cd latex && make && xdvipdfmx refman.dvi)

clean:
	-$(RM) -fr latex html *.log inline_umlgraph_cache_all.pu
