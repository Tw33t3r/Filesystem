/* Driver 6*/

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "disk.h"
#include "diskmanager.h"
#include "partitionmanager.h"
#include "filesystem.h"
#include "client.h"

using namespace std;

/*
  This driver will test the getAttributes() and setAttributes()
  functions. You need to complete this driver according to the
  attributes you have implemented in your file system, before
  testing your program.
*/

int main()
{

  Disk *d = new Disk(300, 64, const_cast<char *>("DISK1"));
  DiskPartition *dp = new DiskPartition[3];

  dp[0].partitionName = 'A';
  dp[0].partitionSize = 100;
  dp[1].partitionName = 'B';
  dp[1].partitionSize = 75;
  dp[2].partitionName = 'C';
  dp[2].partitionSize = 105;

  DiskManager *dm = new DiskManager(d, 3, dp);
  FileSystem *fs1 = new FileSystem(dm, 'A');
  FileSystem *fs2 = new FileSystem(dm, 'B');
  FileSystem *fs3 = new FileSystem(dm, 'C');
  Client *c1 = new Client(fs1);
  Client *c2 = new Client(fs2);
  Client *c3 = new Client(fs3);
  Client *c4 = new Client(fs1);
  Client *c5 = new Client(fs2);



  int r;


  //What every need to show your set and get Attributes functions work
  r = c1->myFS->setAttributes(const_cast<char *>("/e/f"), 4, 'r', 1000, 2);
  cout << "rv from setAttributes /e/f " << r <<(r>0 ? " correct": " fail")<< endl;
  r = c4->myFS->setAttributes(const_cast<char *>("/e/b"), 4, 'r', 1000, 1);
  cout << "rv from setAttributes /e/b " << r <<(r>0 ? " correct": " fail")<< endl;
  r = c1->myFS->getAttributes(const_cast<char *>("/e/f"), 4, 2);
  cout << "rv from getAttributes /e/f " << r <<(r>0 ? " correct": " fail")<< endl;
  r = c4->myFS->getAttributes(const_cast<char *>("/e/b"), 4, 1);
  cout << "rv from getAttributes /e/b " << r <<(r>0 ? " correct": " fail")<< endl;
  r = c1->myFS->getAttributes(const_cast<char *>("/p"), 2, 4);
  cout << "rv from getAttributes /p " << r <<(r<0 ? " correct wrong parameters": " fail")<< endl;
  r = c4->myFS->setAttributes(const_cast<char *>("/p"), 2, 'g', 1000, 3);  
  cout << "rv from setAttributes /p " << r <<(r<0 ? " correct wrong parameters": " fail")<< endl;

  r = c2->myFS->setAttributes(const_cast<char *>("/f"), 2, 'b', 2000, 3);
  cout << "rv from setAttributes /f " << r <<(r<0 ? " correct does not exist": " fail")<< endl;
  r = c5->myFS->setAttributes(const_cast<char *>("/z"), 2, 'r', 3000, 3);
  cout << "rv from setAttributes /z " << r <<(r>0 ? " correct": " fail")<< endl;
  r = c2->myFS->getAttributes(const_cast<char *>("/f"), 2, 1);
  cout << "rv from setAttributes /f " << r <<(r<0 ? " correct does not exist": " fail")<< endl;
  r = c5->myFS->getAttributes(const_cast<char *>("/z"), 2, 2);
  cout << "rv from setAttributes /z " << r <<(r>0 ? " correct": " fail")<< endl;
  r = c3->myFS->setAttributes(const_cast<char *>("/o/o/o/a/l"), 10, 'r', 1000, 3);
  cout << "rv from setAttributes /o/o/o/a/l " << r <<(r<0 ? " correct cant set a directory": " fail")<< endl;
  r = c3->myFS->setAttributes(const_cast<char *>("/o/o/o/a/d"), 10, 'r', 1000, 3);
  cout << "rv from setAttributes /o/o/o/a/d " << r <<(r<0 ? " correct cant set a directory": " fail")<< endl;
  r = c3->myFS->getAttributes(const_cast<char *>("/o/o/o/a/l"), 10, 1);
  cout << "rv from getAttributes /o/o/o/a/l " << r <<(r<0 ? " correct cant get from a directory": " fail")<< endl;
  r = c3->myFS->getAttributes(const_cast<char *>("o/o/o/a/d"), 10, 2);
  cout << "rv from getAttributes /o/o/o/a/d " << r <<(r<0 ? " correct cant get from a directory": " fail")<< endl;

  cout << endl;
  
  cout << "End of Driver 6, Thanks for the great project Ward!!! :)" << endl;
  return 0;
}
