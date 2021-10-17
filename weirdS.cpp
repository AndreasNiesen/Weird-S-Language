#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#define MAX_STACK 500

enum {
  SIMULATION,
  COMPILATION,
};

enum OP {
  NOP = -1, //   Not accessible from the outside.
  END,      // ^ End of the program. (No Operation as second argument)
  PUSH,     // / Pushes second argument onto the stack. (currently only ints, will add support for characters later on).
  ADD,      // + Adds (and removes if OP::POP is second argument) the last two entries on the stack and pushes the result.
  SUB,      // - Subtracts (and removes if OP::POP is second argument) the last from the second to last entry on the stack and pushes the result.
  DUMP,     // . Prints last entry on stack and either discards entry (OP::POP) or keeps it (OP::END).
  POP,      // \ Removes entry at stackCounter / last entry from stack.
  count,
};

void createProgram(char *input, std::vector<std::array<int, 2>> &prog);
void readFile(char *input, std::vector<std::string> &output);
void simulateProgram(std::vector<std::array<int, 2>> &prog);
void compileProgram(std::vector<std::array<int, 2>> &prog);

int main(int argc, char **argv) {
  int i = 0;
  int mode = SIMULATION; // default is Simulation mode.
  char *input = nullptr;
  char *compilationOutput = nullptr;

  std::vector<std::array<int, 2>> prog;

  std::map<std::string, int> options = {
    {"in", 0},
    {"i", 0},
    {"out", 1},
    {"o", 1},
    {"simulate", 2},
    {"sim", 2},
    {"s", 2},
    {"compile", 3},
    {"com", 3},
    {"c", 3},
  };

  while (argv[++i] != nullptr) {
    if (argv[i][0] == '-') {
      if (options.find(argv[i] + 1) != options.end()) {
        switch (options[argv[i] + 1]) {
          case 0: // Input File
                  if (input != nullptr) {
                    std::cout << "[ERROR] Multiple input files given: \"";
                    std::cout << input << "\" and \"" << argv[i + 1] << "\"" << std::endl;
                    return(0);
                  }
                  input = argv[++i];
                  std::cout << "[INFO] Input file: \"" << input << "\"" << std::endl;
                  break;
          case 1: // Output File
                  if (compilationOutput != nullptr) {
                    std::cout << "[ERROR] Multiple output files given: \"";
                    std::cout << compilationOutput << "\" and \"" << argv[i + 1] << "\"" << std::endl;
                    return(0);
                  }
                  compilationOutput = argv[++i];
                  std::cout << "[INFO] Output file: \"" << compilationOutput << "\"" << std::endl;
                  break;
          case 2: // Activate Simulation mode
                  mode = SIMULATION;
                  break;
          case 3: // Activate Compilation mode
                  mode = COMPILATION;
                  break;
        }
      }
    } else {
      // If no option-name is given, assume Input File
      if (input != nullptr) {
        std::cout << "[ERROR] Multiple input files given: \"";
        std::cout << input << "\" and \"" << argv[i] << "\"" << std::endl;
        return(0);
      }
      input = argv[i];
      std::cout << "[INFO] Input file: \"" << input << "\"" << std::endl;
    }
  }

  if (input == nullptr) {
    std::cout << "[ERROR] No input file given" << std::endl;
    return(0);
  }

  if (compilationOutput == nullptr && mode == COMPILATION) {
    compilationOutput = (char *)"out.o";
    std::cout << "[INFO] No output file given, using standard: \"" << compilationOutput << "\"" << std::endl;
  }

  std::cout << "[INFO] Mode: " << (mode == SIMULATION ? "Simulation" : "Compilation") << std::endl;

  createProgram(input, prog);
  
  switch(mode) {
    case SIMULATION:  simulateProgram(prog);
                      break;
    case COMPILATION: compileProgram(prog);
                      break;
    default:  std::cout << "[ERROR] Bad mode.";
              exit(0);
  }

  return(0);
}

void createProgram(char *input, std::vector<std::array<int, 2>> &prog) {
  std::vector<std::string> file_contents;
  readFile(input, file_contents);

  for (std::string buffer : file_contents) {
    static_assert(OP::count == 6, "Define all operations!");
    switch((int)buffer[0]) {
      case 43:  // "+"
                prog.push_back({OP::ADD, buffer[2] == '\\' ? OP::POP : OP::END});
                break;
      case 45:  // "-"
                prog.push_back({OP::SUB, buffer[2] == '\\' ? OP::POP : OP::END});
                break;
      case 46:  // "."
                prog.push_back({OP::DUMP, buffer[2] == '-' ? OP::END : OP::POP});
                break;
      case 47:  // "/"
                prog.push_back({OP::PUSH, atoi(buffer.c_str() + 2)});
                break;
      case 92:  // "\"
                prog.push_back({OP::POP, OP::NOP});
                break;
      case 94:  // "^"
                prog.push_back({OP::END, OP::NOP});
                break;
      default:  std::cout << "[ERROR] Could not understand \"" << buffer << "\"" << std::endl;
                exit(0);
    }
  }
}

void readFile(char *input, std::vector<std::string> &output) {
  std::fstream fs;
  std::string buffer;
  fs.open(input, std::fstream::in | std::fstream::binary);

  if (!fs.is_open()) {
    std::cout << "[ERROR] Could not open \"" << input << "\"" << std::endl;
    exit(0);
  }

  while(std::getline(fs, buffer)) {
    output.push_back(buffer);
  }

  fs.close();
}

void simulateProgram(std::vector<std::array<int, 2>> &prog) {
  int stack[MAX_STACK] = {0};
  int stackCounter = -1;

  int bufferA, bufferB;

  static_assert(OP::count == 6, "Define all operations!");

  for (auto cmd : prog) {
    switch(cmd[0]) {
      case OP::END:   // std::cout << "stackCounter: " << stackCounter << std::endl;
                      return;
      case OP::PUSH:  stack[++stackCounter] = cmd[1];
                      break;
      case OP::ADD:   if (cmd[1] == OP::END) {
                        bufferA = stack[stackCounter];
                        bufferB = stack[stackCounter-1];
                        stackCounter++;
                      } else if (cmd[1] == OP::POP) {
                        bufferA = stack[stackCounter];
                        bufferB = stack[--stackCounter];
                      } else {
                        std::cout << "[ERROR] ADD needs second argument." << std::endl;
                        exit(0);
                      }
                      stack[stackCounter] = bufferA + bufferB;
                      break;
      case OP::SUB:   if (cmd[1] == OP::END) {
                        bufferA = stack[stackCounter];
                        bufferB = stack[stackCounter-1];
                        stackCounter++;
                      } else if (cmd[1] == OP::POP) {
                        bufferA = stack[stackCounter];
                        bufferB = stack[--stackCounter];
                      } else {
                        std::cout << "[ERROR] SUB needs second argument." << std::endl;
                        exit(0);
                      }
                      stack[stackCounter] = bufferB - bufferA;
                      break;
      case OP::DUMP:  std::cout << stack[stackCounter] << std::endl;
                      if (cmd[1] == OP::POP) stackCounter--;
                      break;
      case OP::POP:   stackCounter--;
                      break;
      default:        std::cout << "[ERROR] In simulateProgram: Undefined operation." << std::endl;
    }
  }
}

void compileProgram(std::vector<std::array<int, 2>> &prog) {
  std::cout << "[ERROR] Compilation mode not yet implemented." << std::endl;
  exit(0);
}