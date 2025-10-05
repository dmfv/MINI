CC      = gcc
CXX     = g++
CFLAGS  = -Wall -Wextra -g -I./
CXXFLAGS= -Wall -Wextra -g -I3rdParty/googletest/googletest -I3rdParty/googletest/googletest/include -I./
GTEST   = 3rdParty/googletest/googletest

# output dirs
BUILD   = build
BIN     = bin

# targets
PROJECT = $(BIN)/lang
TESTBIN = $(BIN)/tests

# sources
PROJ_SRC = lang.c
TEST_SRC = tests/parser/math.cpp
GTEST_SRC= $(GTEST)/src/gtest-all.cc $(GTEST)/src/gtest_main.cc

all: $(PROJECT)

$(PROJECT): $(PROJ_SRC) | $(BIN)
	$(CC) $(CFLAGS) -o $@ $^

# build tests
test: $(TESTBIN)

$(TESTBIN): $(TEST_SRC) $(GTEST_SRC) | $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $^

# run tests
runtest: test
	./$(TESTBIN)

$(BIN):
	mkdir -p $(BIN)

clean:
	rm -rf $(BIN) $(BUILD)
