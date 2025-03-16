CXX := g++
CXXFLAGS := -g -std=c++17 -DMEMTRACE
OUTPUT := output
SRC := main.cpp \
    devtools/memtrace.cpp \
    console/ui.cpp \
    consts/consts.cpp \
    io/io.cpp \
    sysmod/sysmod.cpp
INCLUDES := -Iconsole -Iconsts -Idevtools -Iio -Isysmod

.PHONY: all build run clean

all: build

build: $(SRC)
	$(CXX) $(CXXFLAGS) -o $(OUTPUT) $(SRC) $(INCLUDES)

run: build
	./$(OUTPUT)

clean:
	rm -f $(OUTPUT)
