
#include "iGaging.h"


testDRO::testDRO(byte data[13]) {
  this->read_index = 0;
  for (int i = 0; i < 13; i++) {
    this->testbytes[i] = data[i];
  }
}

void testDRO::setupDROReadHead() {
  // Nosetup required
}

void testDRO::request() {
  this->read_index = 0;
}

byte testDRO::readBit() {
  if (this->read_index >= 13 * 4)
    return 0;
  int nibblenum = this->read_index / 4;
  int b = this->testbytes[nibblenum];
  int bitnum = this->read_index & 3;
  this->read_index++;
  return (b >> bitnum) & 1;
}


