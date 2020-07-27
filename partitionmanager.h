#include "bitvector.h"

class PartitionManager {
  DiskManager *myDM;
  BitVector *dmBV;


  public:
    int testBlock(int blk);
    void setBlock(int blk);
    char myPartitionName;
    int myPartitionSize;
    PartitionManager(DiskManager *dm, char partitionname, int partitionsize);
    ~PartitionManager();
    int readDiskBlock(int blknum, char *blkdata);
    int writeDiskBlock(int blknum, char *blkdata);
    int getBlockSize();
    int getFreeDiskBlock();
    int returnDiskBlock(int blknum);
};