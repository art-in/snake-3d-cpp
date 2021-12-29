#include "utils.hpp"

#include <fstream>
#include <sstream>

auto read_file_to_string(const std::string& file_path) -> std::string {
  std::fstream f{file_path};
  std::stringstream s;
  s << f.rdbuf();
  return s.str();
}