#include <iostream>

#include "CommonDataBus.hpp"
#include "Instruction.hpp"
#include "LoadBuffer.hpp"
#include "Register.hpp"
#include "ReservationStation.hpp"

bool hasBusyBuffer(shared_ptr<vector<shared_ptr<Buffer>>>& buffers);
shared_ptr<Buffer> findEmptyBuffer(
    shared_ptr<vector<shared_ptr<Buffer>>>& buffers);
bool assignBuffer(
    shared_ptr<vector<shared_ptr<Buffer>>>& loadBuffers,
    shared_ptr<vector<shared_ptr<Buffer>>>& reservationStations,
    shared_ptr<Instruction>& instruction,
    shared_ptr<unordered_map<string, shared_ptr<Register>>>& registers,
    const int& clockCycle);

bool assignBuffer(
    shared_ptr<vector<shared_ptr<Buffer>>>& buffers,
    shared_ptr<Instruction>& instruction,
    shared_ptr<unordered_map<string, shared_ptr<Register>>>& registers,
    const int& clockCycle);

int main(int argc, char* argv[]) {
  // Default instruction file
  string instructionFile = "instrucoes.txt";
  auto instructionQueue = make_shared<deque<shared_ptr<Instruction>>>();

  for (int i = 1; i < argc; i++) {
    instructionQueue->push_back(Instruction::fromLine(string(argv[i])));
    cout << instructionQueue->back()->toString() << endl;
  }

  if (instructionQueue->empty()) {
    return 1;
  }

  auto registers = Register::fromInstructions(instructionQueue);
  for (auto& instruction : *instructionQueue) {
    cout << instruction->toString() << endl;
  }

  auto loadBuffers = make_shared<vector<shared_ptr<Buffer>>>();
  auto reservationStations = make_shared<vector<shared_ptr<Buffer>>>();
  auto commonDataBuses = make_shared<vector<shared_ptr<CommonDataBus>>>();

  for (int i = 0; i < 2; i++) {
    loadBuffers->push_back(make_shared<LoadBuffer>(i));
    reservationStations->push_back(make_shared<ReservationStation>(i));
    commonDataBuses->push_back(make_shared<CommonDataBus>());
  }

  bool isBusy = false;
  int clockCycle = 0;
  do {
    vector<shared_ptr<CommonDataBus>> emptyDataBuses = *commonDataBuses;
    int loadBufferIndex = 0, rsBufferIndex = 0;
    while (!emptyDataBuses.empty()) {
      auto edb = emptyDataBuses.back();
      auto result = edb->execute(loadBufferIndex, loadBuffers, rsBufferIndex,
                                 reservationStations, registers, clockCycle);
      loadBufferIndex = result.first;
      rsBufferIndex = result.second;
      emptyDataBuses.pop_back();
    }

    bool success = false;
    if (!instructionQueue->empty()) {
      success = assignBuffer(loadBuffers, reservationStations,
                             instructionQueue->front(), registers, clockCycle);
    }

    if (success) {
      instructionQueue->pop_front();
    }

    isBusy = hasBusyBuffer(loadBuffers) || hasBusyBuffer(reservationStations);
    ++clockCycle;
  } while (isBusy || !instructionQueue->empty());

  for (auto& i : *registers) {
    cout << i.first << " " << i.second->getValue() << endl;
  }

  return 0;
}

bool hasBusyBuffer(shared_ptr<vector<shared_ptr<Buffer>>>& buffers) {
  for (auto& buffer : *buffers) {
    if (!buffer->isAvailable()) {
      return true;
    }
  }

  return false;
}

bool assignBuffer(
    shared_ptr<vector<shared_ptr<Buffer>>>& loadBuffers,
    shared_ptr<vector<shared_ptr<Buffer>>>& reservationStations,
    shared_ptr<Instruction>& instruction,
    shared_ptr<unordered_map<string, shared_ptr<Register>>>& registers,
    const int& clockCycle) {
  switch (instruction->getOpCode()) {
    case OpCode::LOAD:
      return assignBuffer(loadBuffers, instruction, registers, clockCycle);
    case OpCode::ADD:
    case OpCode::SUB:
    case OpCode::MUL:
    case OpCode::DIV:
      return assignBuffer(reservationStations, instruction, registers,
                          clockCycle);
    default:
      return false;
  }
}

bool assignBuffer(
    shared_ptr<vector<shared_ptr<Buffer>>>& buffers,
    shared_ptr<Instruction>& instruction,
    shared_ptr<unordered_map<string, shared_ptr<Register>>>& registers,
    const int& clockCycle) {
  shared_ptr<Buffer> emptyBuffer = findEmptyBuffer(buffers);
  if (emptyBuffer == nullptr) {
    return false;
  }

  string src1 = instruction->getSrc1();
  int src1Value = -1;
  if (src1.starts_with("R")) {
    auto i = registers->find(src1);
    shared_ptr<Register> referencedRegister = i->second;
    string name = referencedRegister->getName();
    if (name.empty()) {
      src1Value = referencedRegister->getValue();
    } else {
      src1 = name;
    }

  } else {
    src1Value = stoi(src1);
  }

  string src2 = instruction->getSrc1();
  int src2Value = -1;
  if (src2.starts_with("R")) {
    auto i = registers->find(src2);
    shared_ptr<Register> referencedRegister = i->second;
    string name = referencedRegister->getName();
    if (name.empty()) {
      src2Value = referencedRegister->getValue();
    } else {
      src2 = name;
    }

  } else {
    src2Value = stoi(src2);
  }

  bool success = emptyBuffer->set(instruction, src1, src1Value, src2, src2Value,
                                  clockCycle);

  string destiny = instruction->getDestiny();
  auto destinyRegister = registers->find(destiny);
  destinyRegister->second->setName(emptyBuffer->getName());

  return success;
}

shared_ptr<Buffer> findEmptyBuffer(
    shared_ptr<vector<shared_ptr<Buffer>>>& buffers) {
  for (auto& buffer : *buffers) {
    if (buffer->isAvailable()) {
      return buffer;
    }
  }
  return nullptr;
}
