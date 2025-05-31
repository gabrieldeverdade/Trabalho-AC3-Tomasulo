#ifndef RESERVATION_STATION_HPP
#define RESERVATION_STATION_HPP

#include <map>
#include <string>

#include "Buffer.hpp"
#include "Instruction.hpp"
#include "Register.hpp"

using namespace std;

enum class OpCodeCycles { ADD = 2, SUB = 2, MUL = 10, DIV = 40, INVALID };

class ReservationStation : public Buffer {
 private:
  int id;
  string name;
  bool available = true;

  shared_ptr<Instruction> instruction = nullptr;
  string tagSrc1;
  int valueSrc1 = -1;

  string tagSrc2;
  int valueSrc2 = -1;

  int passsedClockCycles = 0;
  int getOpCodeCycles();

 public:
  ReservationStation(const int& id);
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

#endif  // RESERVATION_STATION_HPP