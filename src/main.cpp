#include <iostream>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // Uncomment this block to pass the first stage
  std::cout << "$ ";
  while(true) {
    std::string input;
    std::getline(std::cin, input);
    if(input == "exit 0") break;
    if(input.substr(0, 4) == "echo") std::cout << input.substr(5) << "\n";
    else std::cout << input << ": command not found\n";
    std::cout << "$ ";
  }
}
