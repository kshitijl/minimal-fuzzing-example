CPP=afl-clang-fast++
CPPFLAGS=-Wall -Wextra -Werror -std=c++20 -O3

target/debug/main: main.cxx
	$(CPP) $(CPPFLAGS) -o target/debug/main main.cxx
