INCS = main.hpp expr.hpp
OBJS = main.o expr.o
msdscript: $(OBJS)
		c++ --std=c++14 -O2 -o msdscript $(OBJS)

main.o: main.cpp $(INCS)
		c++ --std=c++14 -O2 -c main.cpp

expr.o:  expr.cpp $(INCS)
		c++ --std=c++14 -O2 -c expr.cpp

.PHONY : test
test: msdscript
		./msdscript --test