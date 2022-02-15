INCS = main.hpp expr.hpp parse.hpp exec.h
OBJS = main.o expr.o parse.o
OBJS1 = exec.o run_msdscript.o
msdscript: $(OBJS)
		c++ --std=c++14 -O2 -o msdscript $(OBJS)

test_msdscript: $(OBJS1)
		c++ --std=c++14 -O2 -o run_msdscript $(OBJS1)

main.o: main.cpp $(INCS)
		c++ --std=c++14 -O2 -c main.cpp

expr.o: expr.cpp $(INCS)
		c++ --std=c++14 -O2 -c expr.cpp

parse.o: parse.cpp $(INCS)
		c++ --std=c++14 -O2 -c parse.cpp

exec.o: exec.cpp $(INCS)
		c++ --std=c++14 -O2 -c exec.cpp

run_msdscript.o: run_msdscript.cpp $(INCS)
		c++ --std=c++14 -O2 -c run_msdscript.cpp

.PHONY : test
test: msdscript
		./msdscript --test