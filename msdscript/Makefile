INCS = main.hpp expr.hpp
OBJS = main.o
msdscript: $(OBJS)
		c++ --std=c++14 -O2 -o msdscript $(OBJS)
main.o: main.cpp $(INCS)
		c++ --std=c++14 -O2 -c main.cpp

.PHONY : test
test: msdscript
		./msdscript --test