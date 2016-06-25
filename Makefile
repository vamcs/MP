# SYNOPSIS:
#
#   make [all]  - makes everything.
#   make TARGET - makes the given target.
#   make clean  - removes all files generated by make.


EXEC = MP_Final

GTEST_DIR = googletest

USER_DIR = 

OBJDIR = obj

INCL = -I/$(INCLUDE_DIR)/
INCLUDE_DIR = include
TEST_DIR = tests
SRC_DIR = src

CPPFLAGS += -isystem $(GTEST_DIR)/include

CXXFLAGS += -g -Wall -Wextra

TESTS = Digraph_Test

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h


all : $(TESTS) $(EXEC)

clean :
	rm -f $(TESTS) gtest.a gtest_main.a *.o

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)


gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

CFLAGS = -g -Wall

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lncurses -ftest-coverage -fprofile-arcs

$(OBJDIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCL) -lncurses -c $< -o $@ -ftest-coverage -fprofile-arcs

SRCSTEST = $(wildcard $(TEST_DIR)/*.cpp)
OBJSTEST = $(patsubst $(TEST_DIR)/%.cpp,test_obj/%.o, $(SRCSTEST))


TESTE: ./obj/Digraph.o ./obj/manager.o ./obj/GUI.o ./obj/fileReader.o $(OBJSTEST) gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -std=c++0x -ftest-coverage -fprofile-arcs -lncurses

test_obj/%.o: $(TEST_DIR)/%.cpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ -std=c++0x -ftest-coverage -fprofile-arcs -lncurses