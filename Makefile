include_dir := include
json_src := https://github.com/nlohmann/json/releases/download/v3.7.0/json.hpp

include_dir:
	@mkdir -p $(include_dir)

json.hpp: include_dir
	@curl -Lso $(include_dir)/$@ $(json_src)

clean:
	@rm -rf $(include_dir)
.PHONY: clean

build: json.hpp
	g++ -o obj_to_json -I $(include_dir) obj_to_json.cpp
.PHONY: build
