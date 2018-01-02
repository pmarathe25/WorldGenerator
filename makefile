BUILDDIR = build/
BINDIR = ~/bin/
TESTDIR = test/
SRCDIR = src/
# Objects
TESTOBJS = $(addprefix $(BUILDDIR)/, noiseTest.o terrainTest.o)
# Headers
INCLUDEPATH = include/
INCLUDE = -I$(INCLUDEPATH)
HEADERS = $(addprefix $(INCLUDEPATH)/, config.hpp Vector2.hpp \
	Terrain/TerrainMap.hpp Terrain/TerrainGenerator.hpp)
# Compiler settings
CXX = g++
CFLAGS = -fPIC -c -std=c++17 $(INCLUDE) -O3 -Wpedantic -march=native
LFLAGS = -shared -flto -march=native
TESTLFLAGS = -lstealthcolor -lsfml-graphics -lsfml-window -lsfml-system -pthread -flto -march=native
EXECLFLAGS = -flto -march=native

all: $(TESTOBJS)

$(TESTDIR)/noiseTest: $(BUILDDIR)/noiseTest.o $(HEADERS) $(OBJS)
	$(CXX) $(BUILDDIR)/noiseTest.o $(OBJS) $(TESTLFLAGS) -o $(TESTDIR)/noiseTest

$(BUILDDIR)/noiseTest.o: $(TESTDIR)/noiseTest.cpp $(HEADERS)
	$(CXX) $(CFLAGS) $(TESTDIR)/noiseTest.cpp -o $(BUILDDIR)/noiseTest.o

$(TESTDIR)/terrainTest: $(BUILDDIR)/terrainTest.o $(HEADERS) $(OBJS)
	$(CXX) $(BUILDDIR)/terrainTest.o $(OBJS) $(TESTLFLAGS) -o $(TESTDIR)/terrainTest

$(BUILDDIR)/terrainTest.o: $(TESTDIR)/terrainTest.cpp $(HEADERS)
	$(CXX) $(CFLAGS) $(TESTDIR)/terrainTest.cpp -o $(BUILDDIR)/terrainTest.o

clean:
	rm $(OBJS) $(TESTOBJS) $(TESTDIR)/noiseTest

terraintest: $(TESTDIR)/terrainTest
	$(TESTDIR)/terrainTest

noisetest: $(TESTDIR)/noiseTest
	$(TESTDIR)/noiseTest

testall:
	$(TESTDIR)/noiseTest & $(TESTDIR)/terrainTest
