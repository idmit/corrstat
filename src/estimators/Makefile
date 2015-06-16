CXX       = g++
LD        = g++

CFLAGS    = -shared

SRC_DIR   = .
BUILD_DIR = .
BIN_DIR   = .

SRC       = $(wildcard $(SRC_DIR)/*.cpp)
LIBS      = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.so, $(SRC))
INCLUDES  = $(addprefix -I, $(SRC_DIR)) -Iinclude

DEBUG     ?= 1
ifeq ($(DEBUG), 1)
	CFLAGS += -g -pg -DDEBUG
else
	CFLAGS += -O3
endif

.PHONY: all

all: $(LIBS)

%.so: %.cpp
	$(CXX) $(CFLAGS) $< ../../build/data.o -o $@
