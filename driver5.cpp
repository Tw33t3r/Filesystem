
/* Driver 5*/

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "disk.h"
#include "diskmanager.h"
#include "partitionmanager.h"
#include "filesystem.h"
#include "client.h"

using namespace std;

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

  int i, r, l1, l2, f1, f2, f3, f4, f5, f6;
  char buf1[37], buf2[64], buf3[600], buf4[600];
  char rbuf1[37], rbuf2[64], rbuf3[600], rbuf4[600];

  for (i = 0; i < 37; i++) buf1[i] = 'P';
  for (i = 0; i < 64; i++) buf2[i] = 'T';
  for (i = 0; i < 600; i++) buf3[i] = 'F';
  for (i = 0; i < 600; i++) buf4[i] = 'I';


  
  cout <<"Driver 5 begins\n";

  cout <<"Fill a partition test! This assumes driver1-4 were run\n";
  cout <<"if there are minor differences, you will have some serious explaining to do!\n";
  cout <<"Begin filling fs2\n";
  r = c2->myFS->createFile(const_cast<char *>("/o/o/o/a"), 8);
  cout << "rv from createFile /o/o/o/a is " << r <<(r==0 ? " correct": " fail") <<endl;
  f1 = c2->myFS->openFile(const_cast<char *>("/o/o/o/a"), 8, 'w', -1);
  cout << "rv from openFile /o/o/o/a is " << f1 << (f1>0 ? " correct": " fail")<< endl;
  r = c2->myFS->writeFile(f1, buf3, 600);
  cout << "rv from writeFile /o/o/o/b is " << r << (r==600 ? " correct write 600 Fs": " fail")<< endl;
  r = c2->myFS->writeFile(f1, buf4, 600);
  cout << "rv from writeFile /o/o/o/b is " << r << (r==600 ? " correct write 600 Is": " fail")<< endl;
  r = c2->myFS->closeFile(f1);
  cout << "rv from closeFile /o/o/o/b is " << r << (r==0 ? " correct":" fail")<<endl;

  r = c2->myFS->createFile(const_cast<char *>("/o/o/o/b"), 8);
  cout << "rv from createFile /o/o/o/b is " << r <<(r==0 ? " correct": " fail") <<endl;
  f1 = c2->myFS->openFile(const_cast<char *>("/o/o/o/b"), 8, 'w', -1);
  cout << "rv from openFile /o/o/o/b is " << f1 << (f1>0 ? " correct": " fail")<< endl;
  r = c2->myFS->writeFile(f1, buf4, 600);
  cout << "rv from writeFile is " << r << (r==600 ? " correct write 600 Is": " fail")<< endl;
  r = c2->myFS->appendFile(f1, buf2, 64);
  cout << "rv from appendFile /o/o/o/b is " << r <<(r==64 ? " correct write 64 Ts": " fail")<< endl;
  r = c2->myFS->appendFile(f1, buf3, 64);
  cout << "rv from appendFile /o/o/o/b is " << r <<(r==64 ? " correct write 64 Fs": " fail")<< endl;
  r = c2->myFS->appendFile(f1, buf4, 64);
  cout << "rv from appendFile /o/o/o/b is " << r <<(r==64 ? " correct write 64 Is": " fail")<< endl;
  r = c2->myFS->closeFile(f1);
  cout << "rv from closeFile /o/o/o/b is " << r << (r==0 ? " correct":" fail")<<endl;

  r = c2->myFS->createFile(const_cast<char *>("/o/o/o/c"), 8);
  cout << "rv from createFile /o/o/o/c is " << r <<(r==0 ? " correct": " fail") <<endl;
  f1 = c2->myFS->openFile(const_cast<char *>("/o/o/o/c"), 8, 'w', -1);
  cout << "rv from /o/o/o/b openFile is " << f1 << (f1>0 ? " correct": " fail")<< endl;
  r = c2->myFS->writeFile(f1, buf2, 64);
  cout << "rv from writeFile /o/o/o/c is " << r <<(r==64 ? " correct write 64 Ts": " fail")<< endl;
  r = c2->myFS->writeFile(f1, buf3, 64);
  cout << "rv from writeFile /o/o/o/c is " << r <<(r==64 ? " correct write 64 Fs": " fail")<< endl;
  r = c2->myFS->writeFile(f1, buf4, 64);
  cout << "rv from writeFile /o/o/o/c is " << r <<(r==-3 ? " correct fs2 is full": " fail")<< endl;
  r = c2->myFS->appendFile(f1, buf4, 64);
  cout << "rv from appendFile /o/o/o/c is " << r <<(r==-3 ? " correct fs2 is full": " fail")<< endl;
  r = c2->myFS->closeFile(f1);
  cout << "rv from closeFile /o/o/o/c is " << r << (r==0 ? " correct":" fail")<<endl;


/* decided I don't care about reading it all back.
  r = c2->myFS->readFile(f4, rbuf2, 64);
  cout << "rv from readFile is " << r << endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf2[i];
  cout << endl;
*/
 

  cout <<"end driver 5\n";
  return 0;
}
