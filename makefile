BUILDDIR = build/
BINDIR = ~/bin/
TESTDIR = test/
SRCDIR = src/
# Objects
OBJS =
TESTOBJS = $(BUILDDIR)/test.o
# Headers
INCLUDEPATH = include/
INCLUDE = -I$(INCLUDEPATH)
HEADERS = $(addprefix $(INCLUDEPATH)/, TileMap.hpp)
# Compiler settings
CXX = g++
CFLAGS = -fPIC -c -std=c++17 $(INCLUDE)
LFLAGS = -shared
TESTLFLAGS =
EXECLFLAGS =

$(TESTDIR)/test: $(TESTOBJS)
	$(CXX) $(TESTLFLAGS) $(TESTOBJS) -o $(TESTDIR)/test

$(BUILDDIR)/test.o: $(TESTDIR)/test.cpp $(HEADERS)
	$(CXX) $(CFLAGS) $(TESTDIR)/test.cpp -o $(BUILDDIR)/test.o


clean:
	rm $(OBJS) $(TESTOBJS) $(TESTDIR)/test

test: $(TESTDIR)/test
	$(TESTDIR)/test
