#ifndef REGISTER_HPP
#define REGISTER_HPP

#include <set>
#include <string>
#include <unordered_map>

#include "Instruction.hpp"

using namespace std;

// Representa um registrador
class Register {
 private:
  string name;
  int value = -1;

 public:
  Register() : name("") {}
  Register(string name);
  static shared_ptr<unordered_map<string, shared_ptr<Register>>>
  fromInstructions(shared_ptr<deque<shared_ptr<Instruction>>>& instructions);

  string getName();
  void setName(const string& name);

  bool setValue(const int& value);
  int getValue();
};

#endif