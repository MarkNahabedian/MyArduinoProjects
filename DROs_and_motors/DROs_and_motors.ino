#include "iGaging.h"

byte d1[] = {
  0xf, 0xf, 0xf, 0xf,             // leader
  0x8,                            // sign
  0x1, 0x2, 0x3, 0x4, 0x5, 0x6,   // digits
  0x2,                            // decimal location
  0x1
};
testDRO* t1 = new testDRO(d1);

String nibblearray2str(byte a[], int length) {
  String s = "";
  for (int i = 0; i < length; i++) {
    byte nibble = a[i];
    char c = "0123456789abcdef"[nibble];
    s += c;
  }
  return s;
}


void setup() {
  Serial.begin(9600);

  // Test nibblearray2str:
  byte testnibbles[] = { 0x1, 0x2, 0x3, 0x4 };
  Serial.println(nibblearray2str(testnibbles, 4));

  // Test testDRO.readBit
  String bitstring = "";
  t1->request();
  for (int i = 0; i < 13 * 4; i++) {
    if (i % 4 == 0)
      bitstring += " ";
    byte b = t1->readBit();
    bitstring += (b == 0) ? "0" :"1";
  }
  Serial.println(bitstring);

  t1->request();
  DROData d;
  t1->readraw(&d);
  Serial.print("\n\n");
  Serial.println(nibblearray2str(d.decoded.leader,
                                 sizeof(d.decoded.leader)));
  Serial.println(d.decoded.sign);
  Serial.println(nibblearray2str(d.decoded.position,
                                 sizeof(d.decoded.position)));
  Serial.println(d.decoded.decimal);
  Serial.println(d.decoded.units);
}

void loop() {
}

