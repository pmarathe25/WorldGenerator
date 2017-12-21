BUILDDIR = build/
BINDIR = ~/bin/
TESTDIR = test/
SRCDIR = src/
# Objects
OBJS =
TESTOBJS = $(addprefix $(BUILDDIR)/, noiseTest.o terrainTest.o)
# Headers
INCLUDEPATH = include/
INCLUDE = -I$(INCLUDEPATH)
HEADERS = $(addprefix $(INCLUDEPATH)/, TileMap/TileMap.hpp TileMap/TileMapBase.hpp TileMap/ForwardDeclarations.hpp \
	TileMap/Operations/InternalOperations.hpp TileMap/Operations/BinaryOperations.hpp TileMap/Operations/UnaryOperations.hpp\
	TileMap/BinaryOp.hpp TileMap/UnaryOp.hpp Utility.hpp Vector2.hpp Noise/NoiseGenerator.hpp Noise/InterpolationKernel.hpp\
	Terrain/TerrainMap.hpp)
# Compiler settings
CXX = g++
CFLAGS = -fPIC -c -std=c++17 $(INCLUDE) -O3
LFLAGS = -shared -flto
TESTLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -pthread -flto
EXECLFLAGS = -flto

$(TESTDIR)/noiseTest: $(BUILDDIR)/noiseTest.o $(HEADERS)
	$(CXX) $(BUILDDIR)/noiseTest.o $(TESTLFLAGS) -o $(TESTDIR)/noiseTest

$(BUILDDIR)/noiseTest.o: $(TESTDIR)/noiseTest.cpp $(HEADERS)
	$(CXX) $(CFLAGS) $(TESTDIR)/noiseTest.cpp -o $(BUILDDIR)/noiseTest.o

$(TESTDIR)/terrainTest: $(BUILDDIR)/terrainTest.o $(HEADERS)
	$(CXX) $(BUILDDIR)/terrainTest.o $(TESTLFLAGS) -o $(TESTDIR)/terrainTest

$(BUILDDIR)/terrainTest.o: $(TESTDIR)/terrainTest.cpp $(HEADERS)
	$(CXX) $(CFLAGS) $(TESTDIR)/terrainTest.cpp -o $(BUILDDIR)/terrainTest.o

clean:
	rm $(OBJS) $(TESTOBJS) $(TESTDIR)/noiseTest

testTerrain: $(TESTDIR)/terrainTest
	$(TESTDIR)/terrainTest

testNoise: $(TESTDIR)/noiseTest
	$(TESTDIR)/noiseTest

testAll: $(TESTDIR)/noiseTest $(TESTDIR)/terrainTest
	$(TESTDIR)/noiseTest & $(TESTDIR)/terrainTest
