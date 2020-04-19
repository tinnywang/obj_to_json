json_src := https://github.com/nlohmann/json/releases/download/v3.7.0/json.hpp
objects := material.o object.o parse.o point.o
src := $(shell find . -name "*.cpp" -o -name "*.h")

CXX := g++
CXXFLAGS := -std=c++17
LDFLAGS := -lstdc++fs

nlohmann:
	@mkdir -p $@

nlohmann/json.hpp: nlohmann
	@curl -m 2 -Lso $@ $(json_src)

material.o: material.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $^

object.o: object.cpp
	$(CXX) -c $(CXXFLAGS) -I nlohmann -o $@ object.cpp

parse.o: parse.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $^

point.o: point.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $^

clean:
	@rm -rf *.o nlohmann obj_to_json
.PHONY: clean

build: format nlohmann/json.hpp $(objects)
	$(CXX) $(CXXFLAGS) -o obj_to_json obj_to_json.cpp $(objects) $(LDFLAGS)
.PHONY: build

format:
	@clang-format -i $(src)
.PHONY: format
