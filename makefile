# Makefile for Doodle Jump (SFML 3)
#
# Designed to be run from Git Bash on Windows with a MinGW-w64 g++ toolchain
# and SFML 3 installed (headers + libs discoverable by the compiler/linker,
# e.g. SFML's "include" and "lib" folders added to MinGW installation,
# or SFML installed alongside the compiler). It also works unmodified on
# Linux as long as the SFML 3 dev packages are installed.
#
# NOTE: SFML 3 builds static libraries by default. If you linked against a
# static build of SFML, you will additionally need to link SFML's own
# dependencies (freetype, opengl32, winmm, gdi32, etc. on Windows) and add
# -DSFML_STATIC to CXXFLAGS. If you used the prebuilt "GCC (MinGW)" shared
# package from sfml-dev.org (the simplest option), the flags below are all
# you need — remember to copy the SFML .dll files next to the .exe
#
# Phase 2 note: -lsfml-audio was added below for sound support (SoundManager,
# background music, sound effects). If linking statically on Windows, audio
# additionally needs -lopenal32 and -lFLAC/-lvorbis/-logg (whichever SFML's
# own build pulled in) per SFML's own dependency list for sfml-audio.

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS  := -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system

SRC_DIR   := src
BUILD_DIR := build
BIN_DIR   := bin

ifeq ($(OS),Windows_NT)
    TARGET    := $(BIN_DIR)/doodle-jump.exe
    LDFLAGS   += -static-libgcc -static-libstdc++
else
    TARGET    := $(BIN_DIR)/doodle-jump
endif


SOURCES := $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
