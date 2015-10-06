src=./src
bin=./bin

ALL:	${bin}/randgen ${bin}/markline ${bin}/linearsolver ${bin}/convergecheck ${bin}/existencecheck ${bin}/svm2plane  ${bin}/rand2input 
.PHONY:	clean

${bin}/rand2input:	${src}/rand2input.cpp
	g++ $< -o $@

${bin}/randgen:	${src}/randgen.cpp
	g++ $< -o $@

${bin}/markline:	${src}/markline.cpp
	g++ $< -o $@

${bin}/linearsolver:	${src}/linearsolver.cpp
	g++ $< -o $@

$r${bin}/convergecheck:	${src}/convergecheck.cpp
#	g++ -DBOUND=0.0001 $< -o $@
	g++ $< -o $@

${bin}/existencecheck:	${src}/existencecheck.cpp
	g++ $< -o $@

${bin}/svm2plane:	${src}/svm2plane.cpp
	g++ $< -o $@

clean:
	rm ${bin}/* 
