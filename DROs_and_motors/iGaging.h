// Code to read an iGaging DRO like the Easy-view 35-706-P+.

// typedef unsigned char byte;
#import <Arduino.h>

struct DROData {
  // String id;
  union {
    byte rawdata[13];
    struct {
      byte leader[4];
      byte sign;
      byte position[6];
      byte decimal;
      byte units;
    } decoded;
  };
};


class AbstractDRO {
 public:
  virtual void setupDROReadHead();
  virtual void request();
  virtual byte readBit();
  void readraw(DROData* data);
};


class testDRO: public AbstractDRO {
  int read_index;       // in bits
  byte testbytes[13];

 public:
  void setupDROReadHead();
  void request();
  byte readBit();
  testDRO(byte data[13]);
};


class DRO_iGaging: public AbstractDRO {
  // String id;
  int clockPin;    // Digital input
  int reqadPin;    // Digital output
  int dataPin;     // Digital input

 public:
  void setupDROReadHead();
  void request();
  byte readBit();
};
