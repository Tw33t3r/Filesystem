
/* Driver 2*/

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

  int i, r, l1, l2, f1, f2, f3, f4, f5;
  char buf1[37], buf2[64], buf3[100], buf4[600], buf5[1200];
  char rbuf1[37], rbuf2[64], rbuf3[100], rbuf4[600], rbuf5[1200];
  

  cout <<"begin driver 2\n";
  cout <<"rename test on fs1 (need driver1 first)\n";
  r = c1->myFS->renameFile(const_cast<char *>("/a"), 2, const_cast<char *>("/x"), 2);
  cout << "rv from renameFile /a to /x is " << r <<(r==0 ? " correct": " fail") <<endl;
  r = c1->myFS->renameFile(const_cast<char *>("/a"), 2, const_cast<char *>("/y"), 2);
  cout << "rv from renameFile /a to /y is " << r <<(r==-2 ? " correct": " fail")  <<endl;
  f1 = c1->myFS->openFile(const_cast<char *>("/a"), 2, 'w', -1);
  cout << "rv from openFile /a is " << f1 << (f1==-1 ? " correct": " fail")<<endl;
  r = c1->myFS->renameFile(const_cast<char *>("/b"), 2, const_cast<char *>("/c"), 2);
  cout << "rv from renameFile /b to /c is " << r <<(r==-3 ? " correct": " fail")  <<endl;
  r = c1->myFS->renameFile(const_cast<char *>("/b"), 2, const_cast<char *>("/y"), 2);
  cout << "rv from renameFile /b to /y is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c1->myFS->renameFile(const_cast<char *>("/c"), 2, const_cast<char *>("/$"), 2);
  cout << "rv from renameFile /c to /$ is " << r << (r==-1 ? " correct": " fail") <<endl;
  r = c1->myFS->renameFile(const_cast<char *>("/c"), 2, const_cast<char *>("z"), 1);
  cout << "rv from renameFile /c to z is " << r << (r==-1 ? " correct": " fail") <<endl;
  f1 = c1->myFS->openFile(const_cast<char *>("/c"), 2, 'w', -1);
  cout << "rv from openFile /a is " << f1 << (f1>0 ? " correct": " fail")<<endl;
  r = c1->myFS->renameFile(const_cast<char *>("/c"), 2, const_cast<char *>("/z"), 2);
  cout << "rv from renameFile /c to /z is " << r << (r==-4 ? " correct": " fail") <<endl;
  
  r = c1->myFS->closeFile(f1);
  cout << "rv from closeFile /c is " << r <<(r==0 ? " correct": " fail") <<endl;
  l2 = c1->myFS->lockFile(const_cast<char *>("/c"), 2);
  cout << "rv from lockFile /c is " << l2 << (l2>0 ? " correct": " failure") <<endl;
  r = c1->myFS->renameFile(const_cast<char *>("/c"), 2, const_cast<char *>("/z"), 2);
  cout << "rv from renameFile /c to /z is " << r << (r==-4 ? " correct": " fail") <<endl;
  r = c1->myFS->unlockFile( const_cast<char *>("/c"),2,l2);
  cout << "rv from unlockFile /c is " << r << (r==0 ? " correct ": " failure") <<endl;

  cout <<"\ndelete testing \n";
  r = c1->myFS->deleteFile(const_cast<char *>("/$"), 2);
  cout << "rv from deleteFile /$ is " << r <<(r==-3 ? " correct":" fail")<<endl;
  r = c1->myFS->deleteFile(const_cast<char *>("b"), 1);
  cout << "rv from deleteFile b is " << r <<(r==-3 ? " correct":" fail")<<endl;
  r = c1->myFS->deleteFile(const_cast<char *>("/bb"), 3);
  cout << "rv from deleteFile /bb is " << r <<(r==-3 ? " correct":" fail")<<endl;
  r = c1->myFS->deleteFile(const_cast<char *>("/y"), 2);
  cout << "rv from deleteFile /y is " << r <<(r==0 ? " correct":" fail")<<endl;
  r = c1->myFS->deleteFile(const_cast<char *>("/b"), 2);
  //Said y here, should be b?
  cout << "rv from deleteFile /b is " << r <<(r==-1 ? " correct":" fail")<<endl;
  
  f1 = c1->myFS->openFile(const_cast<char *>("/y"), 2, 'w', -1);
  cout << "rv from openFile /y is " << f1 << (f1==-1 ? " correct": " fail")<<endl;
  f1 = c1->myFS->openFile(const_cast<char *>("/d"), 2, 'w', -1);
  cout << "rv from openFile /d is " << f1 << (f1>0 ? " correct": " fail")<<endl;
  r = c1->myFS->deleteFile(const_cast<char *>("/d"), 2);
  cout << "rv from deleteFile /d is " << r <<(r==-2 ? " correct":" fail")<<endl;
  r = c1->myFS->closeFile(f1);
  cout << "rv from closeFile /d is " << r << (r==0 ? " correct":" fail")<<endl;
  l2 = c1->myFS->lockFile(const_cast<char *>("/c"), 2);
  cout << "rv from lockFile /c is " << l2 << (l2>0 ? " correct": " fail") <<endl;
  r = c1->myFS->deleteFile(const_cast<char *>("/c"), 2);
  cout << "rv from deleteFile /c is " << r <<(r==-2 ? " correct":" fail")<<endl;
  r = c1->myFS->unlockFile( const_cast<char *>("/c"),2,l2);
  cout << "rv from unlockFile /c is " << r << (r==0 ? " correct ": " fail") <<endl;

  cout <<"\ncreate files on 3 partitions\n";
  r = c1->myFS->createFile(const_cast<char *>("/a"), 2);
  cout << "rv from createFile /a fs1 is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c1->myFS->createFile(const_cast<char *>("/b"), 2);
  cout << "rv from createFile /b fs1 is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c2->myFS->createFile(const_cast<char *>("/a"), 2);
  cout << "rv from createFile /a fs2 is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c2->myFS->createFile(const_cast<char *>("/b"), 2);
  cout << "rv from createFile /b fs2 is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c3->myFS->createFile(const_cast<char *>("/a"), 2);
  cout << "rv from createFile /a fs3 is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c3->myFS->createFile(const_cast<char *>("/b"), 2);
  cout << "rv from createFile /b fs3 is " << r <<(r==0 ? " correct ": " fail") <<endl;


  cout <<"\nread/write and files on 3 partitions\n";
  f1 = c1->myFS->openFile(const_cast<char *>("/a"), 2, 'w', -1);
  cout << "rv from openFile /a w fs1 is " << f1 << (f1>0 ? " correct ": " fail") <<endl;
  f2 = c2->myFS->openFile(const_cast<char *>("/a"), 2, 'm', -1);
  cout << "rv from openFile /a m fs2 is " << f2 << (f2>0 ? " correct ": " fail") <<endl;
  f3 = c3->myFS->openFile(const_cast<char *>("/a"), 2, 'm', -1);
  cout << "rv from openFile /a m fs3 is " << f3 << (f3>0 ? " correct ": " fail") <<endl;
  f4 = c2->myFS->openFile(const_cast<char *>("/d"), 2, 'w', -1);
  cout << "rv from openFile /d w fs2 is " << f4 << (f4==-1 ? " correct ": " fail") <<endl;
  f4 = c3->myFS->openFile(const_cast<char *>("/d"), 2, 'w', -1);
  cout << "rv from openFile /d w fs3 is " << f4 << (f4==-1 ? " correct ": " fail") <<endl;

  for (i = 0; i < 37; i++) buf1[i] = 's';
  for (i = 0; i < 64; i++) buf2[i] = 'b';
  for (i = 0; i < 100; i++) buf3[i] = 'K';
  for (i = 0; i < 600; i++) buf4[i] = 'M';

  r = c1->myFS->writeFile(f1, buf1, 37);
  cout << "rv from writeFile /a fs1 is " << r <<(r==37 ? " correct ": " fail") <<endl;
  r = c2->myFS->writeFile(f2, buf2, 64);
  cout << "rv from writeFile /a fs2 is " << r <<(r==64 ? " correct ": " fail") <<endl;
  r = c3->myFS->writeFile(f3, buf3, 100);
  cout << "rv from writeFile /a fs3 is " << r <<(r==100 ? " correct ": " fail") <<endl;
  r = c3->myFS->writeFile(f4, buf4, 600);
  cout << "rv from writeFile ?? fs3 is " << r <<(r==-1 ? " correct ": " fail") <<endl;

  r = c1->myFS->closeFile(f1);
  cout << "rv from closeFile /a fs1 is " << r << (r==0 ? " correct":" fail")<<endl;
  r = c2->myFS->closeFile(f2);
  cout << "rv from closeFile /a fs2 is " << r << (r==0 ? " correct":" fail")<<endl;
  r = c2->myFS->closeFile(f2);
  cout << "rv from closeFile /a fs2 is " << r << (r==-1 ? " correct":" fail")<<endl;
  r = c3->myFS->closeFile(f3);
  cout << "rv from closeFile /a fs3 is " << r << (r==0 ? " correct":" fail")<<endl;
  r = c3->myFS->closeFile(f3);
  cout << "rv from closeFile /a fs3 is " << r << (r==-1 ? " correct":" fail")<<endl;
  r = c3->myFS->closeFile(f4);
  cout << "rv from closeFile ?? fs3 is " << r << (r==-1 ? " correct":" fail")<<endl;

  f1 = c1->myFS->openFile(const_cast<char *>("/a"), 2, 'r', -1);
  cout << "rv from openFile /a r fs1 is " << f1 << (f1>0 ? " correct ": " fail") <<endl;
  f2 = c2->myFS->openFile(const_cast<char *>("/a"), 2, 'm', -1);
  cout << "rv from openFile /a m fs2 is " << f2 << (f2>0 ? " correct ": " fail") <<endl;
  f3 = c3->myFS->openFile(const_cast<char *>("/a"), 2, 'r', -1);
  cout << "rv from openFile /a r fs3 is " << f3 << (f3>0 ? " correct ": " fail") <<endl;
  f4 = c3->myFS->openFile(const_cast<char *>("/b"), 2, 'm', -1);
  cout << "rv from openFile /b m fs3 is " << f4 << (f4>0 ? " correct ": " fail") <<endl;

  r = c1->myFS->readFile(f1, rbuf1, 20);
  cout << "rv from readFile /a fs1 is " << r << (r==20 ? " correct ": " fail") <<endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf1[i];
  cout << endl;

  r = c1->myFS->readFile(f1, rbuf1, 20);
  cout << "rv from readFile /a fs1 is " << r << (r==17 ? " correct ": " fail") <<endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf1[i];
  cout << endl;

  r = c2->myFS->readFile(f2, rbuf3, 85);
  cout << "rv from readFile /a fs2 is " << r << (r==64 ? " correct ": " fail") <<endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf3[i];
  cout << endl;

  r = c3->myFS->readFile(f3, rbuf4, 130);
  cout << "rv from readFile /a fs3 is " << r << (r==100 ? " correct ": " fail") <<endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf4[i];
  cout << endl;

  r = c3->myFS->readFile(f4, rbuf4, 130);
  cout << "rv from readFile /a fs3 is " << r << (r==0 ? " correct ": " fail") <<endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf4[i];
  cout << endl;

  r = c1->myFS->writeFile(f1, buf1, 20);
  cout << "rv from writeFile /a fs1 is " << r << (r==-3 ? " correct ": " fail") <<endl;

  r = c1->myFS->readFile(f1, rbuf1, 10);
  cout << "rv from readFile /a fs1 is " << r << (r==0 ? " correct ": " fail") <<endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf1[i];
  cout << endl;

  r = c3->myFS->readFile(f3, rbuf3, 85);
  cout << "rv from readFile /a fs2 is " << r << (r==0 ? " correct ": " fail") <<endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf3[i];
  cout << endl;


  r = c3->myFS->appendFile(f4, buf2, 20);
  cout << "rv from appendFile /b fs3 is " << r << (r==20 ? " correct ": " fail") <<endl;
  r = c3->myFS->seekFile(f4, 0, 1);
  cout << "rv from seekFile /b fs3 is " << r << (r==0 ? " correct ": " fail") <<endl;
  r = c3->myFS->readFile(f4, rbuf4, 300);
  cout << "rv from readFile /b fs3 is " << r << (r==20 ? " correct ": " fail") <<endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf4[i];
  cout << endl;

  cout <<"\nlocks on 3 partitions \n";

  r = c1->myFS->closeFile(f1);
  cout << "rv from closeFile /a fs1 is " << r << (r==0 ? " correct":" fail")<<endl;
  r = c2->myFS->closeFile(f2);
  cout << "rv from closeFile /a fs2 is " << r << (r==0 ? " correct":" fail")<<endl;
  r = c3->myFS->closeFile(f3);
  cout << "rv from closeFile /a fs3 is " << r << (r==0 ? " correct":" fail")<<endl;

  l1 = c2->myFS->lockFile(const_cast<char *>("/a"), 2);
  cout << "rv from lockFile /a fs2 is " << l2 << (l1>0 ? " correct": " failure") <<endl;
  l2 = c3->myFS->lockFile(const_cast<char *>("/a"), 2);
  cout << "rv from lockFile /a fs3 is " << l2 << (l1>0 ? " correct": " failure") <<endl;
  r = c2->myFS->unlockFile( const_cast<char *>("/a"),2,l1);
  cout << "rv from unlockFile /a fs2 is " << r << (r==0 ? " correct ": " failure") <<endl;
  l1 = c3->myFS->lockFile(const_cast<char *>("/b"), 2);
  cout << "rv from lockFile /b fs3 is " << l2 << (l1==-3 ? " correct": " failure") <<endl;
  r = c3->myFS->unlockFile( const_cast<char *>("/a"),2,l2);
  cout << "rv from unlockFile /a fs3 is " << r << (r==0 ? " correct ": " failure") <<endl;
  r = c3->myFS->closeFile(f4);
  cout << "rv from closeFile /b fs3 is " << r << (r==0 ? " correct":" fail")<<endl;

  cout <<"\ndirectory edge test on fs2\n";  //file up a directory, then delete
  r = c2->myFS->createFile(const_cast<char *>("/b"), 2);
  cout << "rv from createFile /b is " << r <<(r==-1 ? " correct ": " fail") <<endl;
  r = c2->myFS->createFile(const_cast<char *>("/c"), 2);
  cout << "rv from createFile /c is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c2->myFS->createFile(const_cast<char *>("/d"), 2);
  cout << "rv from createFile /d is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c2->myFS->createFile(const_cast<char *>("/e"), 2);
  cout << "rv from createFile /e is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c2->myFS->createFile(const_cast<char *>("/f"), 2);
  cout << "rv from createFile /f is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c2->myFS->createFile(const_cast<char *>("/g"), 2);
  cout << "rv from createFile /g is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c2->myFS->createFile(const_cast<char *>("/h"), 2);
  cout << "rv from createFile /h is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c2->myFS->createFile(const_cast<char *>("/i"), 2);
  cout << "rv from createFile /i is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c2->myFS->createFile(const_cast<char *>("/j"), 2);
  cout << "rv from createFile /j is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c2->myFS->createFile(const_cast<char *>("/k"), 2);
  cout << "rv from createFile /k is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c2->myFS->createFile(const_cast<char *>("/l"), 2);
  cout << "rv from createFile /l is " << r <<(r==0 ? " correct ": " fail") <<endl;

  f3 = c2->myFS->openFile(const_cast<char *>("/j"), 2, 'r', -1);
  cout << "rv from openFile /j r is " << f3 << (f3>0 ? " correct ": " fail") <<endl;
  f1 = c2->myFS->openFile(const_cast<char *>("/k"), 2, 'r', -1);
  cout << "rv from openFile /k r is " << f1 << (f1>0 ? " correct ": " fail") <<endl;

  f2 = c2->myFS->openFile(const_cast<char *>("/l"), 2, 'm', -1);
  cout << "rv from openFile /l r is " << f2 << (f2>0 ? " correct ": " fail") <<endl;
  r = c2->myFS->writeFile(f2, buf2, 7);
  cout << "rv from writeFile /l is " << r << (r==7 ? " correct ": " fail") <<endl;

  r = c2->myFS->closeFile(f2);
  cout << "rv from closeFile /l is " << r << (r==0 ? " correct":" fail")<<endl;

  r = c2->myFS->deleteFile(const_cast<char *>("/a"), 2);
  cout << "rv from deleteFile /a is " << r <<(r==0 ? " correct":" fail")<<endl;
  r = c2->myFS->deleteFile(const_cast<char *>("/b"), 2);
  cout << "rv from deleteFile /b is " << r <<(r==0 ? " correct":" fail")<<endl;

  f2 = c2->myFS->openFile(const_cast<char *>("/l"), 2, 'm', -1);
  cout << "rv from openFile /l r is " << f2 << (f2>0 ? " correct ": " fail") <<endl;
  r = c2->myFS->closeFile(f2);
  cout << "rv from closeFile /l is " << r << (r==0 ? " correct":" fail")<<endl;
  
  r = c2->myFS->deleteFile(const_cast<char *>("/c"), 2);
  cout << "rv from deleteFile /c is " << r <<(r==0 ? " correct":" fail")<<endl;
  r = c2->myFS->deleteFile(const_cast<char *>("/d"), 2);
  cout << "rv from deleteFile /d is " << r <<(r==0 ? " correct":" fail")<<endl;
  r = c2->myFS->deleteFile(const_cast<char *>("/e"), 2);
  cout << "rv from deleteFile /e is " << r <<(r==0 ? " correct":" fail")<<endl;
  r = c2->myFS->deleteFile(const_cast<char *>("/f"), 2);
  cout << "rv from deleteFile /f is " << r <<(r==0 ? " correct":" fail")<<endl;
  r = c2->myFS->deleteFile(const_cast<char *>("/g"), 2);
  cout << "rv from deleteFile /g is " << r <<(r==0 ? " correct":" fail")<<endl;
  r = c2->myFS->deleteFile(const_cast<char *>("/h"), 2);
  cout << "rv from deleteFile /h is " << r <<(r==0 ? " correct":" fail")<<endl;
  r = c2->myFS->deleteFile(const_cast<char *>("/i"), 2);
  cout << "rv from deleteFile /i is " << r <<(r==0 ? " correct":" fail")<<endl;
  r = c2->myFS->deleteFile(const_cast<char *>("/j"), 2);
  cout << "rv from deleteFile /j is " << r <<(r==-2 ? " correct":" fail")<<endl;
  r = c2->myFS->closeFile(f3);
  cout << "rv from closeFile /j is " << r << (r==0 ? " correct":" fail")<<endl;
  r = c2->myFS->closeFile(f1);
  cout << "rv from closeFile /k is " << r << (r==0 ? " correct":" fail")<<endl;
  r = c2->myFS->deleteFile(const_cast<char *>("/j"), 2);
  cout << "rv from deleteFile /j is " << r <<(r==0 ? " correct":" fail")<<endl;
  r = c2->myFS->deleteFile(const_cast<char *>("/k"), 2);
  cout << "rv from deleteFile /k is " << r <<(r==0 ? " correct":" fail")<<endl;

  f2 = c2->myFS->openFile(const_cast<char *>("/l"), 2, 'm', -1);
  cout << "rv from openFile /l r is " << f2 << (f2>0 ? " correct ": " fail") <<endl;
  r = c2->myFS->closeFile(f2);
  cout << "rv from closeFile /l is " << r << (r==0 ? " correct":" fail")<<endl;

  r = c2->myFS->deleteFile(const_cast<char *>("/l"), 2);
  cout << "rv from deleteFile /l is " << r <<(r==0 ? " correct":" fail")<<endl;

  cout <<"\nfile edge test and rw accuraty on fs2\n";  //fill up a file almost
  r = c2->myFS->createFile(const_cast<char *>("/z"), 2);
  cout << "rv from createFile /z is " << r <<(r==0 ? " correct ": " fail") <<endl;
  f2 = c2->myFS->openFile(const_cast<char *>("/z"), 2, 'm', -1);
  cout << "rv from openFile /z r is " << f2 << (f2>0 ? " correct ": " fail") <<endl;
  r = c2->myFS->writeFile(f2, buf4, 600);
  cout << "rv from writeFile /z is " << r << (r==600 ? " correct ": " fail") <<endl;
  r = c2->myFS->writeFile(f2, buf4, 600);
  cout << "rv from writeFile /z is " << r << (r==600 ? " correct ": " fail") <<endl;

  r = c2->myFS->seekFile(f2, 32, 1);
  cout << "rv from seekFile is " << r <<(r==0 ? " correct ": " fail") << endl;
  r = c2->myFS->writeFile(f2, buf2, 64);
  cout << "rv from writeFile /z is " << r << (r==64 ? " correct ": " fail") <<endl;

  for (i = 62; i<1152; i+=64) {
    r = c2->myFS->seekFile(f2, i, 1);
    cout << "rv from seekFile is " << r << endl;
    r = c2->myFS->writeFile(f2, buf1, 4);
    cout << "rv from writeFile /z is " << r << (r==4 ? " correct ": " fail") <<endl;
  }
  r = c2->myFS->appendFile(f2, buf2, 16);
  cout << "rv from appendFile is " << r << (r==16 ? " correct ": " fail") <<endl;

  r = c2->myFS->appendFile(f2, buf1, 4);
  cout << "rv from appendFile is " << r << (r==-3 ? " correct exceeds max file size": " fail") <<endl;
  
  r = c2->myFS->seekFile(f2, 0, -1);
  cout << "rv from seekFile is " << r << endl;

  for (int j =0; j<19; j++) {
    r = c2->myFS->readFile(f2, rbuf2, 64);
    cout << "rv from readFile /z is " << r << (r==64 ? " correct ": " fail") <<endl;
    cout << "Data read is " << endl;
    for (i = 0; i < r; i++) cout << rbuf2[i];
    cout << endl;
  }
  r = c2->myFS->closeFile(f2);
  cout << "rv from closeFile /z is " << r << (r==0 ? " correct":" fail")<<endl;
  
  cout <<"End of driver 2\n";
  
  return 0;
}
