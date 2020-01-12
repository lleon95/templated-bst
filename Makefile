CXX = g++
CXXFLAGS = -Wall -Wextra -g -std=c++17 -O3

SRC = $(wildcard *.[ch]pp src/*.[ch]pp)

EXE = $(SRC:.cpp=.x)

# eliminate default suffixes
.SUFFIXES:
SUFFIXES =

# just consider our own suffixes
.SUFFIXES: .cpp .x

all: $(EXE)

.PHONY: all

%.x: %.cpp
	$(CXX) $< -o $@ $(CXXFLAGS)

format: $(SRC)
	@clang-format -i $^ 2>/dev/null || echo "Please install clang-format to run this command"

.PHONY: format

mem-check:
	valgrind --leak-check=yes ./main.x

clean:
	rm -f $(EXE) *~

.PHONY: clean
