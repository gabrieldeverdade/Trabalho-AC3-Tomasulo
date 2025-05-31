#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <deque>
#include <memory>
#include <string>

using namespace std;

// Enum para operações suportadas
enum class OpCode { ADD, SUB, MUL, DIV, LOAD, STORE, INVALID };

// Representa uma instrução com campos para marcação de ciclos
class Instruction {
 private:
  OpCode opCode;
  string destiny, src1, src2;
  int issue = -1;
  int executionStart = -1;
  int executionEnd = -1;
  int writeBack = -1;

  static OpCode stringToOpCode(const string& opStr);

 public:
  Instruction(const OpCode& opCode, const string& destiny,
              const string& source1, const string& source2);

  Instruction(const string& opStr, const string& destiny, const string& source1,
              const string& source2);

  static shared_ptr<deque<shared_ptr<Instruction>>> fromInstructionFile(
      const string& path);
  static shared_ptr<Instruction> fromLine(const string& line);

  void setIssue(const int& issue);

  bool startExecution(const int& clockCycle);
  bool hasStarted();
  int getExecutionStart();

  bool finishExecution(const int& clockCycle);
  bool hasFinished();

  bool isExecuting();

  OpCode getOpCode();
  string getOpCodeString();

  string getDestiny();
  void setDestiny(const string& destiny);
  string getSrc1();
  void setSrc1(const string& src1);
  string getSrc2();
  void setSrc2(const string& src2);

  string toString();
};

#endif  // INSTRUCTION_HPP