#include <iostream>
#include "parse.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: obj_to_json <file>" << std::endl;
        return 1;
    }

    parse(argv[1]);

    return 0;
}
