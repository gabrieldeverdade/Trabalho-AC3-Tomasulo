#ifndef LOADBUFFER_HPP
#define LOADBUFFER_HPP

#include <string>

#include "Buffer.hpp"
#include "Instruction.hpp"
#include "Register.hpp"

using namespace std;

class LoadBuffer : public Buffer {
 private:
  string name;
  bool available = true;

  shared_ptr<Instruction> instruction = nullptr;
  string tag;
  int value = -1;

  int passsedClockCycles = 0;
  const static int REQUIRED_CLOCK_CYCLES = 2;

  bool set(const shared_ptr<Instruction>& instruction, const string& tag,
           const int& value, const int& issue);

 public:
  LoadBuffer(const int& id);
  virtual bool set(const shared_ptr<Instruction>& instruction,
                   const string& tagSrc1, const int& valueSrc1,
                   const string& tagSrc2, const int& valueSrc2,
                   const int& issue) override;

  virtual bool execute(const string& tag, const int& value,
                       const int& clockCycle) override;
  virtual shared_ptr<pair<string, int>> execute(const int& clockCycle) override;

  virtual bool isAvailable() override;
  virtual string getName() override;
};

#endif  // LOADBUFFER_HPP