#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>

void print_usage() {
  std::cerr << "Usage: tac [-b] [-s SEP] [FILE]..." << std::endl;
}

int main(int argc, char* argv[]) {
  bool separate_blank_lines = false;
  std::string separator;

  // Parse command-line flags
  int i = 1;
  while (i < argc && argv[i][0] == '-') {
    if (argv[i] == std::string("-b")) {
      separate_blank_lines = true;
      ++i;
    } else if (argv[i] == std::string("-s")) {
      if (i + 1 >= argc) {
        std::cerr << "Error: missing argument for -s flag." << std::endl;
        print_usage();
        return 1;
      }
      separator = argv[i + 1];
      i += 2;
    } else {
      std::cerr << "Error: invalid flag: " << argv[i] << std::endl;
      print_usage();
      return 1;
    }
  }

  // Read input from the specified files (or standard input if no files are given)
  std::vector<std::string> lines;
  if (i >= argc) {
    // Read from standard input
    std::string line;
    while (std::getline(std::cin, line)) {
      lines.push_back(line);
    }
  } else {
    // Read from files
    for (; i < argc; ++i) {
      std::ifstream input(argv[i]);
      if (!input) {
        std::cerr << "Error: unable to open input file: " << argv[i] << std::endl;
        return 1;
      }
      std::string line;
      while (std::getline(input, line)) {
        lines.push_back(line);
      }
    }
  }

  // Reverse the lines and print them
  if (separate_blank_lines) {
    std::reverse(lines.begin(), lines.end());
    for (const auto& line : lines) {
      std::cout << line << std::endl;
    }
  } else {
    std::vector<std::string> non_blank_lines;
    for (const auto& line : lines) {
      if (!line.empty()) {
        non_blank_lines.push_back(line);
      }
    }
    std::reverse(non_blank_lines.begin(), non_blank_lines.end());
    for (const auto& line : non_blank_lines) {
      std::cout << line << separator;
    }
    std::cout << std::endl;
  }

  return 0;
}
