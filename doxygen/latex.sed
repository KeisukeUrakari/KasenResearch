s/\\documentclass\[twoside\]{article}/\\documentclass\[twoside,dvipdfmx\]{article}/g
s/\\newunicodechar/%\\newunicodechar/g

2a\\\usepackage[table,dvipdfmx]{xcolor}

s/\\usepackage\[ps2pdf,pagebackref=true\]{hyperref}/\\usepackage\[dvipdfmx,pagebackref=true\]{hyperref}/g
s/\\begin{document}/\\usepackage{pxjahyper}\n\0/g

#2a\\\usepackage[deluxe,jis2004,uplatex]{otf}
#2a\\\renewcommand{\\kanjifamilydefault}{\\gtdefault}
#	(cd doc/latex && sed -i '2a\\\DeclareFontShape{JY2}{gt}{bc}{n}{<->ssub*hgt/bx/n}{}' refman.tex)
#	(cd doc/latex && sed -i '2a\\\DeclareFontShape{JT2}{gt}{bc}{n}{<->ssub*hgt/bx/n}{}' refman.tex)

/large 構築/d
/scriptsize 構築/d
