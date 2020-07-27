#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "disk.h"

using namespace std;

Disk::Disk(int numblocks, int blksz, char *fname)
{
  blkCount = numblocks;
  diskSize = numblocks * blksz;
  blkSize = blksz;
  diskFilename = strdup(fname);
}

Disk::~Disk()
{
}

int Disk::initDisk()
{
  fstream f(diskFilename, ios::in);
  if (!f) {
    f.open(diskFilename, ios::out);
    if (!f) {
      cerr << "Error: Cannot create disk file" << endl;
      return(-1);
    }
    for (int i = 0; i < diskSize; i++) f.put('c');
    f.close();
    return(1);
  }
  f.close();
  return 0 ;
}

int Disk::readDiskBlock(int blknum, char *blkdata)
/*
  returns -1, if disk can't be opened;
  returns -2, if blknum is out of bounds;
  returns 0, if the block is successfully read;
*/
{
  if ((blknum < 0) || (blknum >= blkCount)) return(-2);
  ifstream f(diskFilename, ios::in);
  if (!f) return(-1);
  f.seekg(blknum * blkSize);
  f.read(blkdata, blkSize);
  f.close();
  return(0);
}

int Disk::writeDiskBlock(int blknum, char *blkdata)
/*
  returns -1, if DISK can't be opened;
  returns -2, if blknum is out of bounds;
  returns 0, if the block is successfully read;
*/
{
  if ((blknum < 0) || (blknum >= blkCount)) return(-2);
  fstream f(diskFilename, ios::in|ios::out);
  if (!f) return(-1);
  f.seekg(blknum * blkSize);
  f.write(blkdata, blkSize);
  f.close();
  return(0);
}
