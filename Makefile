CXX = g++
CFLAGS = -g -Wall -O2 -pedantic
LIBS = -Llib -lspn
INCLUDES = -Iinclude

TARGET = corrstat
BINDIR = bin
SRCDIR = src
BUILDDIR = build

.PHONY: destdir all clean

all: $(TARGET)

OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(wildcard $(SRCDIR)/*.cpp))
HEADERS = $(wildcard $(SRCDIR)/*.h)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): destdir $(OBJECTS)
	$(CXX) $(OBJECTS) -Wall $(LIBS) -o $(BINDIR)/$@

destdir:
	mkdir -p ./bin
	mkdir -p ./build

clean:
	-rm -f $(BUILDDIR)/* $(BINDIR)/$(TARGET)
