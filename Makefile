CPP=g++
CPPFLAGS=-Wall -Wextra -Werror -std=c++20

target/debug/main: main.cxx
	$(CPP) $(CPPFLAGS) -o target/debug/main main.cxx
