#include <exception>
#include <fstream>
#include <math.h>
#include <string>

namespace parse {
std::ifstream open(const std::string &filename) {
  std::ifstream file(filename);
  if (file.fail()) {
    throw std::invalid_argument("Failed to open file: " + filename);
  }
  return file;
}

double round(double d, double precision) {
  double shift = ::pow(10, precision);
  return ::round(d * shift) / shift;
}
} // namespace parse