CXX       = g++
LD        = g++

ifeq ($(OS), Windows_NT)
	LIB_DIR = lib/win
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		LIB_DIR = lib/lnx
	endif
	ifeq ($(UNAME_S), Darwin)
		LIB_DIR = lib/osx
	endif
endif

CFLAGS    = -Wall -std=c++03 -pedantic-errors
LIBS      = -L$(LIB_DIR) -lspn

MODULES   = distribution .
SRC_DIR   = $(addprefix src/, $(MODULES))
BUILD_DIR = $(addprefix build/, $(MODULES))
BIN_DIR   = bin
SPNDIR =  lib/spn

SRC       = $(foreach sdir, $(SRC_DIR), $(wildcard $(sdir)/*.cpp))
OBJ       = $(patsubst src/%.cpp, build/%.o, $(SRC))
DEP       = $(OBJ:%.o=%.d)
INCLUDES  = $(addprefix -I, $(SRC_DIR)) -Iinclude

TARGET    = corrstat

DEBUG     ?= 1
ifeq ($(DEBUG), 1)
	CFLAGS += -O0 -g -pg -DDEBUG
	LDFLAGS = -O0 -g -pg -DDEBUG
else
	CFLAGS += -O3
	LDFLAGS = -O3
endif

-include $(DEP)

vpath %.cpp $(SRC_DIR)

define make-goal
$1/%.o: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -MMD -c $$< -o $$@
endef

.PHONY: all checkdirs clean

all: bindir builddirs $(TARGET)

$(TARGET): $(OBJ)
	$(MAKE) -C $(SPNDIR)
	$(LD) $^ -o $(BIN_DIR)/$@ $(LDFLAGS) $(LIBS)

bindir:
	mkdir -p $(BIN_DIR)

builddirs: $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR)/$(TARGET)
	rm -rf build/*

$(foreach bdir, $(BUILD_DIR), $(eval $(call make-goal, $(bdir))))
