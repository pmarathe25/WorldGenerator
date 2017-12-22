BUILDDIR = build/
BINDIR = ~/bin/
TESTDIR = test/
SRCDIR = src/
# Objects
OBJS = $(addprefix $(BUILDDIR)/, Color.o ColorPalette.o)
TESTOBJS = $(addprefix $(BUILDDIR)/, noiseTest.o terrainTest.o)
# Headers
INCLUDEPATH = include/
INCLUDE = -I$(INCLUDEPATH)
HEADERS = $(addprefix $(INCLUDEPATH)/, TileMap/TileMap.hpp TileMap/TileMapBase.hpp TileMap/ForwardDeclarations.hpp \
	TileMap/Operations/InternalOperations.hpp TileMap/Operations/BinaryOperations.hpp TileMap/Operations/UnaryOperations.hpp\
	TileMap/BinaryOp.hpp TileMap/UnaryOp.hpp Utility.hpp Vector2.hpp Noise/NoiseGenerator.hpp Noise/InterpolationKernel.hpp\
	Terrain/TerrainMap.hpp Color/Color.hpp Color/ColorPalette.hpp Color/ColorMap.hpp)
# Compiler settings
CXX = g++
CFLAGS = -fPIC -c -std=c++17 $(INCLUDE) -O3
LFLAGS = -shared -flto
TESTLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -pthread -flto
EXECLFLAGS = -flto


$(TESTDIR)/noiseTest: $(BUILDDIR)/noiseTest.o $(HEADERS) $(OBJS)
	$(CXX) $(BUILDDIR)/noiseTest.o $(OBJS) $(TESTLFLAGS) -o $(TESTDIR)/noiseTest

$(BUILDDIR)/noiseTest.o: $(TESTDIR)/noiseTest.cpp $(HEADERS)
	$(CXX) $(CFLAGS) $(TESTDIR)/noiseTest.cpp -o $(BUILDDIR)/noiseTest.o

$(TESTDIR)/terrainTest: $(BUILDDIR)/terrainTest.o $(HEADERS) $(OBJS)
	$(CXX) $(BUILDDIR)/terrainTest.o $(OBJS) $(TESTLFLAGS) -o $(TESTDIR)/terrainTest

$(BUILDDIR)/terrainTest.o: $(TESTDIR)/terrainTest.cpp $(HEADERS)
	$(CXX) $(CFLAGS) $(TESTDIR)/terrainTest.cpp -o $(BUILDDIR)/terrainTest.o

$(BUILDDIR)/Color.o: $(SRCDIR)/Color/Color.cpp include/Color/Color.hpp
	$(CXX) $(CFLAGS) $(SRCDIR)/Color/Color.cpp -o $(BUILDDIR)/Color.o

$(BUILDDIR)/ColorPalette.o: $(SRCDIR)/Color/ColorPalette.cpp include/Color/ColorPalette.hpp
	$(CXX) $(CFLAGS) $(SRCDIR)/Color/ColorPalette.cpp -o $(BUILDDIR)/ColorPalette.o

clean:
	rm $(OBJS) $(TESTOBJS) $(TESTDIR)/noiseTest

terraintest: $(TESTDIR)/terrainTest
	$(TESTDIR)/terrainTest

noisetest: $(TESTDIR)/noiseTest
	$(TESTDIR)/noiseTest

testall:
	$(TESTDIR)/noiseTest & $(TESTDIR)/terrainTest

test: testTerrain
