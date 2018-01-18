BUILDDIR = build/
BINDIR = ~/bin/
TESTDIR = test/
SRCDIR = src/
# Objects
TESTOBJS = $(addprefix $(BUILDDIR)/, terrainTest.o)
# Headers
INCLUDEPATH = include/
INCLUDE = -I$(INCLUDEPATH)
HEADERS = $(addprefix $(INCLUDEPATH)/, config.hpp Vector2.hpp \
	Terrain/TerrainMap.hpp Terrain/TerrainGenerator.hpp)
# Compiler settings
CXX = g++
CFLAGS = -fPIC -c -std=c++17 $(INCLUDE) -flto -O3 -Wpedantic -march=native
LFLAGS = -shared -flto -O3 -march=native
TESTLFLAGS = -lstealthcolor -lsfml-graphics -lsfml-window -lsfml-system -flto -O3 -march=native
EXECLFLAGS = -flto -O3 -march=native

$(TESTDIR)/terrainTest: $(BUILDDIR)/terrainTest.o $(HEADERS) $(OBJS)
	$(CXX) $(BUILDDIR)/terrainTest.o $(OBJS) $(TESTLFLAGS) -o $(TESTDIR)/terrainTest

$(BUILDDIR)/terrainTest.o: $(TESTDIR)/terrainTest.cpp $(HEADERS)
	$(CXX) $(CFLAGS) $(TESTDIR)/terrainTest.cpp -o $(BUILDDIR)/terrainTest.o

clean:
	rm $(OBJS) $(TESTOBJS) $(TESTDIR)/terrainTest

test: $(TESTDIR)/terrainTest
	$(TESTDIR)/terrainTest
