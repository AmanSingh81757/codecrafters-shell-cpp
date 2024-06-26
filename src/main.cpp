#include <iostream>
#include <sstream>
#include <filesystem>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

using namespace std;

std::string exec(const std::string& command) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

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

void handleEcho(string input){
  std::cout << input.substr(5) << "\n";
  std::cout << "$ ";
}

void handleType(string input){
  std::string user_input = input.substr(5);
  if(user_input == "echo" || user_input == "type" || user_input == "exit" || user_input == "pwd") std::cout << user_input << " is a shell builtin\n";
  else {
    std::string path = get_path(user_input);
    if(path == "") std::cout << user_input << ": not found\n";
    else std::cout << user_input << " is " << path << "\n";
  }
  std::cout << "$ ";
}

void handleCommandNotFound(string input){
  std::cout << input << ": command not found\n";
  std::cout << "$ ";
}

void handleRunProgram(string input){
  string command = "", param = "";
  int i = 0;
  while(i<input.size() && input[i] != ' ') {
    command+=input[i];
    i++;
  }
  i++;
  while(i<input.size() && input[i] != ' ') {
    param+=input[i];
    i++;
  }
  string path = get_path(command);
  if(path == ""){
    handleCommandNotFound(input);
    return;
  }
  // string output = exec(command + " " + param);
  // cout << output << "\n";
  string com = command + " " + param;
  std::system(com.c_str());
  std::cout << "$ ";
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
    std::string user_input = input.substr(5);
    if(input == "pwd" and input.size() >= 3){
      std::string cwd = std::filesystem::current_path();
      std::string print_cwd = cwd.substr(0, cwd.length());
      std::cout << print_cwd << "\n";
      std::cout << "$ ";
      continue;
    }
    else if(input == "exit 0" and input.size()>=6) break;
    else if(input.substr(0, 4) == "echo") {
      handleEcho(input);
      continue;
    }
    else if(input.substr(0, 4) == "type" and input.size()>4) {
      handleType(input);
      continue;
    }
    else{
      handleRunProgram(input);
    }
  }
}
