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
HEADERS = $(addprefix $(INCLUDEPATH)/, TileMap/TileMap.hpp TileMap/TileMapBase.hpp TileMap/ForwardDeclarations.hpp \
	TileMap/Operations/InternalOperations.hpp TileMap/Operations/BinaryOperations.hpp TileMap/Operations/UnaryOperations.hpp\
	TileMap/BinaryOp.hpp TileMap/UnaryOp.hpp Utility.hpp Vector2.hpp Terrain/TerrainTile.hpp Noise/NoiseGenerator.hpp Noise/InterpolationKernel.hpp)
# Compiler settings
CXX = g++
CFLAGS = -fPIC -c -std=c++17 $(INCLUDE) -O3
LFLAGS = -shared -flto
TESTLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -pthread -flto
EXECLFLAGS = -flto

$(TESTDIR)/test: $(TESTOBJS) $(HEADERS)
	$(CXX) $(TESTOBJS) $(TESTLFLAGS) -o $(TESTDIR)/test

$(BUILDDIR)/test.o: $(TESTDIR)/test.cpp $(HEADERS)
	$(CXX) $(CFLAGS) $(TESTDIR)/test.cpp -o $(BUILDDIR)/test.o

clean:
	rm $(OBJS) $(TESTOBJS) $(TESTDIR)/test

test: $(TESTDIR)/test
	$(TESTDIR)/test
