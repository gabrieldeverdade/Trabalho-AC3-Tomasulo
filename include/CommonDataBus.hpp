#ifndef COMMON_DATA_BUS_HPP
#define COMMON_DATA_BUS_HPP

#include <queue>

#include "Buffer.hpp"
#include "Instruction.hpp"
#include "Register.hpp"

struct CommonDataBus {
  shared_ptr<pair<string, int>> data;

  CommonDataBus();
  pair<int, int> execute(
      int &loadBufferIndex, shared_ptr<vector<shared_ptr<Buffer>>> loadBuffers,
      int rsBufferIndex,
      shared_ptr<vector<shared_ptr<Buffer>>> reservationStations,
      shared_ptr<unordered_map<string, shared_ptr<Register>>> &registers,
      const int &clockCycle);
};

#endif  // COMMON_DATA_BUS_HPP