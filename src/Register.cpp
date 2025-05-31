#include "Register.hpp"

#include <unordered_map>
#include <vector>

Register::Register(string name) : name(name) {}

shared_ptr<unordered_map<string, shared_ptr<Register>>>
Register::fromInstructions(
    shared_ptr<deque<shared_ptr<Instruction>>>& instructions) {
  auto registers = make_shared<unordered_map<string, shared_ptr<Register>>>();
  auto replacedRegisters =
      make_unique<unordered_map<string, shared_ptr<Register>>>();

  int i = 100;
  for (shared_ptr<Instruction>& instruction : *instructions) {
    string destiny = instruction->getDestiny();
    if (destiny.empty()) {
      return nullptr;
    }

    if (registers->contains(destiny)) {
      (*replacedRegisters)[destiny] = (*registers)[destiny];

      string newName = "R" + to_string(i);
      (*registers)[destiny] = make_shared<Register>(newName);
      instruction->setDestiny(newName);
      ++i;
    } else {
      (*registers)[destiny] = make_shared<Register>(destiny);
    }

    string src1 = instruction->getSrc1();
    bool src1IsRegister = src1.starts_with("R");
    if (src1IsRegister && !registers->contains(src1)) {
      (*registers)[destiny] = make_shared<Register>(destiny);
    } else if (src1IsRegister) {
      instruction->setSrc1((*registers)[src1]->name);
    }

    string src2 = instruction->getSrc2();
    bool src2IsRegister = src2.starts_with("R");
    if (src2IsRegister && !registers->contains(src2)) {
      (*registers)[destiny] = make_shared<Register>(destiny);
    } else if (src2IsRegister) {
      instruction->setSrc2((*registers)[src2]->name);
    }
  }

  for (auto& item : (*registers)) {
    if (item.first == item.second->name) {
      (*replacedRegisters)[item.first] = item.second;
      continue;
    }

    (*replacedRegisters)[item.second->name] = item.second;
  }

  return replacedRegisters;
}

string Register::getName() { return this->name; }

void Register::setName(const string& name) { this->name = name; }

bool Register::setValue(const int& value) {
  this->value = value;
  return true;
}

int Register::getValue() { return this->value; }
