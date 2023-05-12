CPP-INSTRUMENTED=afl-clang-fast++
CPP=clang++
CPPFLAGS=-Wall -Wextra -Werror -std=c++20 -O3

.PHONY : clean

all: target/debug/main-instrumented target/debug/main-uninstrumented

target/debug/main-instrumented: main.cxx
	$(CPP-INSTRUMENTED) $(CPPFLAGS) -o target/debug/main-instrumented main.cxx

target/debug/main-uninstrumented: main.cxx
	$(CPP) $(CPPFLAGS) -o target/debug/main-uninstrumented main.cxx

clean:
	rm -rf target/debug/*
