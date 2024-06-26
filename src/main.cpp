#include <iostream>
#include <sstream>
#include <filesystem>
using namespace std;

std::string get_path(std::string command){
    std::string path_env = std::getenv("PATH");
    std::stringstream ss(path_env);
    std::string path;
    while(!ss.eof()){
        getline(ss, path, ':');
        string abs_path = path + '/' + command;
        if(filesystem::exists(abs_path)){
            return abs_path;
        }
    }
    return "";
}

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
    if(input.substr(0, 4) == "echo") {
      std::cout << input.substr(5) << "\n";
      std::cout << "$ ";
      continue;
    }
    if(input.substr(0, 4) == "type") {
      std::string user_input = input.substr(5);
      if(user_input == "echo" || user_input == "type" || user_input == "exit") std::cout << user_input << " is a shell builtin\n";
      else {
        std::string path = get_path(user_input);
        if(path == "") std::cout << user_input << ": not found\n";
        else std::cout << user_input << " is " << path << "\n";
      }
      std::cout << "$ ";
      continue;
    }
    std::cout << "$ ";
  }
}
