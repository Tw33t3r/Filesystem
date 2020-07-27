#include "bitvector.h"
#include "disk.h"
#include "diskmanager.h"
#include <cstdlib>
#include <iostream>
#include <string.h>

using namespace std;

int DiskManager::charToInt(int pos, char * buff) {
    if(buff == NULL || pos < 0 || pos >= strlen(buff)-3)
    {
        cout << "Error in charToInt arguments" << endl;
        return 0;
    }
    int value=0;
    for(int i = 0; i < 4; i++)
    {
        if(!*buff)
        {
            cout << "error in !*buff" << endl;
            break;   
        }
        if(buff[pos+i] < '0' || buff[pos+i]>'9')
        {
            cout << "error not a number" << endl;
            break;
        }
        value = (value * 10) + (buff[pos+i] - '0');
    }

}


void DiskManager::intToChar(int pos, int num, char * buff) {
    int temp;
    for(int i = 3; i >= 0; i--)
    {
        temp = num %10;
        num/=10;
        char c = temp + '0';
        buff[pos+i] = c;
    }
}

DiskManager::DiskManager(Disk *d, int partcount, DiskPartition *dp)
{
  myDisk = d;
  partCount= partcount;
  int r = myDisk->initDisk();
  char buffer[64] = {'0'};

  diskP = new DiskPartition[partCount];
  diskP = dp;
  /* If needed, initialize the disk to keep partition information */
  if (r==1){
    //cout << "initializing disk" << endl;
    //Edit superblock to reflect new partitions from dp
    int partitionBegin = 1;
    for(int i = 0; i < partcount; i++){
        int nameAddr = i*9;
        buffer[nameAddr] = diskP[i].partitionName;
        //cout << "Adding " << diskP[i].partitionName << endl;
        intToChar(nameAddr+1, partitionBegin, buffer);
        int partitionEnd = (diskP[i].partitionSize + partitionBegin);
        intToChar(nameAddr+5, partitionEnd, buffer);
        partitionBegin = partitionEnd+1;
    }
        myDisk->writeDiskBlock(0, buffer);
  }
  /* else  read back the partition information from the DISK1 */
  else{
      char * superBlock;
      myDisk->readDiskBlock(0, superBlock);
      int begin;
      int end;
      for(int i=0; i < partcount; i++){
          int nameAddr = i*9;
          diskP[i].partitionName = superBlock[nameAddr];
          begin = charToInt(nameAddr+1,superBlock);
          end = charToInt(nameAddr+5,superBlock);
          diskP[i].partitionSize = end-begin;  
    }
  }
}

/*
 *   reads partition block blknum into buffer pointed to by blkdata
 *   returns: 
 *   0, if the block is successfully read;
 *  -1, if disk can't be opened; (same as disk)
 *  -2, if blknum is out of bounds; (same as disk)
 *  -3 if partition doesn't exist
 */
int DiskManager::readDiskBlock(char partitionname, int blknum, char *blkdata)
{
  /* write the code for reading a disk block from a partition */
  //cout << "In readDiskBlock partCount is: " << partCount << endl;
  char superBlock[64] = {'0'};
  myDisk->readDiskBlock(0, superBlock);
  for(int i=0; i < partCount; i++){
    int nameAddr = i*9;
    char name = superBlock[nameAddr];
    if(name == partitionname){
      int begin = charToInt(nameAddr+1,superBlock);
      int diskBlkNum = begin + blknum;
      //cout << "diskBlknum "<< diskBlkNum << endl;
      int toreturn = myDisk->readDiskBlock(diskBlkNum, blkdata);
      //cout << "toreturn " <<  toreturn << endl;
      //cout << "read: " << blkdata << " from: " << diskBlkNum << endl;
      return toreturn;
    }
  }
  return -3;
}


/*
 *   returns: 
 *   0, if the block is successfully written;
 *  -1, if disk can't be opened; (same as disk)
 *  -2, if blknum is out of bounds;  (same as disk)
 *  -3 if partition doesn't exist
 */
int DiskManager::writeDiskBlock(char partitionname, int blknum, char *blkdata)
{
  /* write the code for writing a disk block to a partition */
  //cout << "In writeDiskBlock partCount is: " << partCount << endl;
  char superBlock[64] = {'0'};
  myDisk->readDiskBlock(0, superBlock);
  for(int i=0; i < partCount; i++){
    int nameAddr = i*9;
    char name = superBlock[nameAddr];
    if(name == partitionname){
      int begin = charToInt(nameAddr+1,superBlock);
      int diskBlkNum = begin + blknum;
      //cout << "diskBlknum "<< diskBlkNum << endl;
      int toreturn = myDisk->writeDiskBlock(diskBlkNum, blkdata);
      //cout << "toreturn " <<  toreturn << endl;
      //cout << "wrote: " << blkdata << " to: " << diskBlkNum << endl;
      return toreturn;
    }
  }
  return -3;
}

/*
 * return size of partition
 * -1 if partition doesn't exist.
 */
int DiskManager::getPartitionSize(char partitionname)
{
  /* write the code for returning partition size */
  for(int i = 0; i < partCount;  i++)
  {
    if(diskP[i].partitionName == partitionname)
    {
      return diskP[i].partitionSize+1;
    }
  }
  return -1;
}
