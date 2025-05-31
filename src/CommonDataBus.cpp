#include "CommonDataBus.hpp"

CommonDataBus::CommonDataBus() : data(nullptr) {}

pair<int, int> CommonDataBus::execute(
    int &loadBufferIndex,
    std::shared_ptr<std::vector<std::shared_ptr<Buffer>>> loadBuffers,
    int rsBufferIndex,
    std::shared_ptr<std::vector<std::shared_ptr<Buffer>>> reservationStations,
    shared_ptr<unordered_map<string, shared_ptr<Register>>> &registers,

    const int &clockCycle) {
  for (int i = rsBufferIndex;
       i < reservationStations->size() && this->data == nullptr; ++i) {
    this->data = reservationStations->at(i)->execute(clockCycle);
  }

  for (int j = loadBufferIndex;
       j < loadBuffers->size() && this->data == nullptr; ++j) {
    this->data = loadBuffers->at(j)->execute(clockCycle);
  }

  if (this->data == nullptr) {
    return make_pair(loadBufferIndex, rsBufferIndex);
  }

  auto data = *this->data;
  this->data = nullptr;

  for (auto &lb : *loadBuffers) {
    lb->execute(data.first, data.second, clockCycle);
  }

  for (auto &rs : *reservationStations) {
    rs->execute(data.first, data.second, clockCycle);
  }

  for (auto &i : *registers) {
    if (i.second->getName() != data.first) {
      continue;
    }

    i.second->setValue(data.second);
  }

  return make_pair(loadBufferIndex, rsBufferIndex);
}
