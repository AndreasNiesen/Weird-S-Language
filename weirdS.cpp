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
  END,      // ^ End of the program. (Endpoint of jump if second argument is given)
  PUSH,     // / Pushes second argument onto the stack. (currently only ints, will add support for characters later on)
  ADD,      // + Adds (and removes if OP::POP is second argument) the last two entries on the stack and pushes the result.
  SUB,      // - Subtracts (and removes if OP::POP is second argument) the last from the second to last entry on the stack and pushes the result.
  DUMP,     // . Prints last entry on stack and either discards entry (OP::POP) or keeps it (OP::END).
  POP,      // \ Removes last entry from stack. (Saves into ID given by second argument if given)
  GT,       // > Compares the last two stack entries (removing the last one) and jumping to ID given by second argument, if second to last > last.
  EQ,       // = Compares the last two stack entries (removing the last one) and jumping to ID given by second argument, if second to last == last.
  LT,       // < Compares the last two stack entries (removing the last one) and jumping to ID given by second argument, if second to last < last.
  JMP,      // ! (Unconditional) Jump to the ID given by the second argument.
  RSRV,     // ( Reserves place on the stack if ID in second argument not yet reserved. (Currently a single int per ID, will change it to be of variable size)
  LOAD,     // ) Pushes the value in the ID given by the second argument on the stack.
  count,
};

void createProgram(char *input, std::vector<std::array<int, 2>> &prog);
void readFile(char *input, std::vector<std::string> &output);
void simulateProgram(std::vector<std::array<int, 2>> &prog);
void compileProgram(std::vector<std::array<int, 2>> &prog);
int matchingIDsLine(std::vector<std::string> &file_contents, std::string &branchID);
void printUsage(char *name);

int main(int argc, char **argv) {
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
    {"help", 4},
    {"h", 4},
  };

  int i = 0;
  while (argv[++i] != nullptr) {
    if (argv[i][0] == '-') {
      if (options.find(argv[i] + 1) != options.end()) {
        switch (options[argv[i] + 1]) {
          case 0: // Input File
                  if (input != nullptr) {
                    std::cout << "[ERROR] Multiple input files given: \"";
                    std::cout << input << "\" and \"" << argv[i + 1] << "\"\n" << std::endl;
                    printUsage(argv[0]);
                    return(0);
                  }
                  input = argv[++i];
                  std::cout << "[INFO] Input file: \"" << input << "\"" << std::endl;
                  break;
          case 1: // Output File
                  if (compilationOutput != nullptr) {
                    std::cout << "[ERROR] Multiple output files given: \"";
                    std::cout << compilationOutput << "\" and \"" << argv[i + 1] << "\"\n" << std::endl;
                    printUsage(argv[0]);
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
          case 4: // print help and exit.
                  printUsage(argv[0]);
                  exit(0);
        }
      }
    } else {
      // If no option-name is given, assume Input File
      if (input != nullptr) {
        std::cout << "[ERROR] Multiple input files given: \"";
        std::cout << input << "\" and \"" << argv[i] << "\"\n" << std::endl;
        printUsage(argv[0]);
        return(0);
      }
      input = argv[i];
      std::cout << "[INFO] Input file: \"" << input << "\"" << std::endl;
    }
  }

  if (input == nullptr) {
    std::cout << "[ERROR] No input file given\n" << std::endl;
    printUsage(argv[0]);
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
    default:  std::cout << "[ERROR] Bad mode.\n" << std::endl;
              printUsage(argv[0]);
              exit(0);
  }

  return(0);
}

void createProgram(char *input, std::vector<std::array<int, 2>> &prog) {
  std::vector<std::string> file_contents;
  std::map<std::string, int> vars;
  readFile(input, file_contents);
  std::string branchID;
  int counter = 0;
  int branchEndLine;

  for (int i = 0; i < file_contents.size(); i++) {
    static_assert(OP::count == 12, "Define all operations!");

    std::string buffer = file_contents[i];
    switch((int)buffer[0]) {
      case 33:  // "!"
                branchID = buffer.substr(2, buffer.find(" ", 2));  // make sure branchID does not contain trailing whitespace.
                branchEndLine = matchingIDsLine(file_contents, branchID);
                if (branchEndLine == -1) {
                  std::cout << "[ERROR] JMP: Did not find matching branch-ID for \"" << branchID << "\"" << std::endl;
                  exit(0);
                }
                prog.push_back({OP::JMP, branchEndLine});
                break;
      case 40:  // (
                branchID = buffer.substr(2, buffer.find(" ", 2));
                vars[branchID] = counter;
                prog.push_back({OP::RSRV, counter++});
                break;
      case 41:  // )
                branchID = buffer.substr(2, buffer.find(" ", 2));
                if (!vars.contains(branchID)) {
                  std::cout << "[ERROR] Variable \"" << branchID << "\" needs to be registered before use" << std::endl;
                  exit(0);
                }
                prog.push_back({OP::LOAD, vars[branchID]});
                break;
      case 43:  // "+"
                prog.push_back({OP::ADD, buffer[2] == '\\' ? OP::POP : OP::END});
                break;
      case 45:  // "-"
                prog.push_back({OP::SUB, buffer[2] == '\\' ? OP::POP : OP::END});
                break;
      case 46:  // "."
                prog.push_back({OP::DUMP, buffer[2] == '\\' ? OP::POP : OP::END});
                break;
      case 47:  // "/"
                prog.push_back({OP::PUSH, atoi(buffer.c_str() + 2)});
                break;
      case 60:  // <
                branchID = buffer.substr(2, buffer.find(" ", 2));  // make sure branchID does not contain trailing whitespace.
                branchEndLine = matchingIDsLine(file_contents, branchID);
                if (branchEndLine == -1) {
                  std::cout << "[ERROR] LT: Did not find matching branch-ID for \"" << branchID << "\"" << std::endl;
                  exit(0);
                }
                prog.push_back({OP::LT, branchEndLine});
                break;
      case 61:  // =
                branchID = buffer.substr(2, buffer.find(" ", 2));
                branchEndLine = matchingIDsLine(file_contents, branchID);
                if (branchEndLine == -1) {
                  std::cout << "[ERROR] EQ: Did not find matching branch-ID for \"" << branchID << "\"" << std::endl;
                  exit(0);
                }
                prog.push_back({OP::EQ, branchEndLine});
                break;
      case 62:  // >
                branchID = buffer.substr(2, buffer.find(" ", 2));
                branchEndLine = matchingIDsLine(file_contents, branchID);
                if (branchEndLine == -1) {
                  std::cout << "[ERROR] GT: Did not find matching branch-ID for \"" << branchID << "\"" << std::endl;
                  exit(0);
                }
                prog.push_back({OP::GT, branchEndLine});
                break;
      case 92:  // "\"
                if (buffer.length() > 2 && buffer[3] != ' ') {
                  branchID = buffer.substr(2, buffer.find(" ", 2));
                  prog.push_back({OP::POP, vars[branchID]});
                  break;
                }
                prog.push_back({OP::POP, OP::NOP});
                break;
      case 94:  // "^"
                if (buffer.length() > 2 && buffer[3] != ' ') {
                  prog.push_back({OP::NOP, OP::NOP});
                  break;
                }
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
    // if comment (marked by "#" is found, remove it.)
    buffer = buffer.substr(0, buffer.find("#"));
    if (buffer.length() == 0) continue;
    output.push_back(buffer);
  }

  fs.close();
}

void simulateProgram(std::vector<std::array<int, 2>> &prog) {
  int stack[MAX_STACK] = {0};
  int stackCounter = -1;

  int bufferA, bufferB;

  static_assert(OP::count == 12, "Define all operations!");

  for (int i = 0; i < prog.size(); i++) {
    auto cmd = prog[i];
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
      case OP::POP:   if (cmd[1] == OP::NOP) stackCounter--;
                      else stack[cmd[1]] = stack[stackCounter--];
                      break;
      case OP::GT:    bufferA = stack[stackCounter--];
                      bufferB = stack[stackCounter];
                      if (bufferB > bufferA) {
                        i = cmd[1] - 1;
                      }
                      break;
      case OP::EQ:    bufferA = stack[stackCounter--];
                      bufferB = stack[stackCounter];
                      if (bufferB == bufferA) {
                        i = cmd[1] - 1;
                      }
                      break;
      case OP::LT:    bufferA = stack[stackCounter--];
                      bufferB = stack[stackCounter];
                      if (bufferB < bufferA) {
                        i = cmd[1];
                      }
                      break;
      case OP::JMP:   i = cmd[1];
                      break;
      case OP::RSRV:  stack[++stackCounter] = cmd[1];
                      break;
      case OP::LOAD:  stack[++stackCounter] = stack[cmd[1]];
                      break;
      // default:        std::cout << "[ERROR] In simulateProgram: Undefined operation." << std::endl;
      // ^ currently OP::NOP lands here, but does not require any handling.
    }
  }
}

void compileProgram(std::vector<std::array<int, 2>> &prog) {
  for (auto cmd : prog) {
    std::cout << "cmd: " << cmd[0] << "\t | arg: " << cmd[1] << std::endl;
  }
  // TODO: Remove ^that. Just useful for debugging purposes.
  std::cout << "[ERROR] Compilation mode not yet implemented." << std::endl;
  exit(0);
}

int matchingIDsLine(std::vector<std::string> &file_contents, std::string &branchID) {
  for (int i = 0; i < file_contents.size(); i++) {
    if (file_contents[i][0] != '^') continue;
    if (file_contents[i].length() > 2 && file_contents[i][3] != ' ' && branchID == file_contents[i].substr(2, file_contents[i].find(" ", 2))) {
      return(i);
    }
  }

  return(-1);
}

void printUsage(char *name) {
  std::cout << name << " [OPTIONS] input_file\n" << std::endl;
  std::cout << "OPTIONS:" << std::endl;
  std::cout << "\t-c, -com, -compile: \tUse compilation-mode." << std::endl;
  std::cout << "\t-s, -sim, -simulate: \tUse simulation-mode. (default)" << std::endl;
  std::cout << "\t-o, -out: \t\tName of the output file. (compilation-mode only)" << std::endl;
  std::cout << "\t-h, -help: \t\tDisplay this help.\n" << std::endl;
  std::cout << "Example: " << std::endl;
  std::cout << "\t" << name << " -s test.wS" << std::endl;
  std::cout << "\t" << name << " -c -o myProg.o test.wS\n" << std::endl;
}