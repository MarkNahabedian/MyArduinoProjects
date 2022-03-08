// Code to read an iGaging DRO like the Easy-view 35-706-P+.

#include "iGaging.h"

void DRO_iGaging::setupDROReadHead() {
  pinMode(this->clockPin, INPUT);
  pinMode(this->reqadPin, OUTPUT);
  pinMode(this->dataPin, INPUT);
}


void DRO_iGaging::request() {
}


byte readBit() {
}


