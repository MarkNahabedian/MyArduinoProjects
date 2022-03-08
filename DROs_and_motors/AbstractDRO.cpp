#include "iGaging.h"

// *** We don't know if each nibble in the bit serialization is sent
// *** lsb or msb first.
byte databits[] = { 1, 2, 4, 8 };   // Or maybe the other way around

void AbstractDRO::readraw(DROData* data) {
  byte b = 0;
  for (int bitnum = 0;
       bitnum < 4 * sizeof(data->rawdata);
       bitnum++) {
    if ((bitnum % 4) == 0) {
      data->rawdata[bitnum >> 2] = b;
      b = 0;
    }
    b |= this->readBit() * databits[bitnum % 4];
  }
}

