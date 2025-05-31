#include <Instruction.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

Instruction::Instruction(const string& opCode, const string& destiny,
                         const string& source1, const string& source2)
    : opCode(stringToOpCode(opCode)),
      destiny(destiny),
      src1(source1),
      src2(source2) {}

Instruction::Instruction(const OpCode& opCode, const string& destiny,
                         const string& source1, const string& source2)
    : opCode(opCode), destiny(destiny), src1(source1), src2(source2) {}

shared_ptr<deque<shared_ptr<Instruction>>> Instruction::fromInstructionFile(
    const string& path) {
  shared_ptr<deque<shared_ptr<Instruction>>> instructions =
      make_shared<deque<shared_ptr<Instruction>>>();

  ifstream file(path);
  if (!file.is_open()) {
    throw runtime_error("Não foi possível ler o arquivo.");
  }

  string line;
  while (getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    instructions->push_back(fromLine(line));
  }

  return instructions;
}

shared_ptr<Instruction> Instruction::fromLine(const string& line) {
  stringstream ss(line);
  string operation, destiny, src1, src2;

  ss >> operation >> destiny >> src1 >> src2;
  return make_shared<Instruction>(operation, destiny, src1, src2);
}

void Instruction::setIssue(const int& issue) { this->issue = issue; }

OpCode Instruction::stringToOpCode(const string& opStr) {
  static const unordered_map<string, OpCode> opMap = {{"LD", OpCode::LOAD},
                                                      {"ADD", OpCode::ADD},
                                                      {"SUB", OpCode::SUB},
                                                      {"MUL", OpCode::MUL},
                                                      {"DIV", OpCode::DIV}};
  auto it = opMap.find(opStr);
  return (it != opMap.end()) ? it->second : OpCode::INVALID;
}

bool Instruction::startExecution(const int& clockCycle) {
  if (this->executionStart != -1) {
    return false;
  }

  this->executionStart = clockCycle;
  return true;
}

bool Instruction::hasStarted() { return this->executionStart > -1; }

int Instruction::getExecutionStart() { return this->executionStart; }

bool Instruction::finishExecution(const int& clockCycle) {
  if (this->executionEnd != -1) {
    return false;
  }

  this->executionEnd = clockCycle;
  return true;
}

bool Instruction::hasFinished() { return this->executionEnd > -1; }

bool Instruction::isExecuting() {
  return this->hasStarted() && !this->hasFinished();
}

OpCode Instruction::getOpCode() { return this->opCode; }

string Instruction::getOpCodeString() {
  switch (this->opCode) {
    case OpCode::LOAD:
      return "LD";
    case OpCode::ADD:
      return "ADD";
    case OpCode::SUB:
      return "SUB";
    case OpCode::MUL:
      return "MUL";
    case OpCode::DIV:
      return "DIV";
    default:
      return "INVALID";
  }
}

string Instruction::getDestiny() { return this->destiny; }

void Instruction::setDestiny(const string& destiny) { this->destiny = destiny; }

string Instruction::getSrc1() { return this->src1; }

void Instruction::setSrc1(const string& src1) { this->src1 = src1; }

string Instruction::getSrc2() { return this->src2; }

void Instruction::setSrc2(const string& src2) { this->src2 = src2; }

string Instruction::toString() {
  return this->getOpCodeString() + " " + this->destiny + " " + this->src1 +
         " " + this->src2;
}
