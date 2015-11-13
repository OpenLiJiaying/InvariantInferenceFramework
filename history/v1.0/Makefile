src=./src
bin=./bin
CXX=g++

ALL:	${bin}/randgen ${bin}/markline ${bin}/linearsolver ${bin}/convergecheck ${bin}/existencecheck ${bin}/svm2plane  ${bin}/rand2input ${bin}/adjustcoef
	
${bin}/rand2input:	${src}/rand2input.cpp
	$(CXX) $< -o $@

${bin}/randgen:	${src}/randgen.cpp
	$(CXX) $< -o $@

${bin}/markline:	${src}/markline.cpp
	$(CXX) $< -o $@

${bin}/linearsolver:	${src}/linearsolver.cpp
	$(CXX) $< -o $@

$r${bin}/convergecheck:	${src}/convergecheck.cpp
#	g++ -DBOUND=0.0001 $< -o $@
	$(CXX) $< -o $@

${bin}/existencecheck:	${src}/existencecheck.cpp
	$(CXX) $< -o $@

${bin}/svm2plane:	${src}/svm2plane.cpp
	$(CXX) $< -o $@

${bin}/adjustcoef:	${src}/adjustcoef.cpp
	$(CXX) $< -o $@

.PHONY: clean
clean:
	-rm ${bin}/* 

