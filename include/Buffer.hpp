#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "Register.hpp"

class Buffer {
 public:
  virtual bool set(const shared_ptr<Instruction>& instruction,
                   const string& tagSrc1, const int& valueSrc1,
                   const string& tagSrc2, const int& valueSrc2,
                   const int& issue) = 0;
  virtual bool execute(const string& tag, const int& value,
                       const int& clockCycle) = 0;
  virtual shared_ptr<pair<string, int>> execute(const int& clockCycle) = 0;
  virtual bool isAvailable() = 0;
  virtual string getName() = 0;
};

#endif
