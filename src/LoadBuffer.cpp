#include "LoadBuffer.hpp"

#include <Register.hpp>
#include <map>

#include "ReservationStation.hpp"

LoadBuffer::LoadBuffer(const int& id) : name("LD" + std::to_string(id)) {}

bool LoadBuffer::set(const shared_ptr<Instruction>& instruction,
                     const string& tagSrc1, const int& valueSrc1,
                     const string& tagSrc2, const int& valueSrc2,
                     const int& issue) {
  return this->set(instruction, tagSrc1, valueSrc1, issue);
}

bool LoadBuffer::set(const shared_ptr<Instruction>& instruction,
                     const string& tag, const int& value, const int& issue) {
  if (!this->available) {
    return false;
  }

  this->tag = tag;
  this->value = value;

  this->instruction = instruction;
  this->instruction->setIssue(issue);
  if (value != -1) {
    this->instruction->startExecution(issue);
  }

  this->passsedClockCycles = 0;
  this->available = false;
  return true;
}

bool LoadBuffer::execute(const string& tag, const int& value,
                         const int& clockCycle) {
  if (this->available || this->instruction->isExecuting() ||
      this->instruction->hasFinished() || this->tag != tag) {
    return false;
  }

  this->value = value;
  this->instruction->startExecution(clockCycle);
  return true;
}

shared_ptr<pair<string, int>> LoadBuffer::execute(const int& clockCycle) {
  if (this->available || !this->instruction->isExecuting() ||
      this->instruction->getExecutionStart() == clockCycle) {
    return nullptr;
  }

  ++this->passsedClockCycles;
  if (this->passsedClockCycles < REQUIRED_CLOCK_CYCLES) {
    return nullptr;
  }

  this->available = true;
  return make_shared<pair<string, int>>(make_pair(this->name, this->value));
}

bool LoadBuffer::isAvailable() { return this->available; }

string LoadBuffer::getName() { return this->name; }