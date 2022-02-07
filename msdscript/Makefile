INCS = main.hpp expr.hpp parse.hpp
OBJS = main.o expr.o parse.o 
msdscript: $(OBJS)
		c++ --std=c++14 -O2 -o msdscript $(OBJS)

main.o: main.cpp $(INCS)
		c++ --std=c++14 -O2 -c main.cpp

expr.o: expr.cpp $(INCS)
		c++ --std=c++14 -O2 -c expr.cpp

parse.o: parse.cpp $(INCS)
		c++ --std=c++14 -O2 -c parse.cpp



.PHONY : test
test: msdscript
		./msdscript --test