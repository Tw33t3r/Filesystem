
/* Driver 4*/

#include <iostream>
#include <fstream>
#include <cstdlib>
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

  int i,r, l1, l2, f1, f2, f3, f4, f5;

  char buf1[37], buf2[64], buf3[100], buf4[600];
  char rbuf1[37], rbuf2[64], rbuf3[100], rbuf4[600];
  
  for (i = 0; i < 37; i++) buf1[i] = 'j';
  for (i = 0; i < 64; i++) buf2[i] = 'i';
  for (i = 0; i < 100; i++) buf3[i] = 'm';
  for (i = 0; i < 600; i++) buf4[i] = 'W';




  cout <<"begin driver 4\n";
  cout <<"Directory and file creates on multiple fs\n";

  r = c1->myFS->createDirectory(const_cast<char *>("/a"), 2);
  cout << "rv from createDirectory /a fs1 is " << r << (r==-4 ? " correct file /a": " fail") <<endl;
  r = c1->myFS->createDirectory(const_cast<char *>("/e"), 2);
  cout << "rv from createDirectory /e fs1 is " << r << (r==-1 ? " correct": " fail") <<endl;
  r = c2->myFS->createDirectory(const_cast<char *>("/e"), 2);
  cout << "rv from createDirectory /e fs2 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c3->myFS->createDirectory(const_cast<char *>("/e"), 2);
  cout << "rv from createDirectory /e fs3 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c4->myFS->createDirectory(const_cast<char *>("/e"), 2);
  cout << "rv from createDirectory /e fs1 is " << r << (r==-1 ? " correct": " fail") <<endl;
  r = c5->myFS->createDirectory(const_cast<char *>("/f"), 2);
  cout << "rv from createDirectory /f fs1 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c3->myFS->createDirectory(const_cast<char *>("/f"), 2);
  cout << "rv from createDirectory /f fs3 is " << r << (r==0 ? " correct": " fail") <<endl;


  r = c1->myFS->createFile(const_cast<char *>("/e/f"), 4);
  cout << "rv from createFile /e/f fs1 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c1->myFS->createFile(const_cast<char *>("/m/f"), 4);
  cout << "rv from createFile /m/f fs1 is " << r << (r==-4 ? " correct": " fail") <<endl;

  r = c2->myFS->createDirectory(const_cast<char *>("/e/b"), 4);
  cout << "rv from createDirectory /e/b fs2 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c5->myFS->createFile(const_cast<char *>("/e/b/a"), 6);
  cout << "rv from createFile /e/b/a fs2 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c5->myFS->createFile(const_cast<char *>("/e/c"), 4);
  cout << "rv from createFile /e/c fs2 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c2->myFS->createDirectory(const_cast<char *>("/e/b/d"), 6);
  cout << "rv from createDirectory /e/b/d fs2 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c5->myFS->createFile(const_cast<char *>("/e/b/d/x"), 8);
  cout << "rv from createFile /e/b/d/x fs2 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c2->myFS->createFile(const_cast<char *>("/e/b/d/x"), 8);
  cout << "rv from createFile /e/b/d/x fs2 is" << r << (r==-1 ? " correct": " fail") <<endl;
  r = c3->myFS->createDirectory(const_cast<char *>("/e/b"), 4);
  cout << "rv from createDirectory /e/b fs3 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c3->myFS->createFile(const_cast<char *>("/e/b/a"), 6);
  cout << "rv from createFile /e/b/a fs3 is " << r << (r==0 ? " correct": " fail") <<endl;

  cout <<"\nfile writes/appends/reads and seeks on multiple fs\n";
  f1 = c2->myFS->openFile(const_cast<char *>("/e/b/a"), 6, 'w', -1);
  cout << "rv from openFile /e/b/a w f1 fs2 is " << f1 << (f1>0 ? " correct": " fail") <<endl;
  f2 = c5->myFS->openFile(const_cast<char *>("/e/b/a"), 6, 'm', -1);
  cout << "rv from openFile /e/b/a m f2 fs2 is " << f2 << (f2>0 ? " correct": " fail") <<endl;
  f3 = c3->myFS->openFile(const_cast<char *>("/e/b/a"), 6, 'm', -1);
  cout << "rv from openFile /e/b/a m f3 fs3 is " << f2 << (f3>0 ? " correct": " fail") <<endl;

  r = c2->myFS->writeFile(f1, buf2, 5);
  cout << "rv from writeFile /e/b/a f1 is " << r <<(r==5 ? " Correct wrote 5 i": " fail") <<endl;
  r = c3->myFS->writeFile(f3, buf2, 5);
  cout << "rv from writeFile /e/b/a f3 is " << r <<(r==5 ? " Correct wrote 5 i": " fail") <<endl;
  r = c2->myFS->writeFile(f1, buf3, 5);
  cout << "rv from writeFile /e/b/a f1 is " << r <<(r==5 ? " Correct wrote 5 m": " fail") <<endl;
  r = c3->myFS->writeFile(f3, buf3, 5);
  cout << "rv from writeFile /e/b/a f3 is " << r <<(r==5 ? " Correct wrote 5 m": " fail") <<endl;
  r = c2->myFS->writeFile(f1, buf4, 5);
  cout << "rv from writeFile /e/b/a f1 is " << r <<(r==5 ? " Correct wrote 5 W": " fail") <<endl;
  r = c3->myFS->writeFile(f3, buf4, 5);
  cout << "rv from writeFile /e/b/a f3 is " << r <<(r==5 ? " Correct wrote 5 W": " fail") <<endl;
  r = c2->myFS->writeFile(f1, buf1, 5);
  cout << "rv from writeFile /e/b/a f1 is " << r <<(r==5 ? " Correct wrote 5 j": " fail") <<endl;
  r = c3->myFS->writeFile(f3, buf1, 5);
  cout << "rv from writeFile /e/b/a f3 is " << r <<(r==5 ? " Correct wrote 5 j": " fail") <<endl;
  r = c2->myFS->writeFile(f1, buf4, 5);
  cout << "rv from writeFile /e/b/a f1 is " << r <<(r==5 ? " Correct wrote 5 W": " fail") <<endl;
  r = c3->myFS->writeFile(f3, buf4, 5);
  cout << "rv from writeFile /e/b/a f1 is " << r <<(r==5 ? " Correct wrote 5 W": " fail") <<endl;

  r = c5->myFS->seekFile(f2, 4, -1);
  cout << "rv from seekFile /e/b/a f2 is " << r << (r==0 ? " Correct rw set to 4": " fail") <<endl;
  r = c5->myFS->writeFile(f2, buf1, 1);
  cout << "rv from writeFile /e/b/a f2 is " << r <<(r==1 ? " Correct wrote 1 j": " fail") <<endl;
  r = c5->myFS->seekFile(f2, 5, -1);
  cout << "rv from seekFile /e/b/a f2 is " << r << (r==0 ? " Correct rw set to 5": " fail") <<endl;
  r = c5->myFS->writeFile(f2, buf2, 1);
  cout << "rv from writeFile /e/b/a f2 is " << r <<(r==1 ? " Correct wrote 1 i": " fail") <<endl;
  r = c5->myFS->seekFile(f2, 6, -1);
  cout << "rv from seekFile /e/b/a f2 is " << r << (r==0 ? " Correct rw set to 6": " fail") <<endl;
  r = c5->myFS->writeFile(f2, buf3, 1);
  cout << "rv from writeFile /e/b/a f2 is " << r <<(r==1 ? " Correct wrote 1 m": " fail") <<endl;
  r = c5->myFS->seekFile(f2, 7, -1);
  cout << "rv from seekFile /e/b/a f2 is " << r << (r==0 ? " Correct rw set to 7": " fail") <<endl;
  r = c5->myFS->writeFile(f2, buf4, 1);
  cout << "rv from writeFile /e/b/a f2 is " << r <<(r==1 ? " Correct wrote 1 w": " fail") <<endl;
  r = c5->myFS->appendFile(f2, buf1, 1);
  cout << "rv from appendFile /e/b/a f2 is " << r <<(r==1 ? " Correct wrote 1 j": " fail") <<endl;
  r = c5->myFS->appendFile(f2, buf2, 1);
  cout << "rv from appendFile /e/b/a f2 is " << r <<(r==1 ? " Correct wrote 1 i": " fail") <<endl;
  r = c5->myFS->appendFile(f2, buf3, 1);
  cout << "rv from appendFile /e/b/a f2 is " << r <<(r==1 ? " Correct wrote 1 m": " fail") <<endl;
  r = c5->myFS->appendFile(f2, buf4, 1);
  cout << "rv from appendFile /e/b/a f2 is " << r <<(r==1 ? " Correct wrote 1 w": " fail") <<endl;

  r = c2->myFS->closeFile(f1);
  cout << "rv from closeFile /e/b/a f1 fs2 is " << r <<(r==0 ? " Correct": " fail") <<endl;

  r = c5->myFS->seekFile(f2, -100, 0);
  cout << "rv from sekerFile /e/b/a /e/b/a fs2  is " << r << (r== -2 ? " correct -100 before begin": " fail") <<endl;
  r = c5->myFS->seekFile(f2, 0, -1);
  cout << "rv from seekFile /e/b/a f2 is " << r << (r==0 ? " Correct rw set to 0": " fail") <<endl;

  r = c5->myFS->readFile(f2, rbuf2, 64);
  cout << "rv from readFile /e/b/a fs2 is " <<r<< (r==29 ? " Correct ": " fail") <<endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf2[i];
  cout << endl;

  r = c3->myFS->seekFile(f3, 0, -1);
  cout << "rv from seekFile /e/b/a f3 is " << r << (r==0 ? " Correct rw set to 0": " fail") <<endl;
  r = c3->myFS->readFile(f3, rbuf2, 64);
  cout << "rv from readFile /e/b/a fs3 is " <<r<< (r==25 ? " Correct ": " fail") <<endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf2[i];
  cout << endl;

  r = c5->myFS->closeFile(f2);
  cout << "rv from closeFile /e/b/a f2 fs2 is " << r <<(r==0 ? " Correct": " fail") <<endl;
  r = c3->myFS->closeFile(f3);
  cout << "rv from closeFile /e/b/a f3 fs3 is " << r <<(r==0 ? " Correct": " fail") <<endl;


  cout <<"\ndelete tests, with open and locks\n";
  r = c2->myFS->createDirectory(const_cast<char *>("/n"), 2);
  cout << "rv from createDirectory /n fs2 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c2->myFS->createDirectory(const_cast<char *>("/n/n"), 4);
  cout << "rv from createDirectory /n/n fs2 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c2->myFS->createDirectory(const_cast<char *>("/o"), 2);
  cout << "rv from createDirectory /o fs2 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c2->myFS->createDirectory(const_cast<char *>("/o/o"), 4);
  cout << "rv from createDirectory /o/o fs2 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c2->myFS->createDirectory(const_cast<char *>("/o/o/o"), 6);
  cout << "rv from createDirectory /o/o/o fs2 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c2->myFS->createDirectory(const_cast<char *>("/o/o/o/a"), 8);
  cout << "rv from createDirectory /o/o/o/a fs2 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c2->myFS->createDirectory(const_cast<char *>("/o/o/o/a/a"), 10);
  cout << "rv from createDirectory /o/o/o/a/a fs2 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c2->myFS->createFile(const_cast<char *>("/o/o/o/a/a/z"), 12);
  cout << "rv from createFile /o/o/o/a/a/z fs3 is " << r <<(r==0 ? " correct ": " fail") <<endl;

  r = c3->myFS->createDirectory(const_cast<char *>("/n"), 2);
  cout << "rv from createDirectory /n fs3 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c3->myFS->createDirectory(const_cast<char *>("/n/n"), 4);
  cout << "rv from createDirectory /n/n fs3 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c3->myFS->createDirectory(const_cast<char *>("/o"), 2);
  cout << "rv from createDirectory /o fs3 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c3->myFS->createDirectory(const_cast<char *>("/o/o"), 4);
  cout << "rv from createDirectory /o/o fs3 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c3->myFS->createDirectory(const_cast<char *>("/o/o/o"), 6);
  cout << "rv from createDirectory /o/o/o fs3 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c3->myFS->createDirectory(const_cast<char *>("/o/o/o/a"), 8);
  cout << "rv from createDirectory /o/o/o/a fs3 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c3->myFS->createDirectory(const_cast<char *>("/o/o/o/a/a"), 10);
  cout << "rv from createDirectory /o/o/o/a/a fs3 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c3->myFS->createFile(const_cast<char *>("/o/o/o/a/a/z"), 12);
  cout << "rv from createFile /o/o/o/a/a/z fs3 is " << r <<(r==0 ? " correct ": " fail") <<endl;


  r = c2->myFS->createFile(const_cast<char *>("/o/o/o/a"), 8);
  cout << "rv from createFile /o/o/o/a fs2 is " << r <<(r==-4 ? " correct ": " fail") <<endl;
  r = c3->myFS->createFile(const_cast<char *>("/o/o/o/a"), 8);
  cout << "rv from createFile /o/o/o/a fs3 is " << r <<(r==-4 ? " correct ": " fail") <<endl;

  r = c5->myFS->createFile(const_cast<char *>("/o/o/o/a/b"), 10);
  cout << "rv from createFile /o/o/o/a/b fs2 is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c2->myFS->createDirectory(const_cast<char *>("/o/o/o/a/c"), 10);
  cout << "rv from createDirectory /o/o/o/a/c fs2 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c5->myFS->createFile(const_cast<char *>("/o/o/o/a/d"), 10);
  cout << "rv from createFile /o/o/o/a/d fs2 is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c2->myFS->createFile(const_cast<char *>("/o/o/o/a/e"), 10);
  cout << "rv from createFile /o/o/o/a/e fs2 is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c5->myFS->createFile(const_cast<char *>("/o/o/o/a/f"), 10);
  cout << "rv from createFile /o/o/o/a/f fs2 is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c2->myFS->createFile(const_cast<char *>("/o/o/o/a/g"), 10);
  cout << "rv from createFile /o/o/o/a/g fs2 is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c5->myFS->createFile(const_cast<char *>("/o/o/o/a/h"), 10);
  cout << "rv from createFile /o/o/o/a/h fs2 is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c2->myFS->createFile(const_cast<char *>("/o/o/o/a/i"), 10);
  cout << "rv from createFile /o/o/o/a/i fs2 is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c5->myFS->createFile(const_cast<char *>("/o/o/o/a/j"), 10);
  cout << "rv from createFile /o/o/o/a/j fs2 is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c2->myFS->createDirectory(const_cast<char *>("/o/o/o/a/k"), 10);
  cout << "rv from createDirectory /o/o/o/a/k fs2 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c5->myFS->createFile(const_cast<char *>("/o/o/o/a/l"), 10);
  cout << "rv from createFile /o/o/o/a/l fs2 is " << r <<(r==0 ? " correct ": " fail") <<endl;

  r = c3->myFS->createFile(const_cast<char *>("/o/o/o/a/b"), 10);
  cout << "rv from createFile /o/o/o/a/b fs3 is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c3->myFS->createDirectory(const_cast<char *>("/o/o/o/a/c"), 10);
  cout << "rv from createDirectory /o/o/o/a/c fs3 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c3->myFS->createFile(const_cast<char *>("/o/o/o/a/d"), 10);
  cout << "rv from createFile /o/o/o/a/d fs3 is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c3->myFS->createFile(const_cast<char *>("/o/o/o/a/e"), 10);
  cout << "rv from createFile /o/o/o/a/e fs3 is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c3->myFS->createFile(const_cast<char *>("/o/o/o/a/f"), 10);
  cout << "rv from createFile /o/o/o/a/f fs3 is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c3->myFS->createFile(const_cast<char *>("/o/o/o/a/g"), 10);
  cout << "rv from createFile /o/o/o/a/g fs3 is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c3->myFS->createFile(const_cast<char *>("/o/o/o/a/h"), 10);
  cout << "rv from createFile /o/o/o/a/h fs3 is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c3->myFS->createFile(const_cast<char *>("/o/o/o/a/i"), 10);
  cout << "rv from createFile /o/o/o/a/i fs3 is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c3->myFS->createFile(const_cast<char *>("/o/o/o/a/j"), 10);
  cout << "rv from createFile /o/o/o/a/j fs3 is " << r <<(r==0 ? " correct ": " fail") <<endl;
  r = c3->myFS->createDirectory(const_cast<char *>("/o/o/o/a/k"), 10);
  cout << "rv from createDirectory /o/o/o/a/k fs3 is " << r << (r==0 ? " correct": " fail") <<endl;
  r = c3->myFS->createFile(const_cast<char *>("/o/o/o/a/l"), 10);
  cout << "rv from createFile /o/o/o/a/l fs3 is " << r <<(r==0 ? " correct ": " fail") <<endl;

  l1 = c5->myFS->lockFile(const_cast<char *>("/o/o/o/a/h"), 10);
  cout << "rv from lockFile /o/o/o/a/h fs2 is " << l1 << (l1>0 ?" correct": " fail") <<endl;
  l2 = c3->myFS->lockFile(const_cast<char *>("/o/o/o/a/h"), 10);
  cout << "rv from lockFile /o/o/o/a/h fs3 is " << l2 <<(l2>0 ?" correct": " fail")  <<endl;

  f1 = c2->myFS->openFile(const_cast<char *>("/o/o/o/a/b"), 10, 'm', -1);
  cout << "rv from openFile /o/o/o/a/b fs2 is " << f1 << (f1>0? " correct": " fail")<<endl;
  f2 = c3->myFS->openFile(const_cast<char *>("/o/o/o/a/b"), 10, 'm', -1);
  cout << "rv from openFile /o/o/o/a/b fs3 is " << f2 << (f2>0? " correct": " fail")<<endl;
   
  r = c5->myFS->renameFile(const_cast<char *>("/o/o/o/a/b"), 10, const_cast<char *>("/o/o/o/a/x"), 10);
  cout << "rv from renameFile /o/o/o/a/b fs2 to /o/o/o/a/x is " << r << (r==-4 ? " correct":" fail")<<endl;
  r = c3->myFS->renameFile(const_cast<char *>("/o/o/o/a/b"), 10, const_cast<char *>("/o/o/o/a/x"), 10);
  cout << "rv from renameFile /o/o/o/a/b fs3 to /o/o/o/a/x is " << r << (r==-4 ? " correct":" fail")<<endl;

  r = c2->myFS->renameFile(const_cast<char *>("/o/o/o/a/h"), 10, const_cast<char *>("/o/o/o/a/x"), 10);
  cout << "rv from renameFile /o/o/o/a/b to /o/o/o/a/x fs2 is " << r << (r==-4 ? " correct":" fail")<<endl;
  r = c3->myFS->renameFile(const_cast<char *>("/o/o/o/a/h"), 10, const_cast<char *>("/o/o/o/a/x"), 10);
  cout << "rv from renameFile /o/o/o/a/b to /o/o/o/a/x fs3 is " << r << (r==-4 ? " correct":" fail")<<endl;

  r = c2->myFS->deleteDirectory(const_cast<char *>("/o/o/o"), 6);
  cout << "rv from deleteDirectory /o/o/o fs2 is " << r <<(r==-2 ? " correct":" fail") <<endl;
  r = c3->myFS->deleteDirectory(const_cast<char *>("/o/o/o"), 6);
  cout << "rv from deleteDirectory /o/o/o fs3 is " << r <<(r==-2 ? " correct":" fail") <<endl;

  r = c2->myFS->deleteFile(const_cast<char *>("/o/o/o"), 6);
  cout << "rv from deleteFile /o/o/o fs2 is " << r <<(r==-3 ? " correct":" fail")<<endl;
  r = c3->myFS->deleteFile(const_cast<char *>("/o/o/o"), 6);
  cout << "rv from deleteFile /o/o/o fs3 is " << r <<(r==-3 ? " correct":" fail")<<endl;

  r = c2->myFS->deleteFile(const_cast<char *>("/o/o/o/a/b"), 10);
  cout << "rv from deleteFile /o/o/o/a/b fs2 is " << r <<(r==-2 ? " correct":" fail")<<endl;
  r = c3->myFS->deleteFile(const_cast<char *>("/o/o/o/a/b"), 10);
  cout << "rv from deleteFile /o/o/o/a/b fs3 is " << r <<(r==-2 ? " correct":" fail")<<endl;

  r = c2->myFS->closeFile(f1);
  cout << "rv from closeFile /o/o/o/a/b f1 fs2 is " << r <<(r==0 ? " Correct": " fail") <<endl;
  r = c3->myFS->closeFile(f2);
  cout << "rv from closeFile /o/o/o/a/b f2 fs3 is " << r <<(r==0 ? " Correct": " fail") <<endl;

  r = c2->myFS->deleteFile(const_cast<char *>("/o/o/o/a/b"), 10);
  cout << "rv from deleteFile /o/o/o/a/b fs2 is " << r <<(r==0 ? " correct":" fail")<<endl;
  r = c5->myFS->deleteDirectory(const_cast<char *>("/o/o/o/a/c"), 10);
  cout << "rv from deleteDirectory /o/o/o/a/c fs2 is " << r <<(r==0 ? " correct":" fail") <<endl;
  r = c2->myFS->deleteFile(const_cast<char *>("/o/o/o/a/d"), 10);
  cout << "rv from deleteFile /o/o/o/a/d fs2 is " << r <<(r==0 ? " correct":" fail")<<endl;
  r = c5->myFS->deleteFile(const_cast<char *>("/o/o/o/a/e"), 10);
  cout << "rv from deleteFile /o/o/o/a/e fs2 is " << r <<(r==0 ? " correct":" fail")<<endl;
  r = c2->myFS->deleteFile(const_cast<char *>("/o/o/o/a/f"), 10);
  cout << "rv from deleteFile /o/o/o/a/f fs2 is " << r <<(r==0 ? " correct":" fail")<<endl;
  r = c5->myFS->deleteFile(const_cast<char *>("/o/o/o/a/g"), 10);
  cout << "rv from deleteFile /o/o/o/a/g fs2 is " << r <<(r==0 ? " correct":" fail")<<endl;
  r = c2->myFS->deleteFile(const_cast<char *>("/o/o/o/a/h"), 10);
  cout << "rv from deleteFile /o/o/o/a/h fs2 is " << r <<(r==-2 ? " correct":" fail")<<endl;
  r = c5->myFS->unlockFile(const_cast<char *>("/o/o/o/a/h"),10,l1);
  cout << "rv from unlockFile /o/o/o/a/h fs2 is " << r << (r== 0? " correct":" fail")<<endl;
  r = c3->myFS->unlockFile(const_cast<char *>("/o/o/o/a/h"),10,l2);
  cout << "rv from unlockFile /o/o/o/a/h fs3 is " << r << (r== 0? " correct":" fail")<<endl;
  r = c2->myFS->deleteFile(const_cast<char *>("/o/o/o/a/h"), 10);
  cout << "rv from deleteFile /o/o/o/a/h fs2 is " << r <<(r==0 ? " correct":" fail")<<endl;
  r = c5->myFS->deleteFile(const_cast<char *>("/o/o/o/a/i"), 10);
  cout << "rv from deleteFile /o/o/o/a/i fs2 is " << r <<(r==0 ? " correct":" fail")<<endl;
  r = c2->myFS->deleteFile(const_cast<char *>("/o/o/o/a/j"), 10);
  cout << "rv from deleteFile /o/o/o/a/j fs2 is " << r <<(r==0 ? " correct":" fail")<<endl;
  r = c5->myFS->deleteDirectory(const_cast<char *>("/o/o/o/a/k"), 10);
  cout << "rv from deleteDirectory /o/o/o/a/k fs2 is " << r <<(r==0 ? " correct":" fail") <<endl;
  r = c2->myFS->deleteFile(const_cast<char *>("/o/o/o/a/l"), 10);
  cout << "rv from deleteFile /o/o/o/a/l fs2 is " << r <<(r==0 ? " correct":" fail")<<endl;
  r = c5->myFS->deleteDirectory(const_cast<char *>("/o/o/o/a/a"), 10);
  cout << "rv from deleteDirectory /o/o/o/a/a fs2 is " << r <<(r==-2 ? " correct":" fail") <<endl;
  r = c2->myFS->deleteFile(const_cast<char *>("/o/o/o/a/a/z"), 12);
  cout << "rv from deleteFile /o/o/o/a/a/z fs2 is " << r <<(r==0 ? " correct":" fail")<<endl;
  r = c5->myFS->deleteDirectory(const_cast<char *>("/o/o/o/a/a"), 10);
  cout << "rv from deleteDirectory /o/o/o/a/a fs2 is " << r <<(r==0 ? " correct":" fail") <<endl;


  cout <<"\nrename tests\n";
  r = c2->myFS->renameFile(const_cast<char *>("/o/o/o/a/a"), 10, const_cast<char *>("/o/o/o/a/z"), 10);
  cout << "rv from renameFile /o/o/o/a/a /o/o/o/a/z fs2 is " << r << (r==-2 ? " correct":" fail") <<endl;
  r = c5->myFS->renameFile(const_cast<char *>("/o/o/o/a"), 8, const_cast<char *>("/o/o/o/z"), 8);
  cout << "rv from renameFile /o/o/o/a/a /o/o/o/a/z fs3 is " << r << (r==0 ? " correct":" fail") <<endl;
  r = c3->myFS->renameFile(const_cast<char *>("/o/o/o/a"), 8, const_cast<char *>("/o/o/o/z"), 8);
  cout << "rv from renameFile /o/o/o/a /o/o/o/z fs3 is " << r << (r==0 ? " correct":" fail") <<endl;
  r = c3->myFS->createDirectory(const_cast<char *>("/o/o/o/z"), 8);
  cout << "rv from createDirectory /o/o/o/z fs3 is " << r << (r==-1 ? " correct": " fail") <<endl;
  r = c2->myFS->deleteDirectory(const_cast<char *>("/o/o/o/a"), 8);
  cout << "rv from deleteDirectory /o/o/o/a fs2 is " << r <<(r==-1 ? " correct":" fail") <<endl;
  r = c5->myFS->deleteDirectory(const_cast<char *>("/o/o/o/z"), 8);
  cout << "rv from deleteDirectory /o/o/o/z fs2 is " << r <<(r==0 ? " correct":" fail") <<endl;
  r = c3->myFS->deleteDirectory(const_cast<char *>("/o/o/o/z"), 8);
  cout << "rv from deleteDirectory /o/o/o/z fs3 is " << r <<(r==-2 ? " correct":" fail") <<endl;

  cout <<"End of driver 4\n";
  return 0;
}
