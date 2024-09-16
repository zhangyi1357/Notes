#include <iostream>
#include <stdexcept>
#include <string>

void print_prompt() { std::cout << "db > "; }

std::string read_input() {
  std::string input_buffer;
  if (!std::getline(std::cin, input_buffer)) {
    throw std::runtime_error("Error reading input");
  }
  return input_buffer;
}

int main() {
  try {
    while (true) {
      print_prompt();
      std::string input = read_input();

      if (input == ".exit") {
        return 0;
      }
      std::cout << "Unrecognized command '" << input << "'.\n";
    }
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}