#include "disk.h"
#include "diskmanager.h"
#include "partitionmanager.h"
#include <iostream>
using namespace std;

//ADDED


PartitionManager::PartitionManager(DiskManager *dm, char partitionname, int partitionsize)
{
  myDM = dm;
  myPartitionName = partitionname;
  myPartitionSize = myDM->getPartitionSize(myPartitionName);

  /* If needed, initialize bit vector to keep track of free and allocted
     blocks in this partition */
  dmBV = new BitVector(myPartitionSize);
  //set block to size 64 and all 0's
  char buffer[64] = {'0'};
  char temp[64] = {'0'};
  readDiskBlock(0,buffer);
  if(buffer[1] == 'c'){
    dmBV->setBitVector((unsigned int *)temp);
  }
  else{
    dmBV->setBitVector((unsigned int *)buffer);
  }
  //set first bit as full
  dmBV->setBit(0);
  dmBV->setBit(1);
  dmBV->getBitVector((unsigned int *)temp);
  //write bitvector to first block
  //cout << "PMCon writeDiskBlock " << buffer << " to " << 0 << endl;
  writeDiskBlock(0, temp);
}

PartitionManager::~PartitionManager()
{
  delete dmBV;
}

/*
 * Allocates free disk block in a partition
 * return -1 if none available, else returns blocknum Allocated
 */
int PartitionManager::getFreeDiskBlock()
{
  /* write the code for allocating a partition block */
  //Check each block in the partition, starting at 1
  for(int blocknum = 1; blocknum < myPartitionSize; blocknum++){
    //If the block is free
    if(dmBV->testBit(blocknum) == 0){
      //Mark it as unavailable
      dmBV->setBit(blocknum);
      //Fill the buffer with 0's
      char buffer[64] = "";
      writeDiskBlock(blocknum, buffer);
      dmBV->getBitVector((unsigned int *)buffer);
      //Update the disk manager
      writeDiskBlock(0, buffer);
      //Return the block number
      return blocknum;
    }
  }
  //If we are here, there are no blocks available
  return -1;
}

/*
 * deallocates a disk block in a partition
 * write a blank (all c's) to the block returned.
 * return 0 for sucess, -1 otherwise
 */
int PartitionManager::returnDiskBlock(int blknum)
{
  /* write the code for deallocating a partition block */
  dmBV->resetBit(blknum);
  //Make data equal '0' of whatever length a partition block is
  char data [64] = {'c'};
  for(int i=0; i < 64; i++){
    data[i] = 'c';
  }
  int returnVal = writeDiskBlock(blknum, data);
  char buffer[64] = {'0'};
  dmBV->getBitVector((unsigned int *)buffer);
  //Update the disk manager
  writeDiskBlock(0, buffer);
  if(returnVal == 0){
    return 0;
  }
  else{
    return -1;
  }
}


int PartitionManager::readDiskBlock(int blknum, char *blkdata)
{
  int toReturn = myDM->readDiskBlock(myPartitionName, blknum, blkdata);
  //cout << "PM-Read: " << blkdata << " from: " << blknum << endl;
  return toReturn;
}

int PartitionManager::writeDiskBlock(int blknum, char *blkdata)
{
  int toReturn = myDM->writeDiskBlock(myPartitionName, blknum, blkdata);
  //cout << "PM-Write: " << blkdata << " to: " << blknum << endl;
  return toReturn;
}

int PartitionManager::getBlockSize() 
{
  return myDM->getBlockSize();
}