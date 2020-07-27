#include <cstdio>
#include <iostream>
#include "bitvector.h"

BitVector::BitVector(int sz)
{
  int i;

  size = sz;
  ui_size = sizeof(unsigned int) * 8;

  wordWidth = (size + ui_size -1)/ui_size;
  bv =  new int[wordWidth];
  //set all bits to zero
  //for (i = 0; i < wordWidth; i++) *(bv + i) = 0;  //bitwise operations
  for (i = 0; i < wordWidth; i++) bv[i] = 0;  
}

BitVector::~BitVector()
{
  delete [] bv;
}

void BitVector::setBit(int pos)
{
  //first figure out the index to the integer array
  int index = pos/ui_size;
  //next which bit in the integer
  int offset = pos % ui_size;

  //*(bv + index) |= (ON << offset);
  //new use the OR assignment (|=) to set that bit, while leaving the
  // other bits as there were.
  bv[index] |= (ON << offset);
}

void BitVector::resetBit(int pos)
{
  //first figure out the index to the integer array
  int index = pos/ui_size;
  //next which bit in the integer
  int offset = pos % ui_size;

  //*(bv + index) &= (~(ON << offset));
  //seting value to 0 instead of 1, so this time using AND
  // the value into it, using a negated bit
  // ie  01000  as 10111  to set bit 2 to zero, while leaving the rest unchanged.
  bv[index] &= (~(ON << offset));
}

int BitVector::testBit(int pos)
{
  //first figure out the index to the integer array
  int index = pos/ui_size;
  //next which bit in the integer
  int offset = pos % ui_size;
  
//  return(*(bv + index) & (ON << offset));
  //returns 0 if bit is set to zero, some other positive number if set to 1
  //return(   bv[index] & (ON << offset)  );
  if (   bv[index] & (ON << offset)  )
     return ON;  //returns 1;
  else 
     return OFF;  //returns zero bit
}

void BitVector::setBitVector(unsigned int *nbv)
{
  //for final project, should be called with something like this
  //char buffer[64];
  //code here to get bitvector off the disk
  //var.setBitVector((unsigned int *)buffer);
  //it's ugly, but we set the bitvector based off the data in
  //on the "partition" which is read as a 64K block.
  //
  for (int i = 0; i < wordWidth; i++) 
      //*(bv + i) = *(nbv + i);
     bv[i] = nbv[i];
}

void BitVector::getBitVector(unsigned int *nbv)
{
  //There is a bad assumation here, the nbv var actually has
  //enough memory allocated to do what we are doing, but no real way to verify 
  //it either!
  //for final project, should be called with something like this
  //char buffer[64];
  //var.getBitVector((unsigned int *)buffer);
  //on return the buffer now has the bit vector, ugly, but works nicely.
  for (int i = 0; i < wordWidth; i++) 
     nbv[i] = bv[i];
}
