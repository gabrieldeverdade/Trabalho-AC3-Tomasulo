#include "ReservationStation.hpp"
ReservationStation::ReservationStation(const int& id) : id(id), name("") {}

bool ReservationStation::set(const shared_ptr<Instruction>& instruction,
                             const string& tagSrc1, const int& valueSrc1,
                             const string& tagSrc2, const int& valueSrc2,
                             const int& issue) {
  if (!this->available) {
    return false;
  }

  this->tagSrc1 = tagSrc1;
  this->valueSrc1 = valueSrc1;
  this->tagSrc2 = tagSrc2;
  this->valueSrc2 = valueSrc2;

  this->instruction = instruction;
  this->instruction->setIssue(issue);
  if (this->valueSrc1 != -1 && this->valueSrc2 != -1) {
    this->instruction->startExecution(issue);
  }

  this->name = instruction->getOpCodeString() + to_string(this->id);
  this->passsedClockCycles = 0;
  this->available = false;
  return true;
}

int ReservationStation::getOpCodeCycles() {
  if (this->instruction->getOpCode() == OpCode::ADD ||
      this->instruction->getOpCode() == OpCode::SUB) {
    return (int)OpCodeCycles::ADD;
  } else if (this->instruction->getOpCode() == OpCode::MUL) {
    return (int)OpCodeCycles::MUL;
  } else if (this->instruction->getOpCode() == OpCode::DIV) {
    return (int)OpCodeCycles::DIV;
  }

  return (int)OpCodeCycles::INVALID;
}

bool ReservationStation::execute(const string& tag, const int& value,
                                 const int& clockCycle) {
  if (this->available || this->instruction->isExecuting() ||
      this->instruction->hasFinished() ||
      (this->tagSrc1 != tag && this->tagSrc2 != tag)) {
    return false;
  }

  if (this->tagSrc1 == tag) {
    this->valueSrc1 = value;
  } else {
    this->valueSrc2 = value;
  }

  if (this->valueSrc1 == -1 || this->valueSrc2 == -1) {
    return false;
  }

  this->instruction->startExecution(clockCycle);
  return true;
}

shared_ptr<pair<string, int>> ReservationStation::execute(
    const int& clockCycle) {
  if (this->available || !this->instruction->isExecuting() ||
      this->instruction->getExecutionStart() == clockCycle) {
    return nullptr;
  }

  ++this->passsedClockCycles;
  if (this->passsedClockCycles < this->getOpCodeCycles()) {
    return nullptr;
  }

  this->available = true;
  int returnValue = -1;

  switch (this->instruction->getOpCode()) {
    case OpCode::ADD:
      returnValue = this->valueSrc1 + this->valueSrc2;
      break;
    case OpCode::SUB:
      returnValue = this->valueSrc1 - this->valueSrc2;
      break;
    case OpCode::MUL:
      returnValue = this->valueSrc1 * this->valueSrc2;
      break;
    case OpCode::DIV:
      returnValue = this->valueSrc1 / this->valueSrc2;
      break;
    default:
      return nullptr;
  }

  return make_shared<pair<string, int>>(make_pair(this->name, returnValue));
}

bool ReservationStation::isAvailable() { return this->available; }

string ReservationStation::getName() { return this->name; }
