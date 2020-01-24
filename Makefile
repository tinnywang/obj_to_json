include_dir := include
json_src := https://github.com/nlohmann/json/releases/download/v3.7.0/json.hpp
src := $$(find . -name "*.cpp" -o -name "*.h")


include_dir:
	@mkdir -p $(include_dir)

$(include_dir)/json.hpp: include_dir
	@curl -m 2 -Lso $@ $(json_src)

parse.o: parse.h parse.cpp
	g++ -std=c++17 -o $@ -c parse.cpp

clean:
	@rm -rf $(include_dir) *.o
.PHONY: clean

build: format $(include_dir)/json.hpp parse.o
	g++ -std=c++17 -o obj_to_json -I $(include_dir) obj_to_json.cpp parse.o
.PHONY: build

format:
	@clang-format -i $(src)
.PHONY: format
