qbench: qbench.cpp runner.hpp timer.hpp
	g++ -std=c++11 -O2 -o qbench qbench.cpp

.PHONY: clean
clean:
	rm ./qbench