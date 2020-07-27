#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include "bitvector.h"
using namespace std;

/* This is a test file to test how the bitvector class works.
   It's not needed for the project, only for testing bitvector stuff
*/

void prtbv (unsigned int *bv, int length) {

  for (int i=0; i<length; i++) {
     if (*bv & (1 << i) ) {
       cout <<"1";
     } else {
       cout <<"0";
     }
  }
  cout <<"\n";

}

int main() {

unsigned int val1, val2;
char buffer[64];

BitVector bv1(32), bv2(8);

val1 = 0;

  bv1.setBit(1);  //set position 1 to a 1
  bv1.setBit(2);  //set position 2 to a 1
  bv1.resetBit(2); //set position 2 to a 0
  bv1.setBit(31);  //set position 31 to a 1
  bv1.resetBit(30);  //set position 30 to a 0
//print out the bit vector
  for (int i=0; i<32; i++) {
    if (bv1.testBit(i) ==0) {
       cout <<"0";
    } else {
       cout <<"1";
    }
  }

  cout <<"\n";
  //val1 = bv1.getBitVector();

//print out the bit vector in a different way
  bv1.getBitVector((unsigned int *)buffer);
  prtbv((unsigned int *)buffer,32);

  return 0;

}