json_src := https://github.com/nlohmann/json/releases/download/v3.7.0/json.hpp
objects := material.o object.o parse.o point.o
src := $(shell find . -name "*.cpp" -o -name "*.h")

nlohmann:
	@mkdir -p $@

nlohmann/json.hpp: nlohmann
	@curl -m 2 -Lso $@ $(json_src)

material.o: material.cpp
	g++ -std=c++17 -c -o $@ $^

object.o: nlohmann/json.hpp object.cpp
	g++ -std=c++17 -c -I nlohmann -o $@ object.cpp

parse.o: parse.cpp
	g++ -std=c++17 -c -o $@ $^

point.o: point.cpp
	g++ -std=c++17 -c -o $@ $^

clean:
	@rm -rf *.o nlohmann obj_to_json
.PHONY: clean

build: format $(objects)
	g++ -std=c++17 -o obj_to_json obj_to_json.cpp $(objects)
.PHONY: build

format:
	@clang-format -i $(src)
.PHONY: format
