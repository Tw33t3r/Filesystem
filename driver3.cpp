
/* Driver 3*/

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
  Client *c5 = new Client(fs1);
  Client *c6 = new Client(fs2);
  Client *c7 = new Client(fs3);

  int i, r, l1, l2, f1, f2, f3, f4, f5;
  char buf1[37], buf2[64], buf3[100], buf4[600];
  char rbuf1[37], rbuf2[64], rbuf3[100], rbuf4[600];

  for (i = 0; i < 37; i++) buf1[i] = 'J';
  for (i = 0; i < 64; i++) buf2[i] = 'S';
  for (i = 0; i < 100; i++) buf3[i] = 'z';
  for (i = 0; i < 600; i++) buf4[i] = 'h';



  cout <<"Driver 3:  Directories basic tests\n";
  cout <<"create  Directory on fs1\n";
  r = c1->myFS->createDirectory(const_cast<char *>("/e"), 2);
  cout << "rv from createDirectory /e is " << r <<(r==0 ? " correct": " fail") <<endl;
  r = c1->myFS->createDirectory(const_cast<char *>("/a"), 2);
  cout << "rv from createDirectory /a is " << r <<(r==-4 ? " correct file /a exists": " fail") <<endl;
  r = c1->myFS->createDirectory(const_cast<char *>("f"), 1);
  cout << "rv from createDirectory  f is " << r <<(r==-3 ? " correct": " fail") <<endl;
  r = c1->myFS->createDirectory(const_cast<char *>("/ff"), 3);
  cout << "rv from createDirectory /ff is " << r <<(r==-3 ? " correct": " fail") <<endl;
  r = c1->myFS->createDirectory(const_cast<char *>("/$"), 2);
  cout << "rv from createDirectory /$ is " << r <<(r==-3 ? " correct": " fail") <<endl;
  r = c1->myFS->createDirectory(const_cast<char *>("/g"), 2);
  cout << "rv from createDirectory /g is " << r <<(r==0 ? " correct": " fail") <<endl;
  r = c1->myFS->createDirectory(const_cast<char *>("/h"), 2);
  cout << "rv from createDirectory /h is " << r <<(r==0 ? " correct": " fail") <<endl;
  r = c1->myFS->createFile(const_cast<char *>("/e"), 2);
  cout << "rv from createFile /e is " << r <<(r==-4 ? " correct": " fail") <<endl;

  r = c1->myFS->createDirectory(const_cast<char *>("/e/a"), 4);
  cout << "rv from createDirectory /e/a is " << r <<(r==0 ? " correct": " fail") <<endl;
  r = c1->myFS->createFile(const_cast<char *>("/e/b"), 4);
  cout << "rv from createFile /e/b is " << r <<(r==0 ? " correct": " fail") <<endl;
  r = c1->myFS->createFile(const_cast<char *>("/e/a"), 4);
  cout << "rv from createFile /e/a is " << r <<(r==-4 ? " correct": " fail") <<endl;
  r = c1->myFS->createDirectory(const_cast<char *>("/e/a/a"), 6);
  cout << "rv from createDirectory /e/a/a is " << r <<(r==0 ? " correct": " fail") <<endl;
  r = c1->myFS->createFile(const_cast<char *>("/e/a/a"), 6);
  cout << "rv from createFile /e/a/a is " << r <<(r==-4 ? " correct": " fail") <<endl;
  r = c1->myFS->createFile(const_cast<char *>("/e/a/a/b"), 8);
  cout << "rv from createFile /e/a/a/b is " << r <<(r==0 ? " correct": " fail") <<endl;
  r = c1->myFS->createFile(const_cast<char *>("/e/a/$/b"), 8);
  cout << "rv from createFile /e/a/$/b is " << r <<(r==-3 ? " correct": " fail") <<endl;
  r = c1->myFS->createFile(const_cast<char *>("/e/a/a/m"), 8);
  cout << "rv from createFile /e/a/a/m is " << r <<(r==0 ? " correct": " fail") <<endl;

  cout <<"\n open files in directories on fs1\n";
  f1 = c1->myFS->openFile(const_cast<char *>("/e/b"), 4, 'm', -1);
  cout << "rv from openFile is /e/b " << f1 <<(f1>0 ? " correct": " fail") <<endl;
  f2 = c1->myFS->openFile(const_cast<char *>("/e/a"), 4, 'm', -1);
  cout << "rv from openFile is /e/a " << f2 <<(f2==-1 ? " correct directory": " fail") <<endl;
  f2 = c1->myFS->openFile(const_cast<char *>("/e/a/a/b"), 8, 'm', -1);
  cout << "rv from openFile is /e/a/a/b " << f2 <<(f2>0 ? " correct": " fail") <<endl;
  f3 = c1->myFS->openFile(const_cast<char *>("/e/$/a/b"), 8, 'm', -1);
  cout << "rv from openFile is /e/$/a/b " << f3 <<(f3==-1 ? " correct": " fail") <<endl;

  l1 = c1->myFS->lockFile(const_cast<char *>("/e/a/a/m"), 8);
  cout << "rv from lockFile /e/a/a/m is " << l1 <<(l1>0 ? " correct": " fail") <<endl;
  f3 = c1->myFS->openFile(const_cast<char *>("/e/a/a/m"), 8, 'm', -1);
  cout << "rv from openFile is /e/a/a/m " << f3 <<(f3==-3 ? " correct bad lockid": " fail") <<endl;
  f3 = c1->myFS->openFile(const_cast<char *>("/e/a/a/m"), 8, 'm', l1);
  cout << "rv from openFile is /e/a/a/m " << f3 <<(f3>0 ? " correct ": " fail") <<endl;

  cout <<"\n reads and writes on fs1\n";

  r = c1->myFS->writeFile(f1, buf1, 5);
  cout << "rv from writeFile /e/b f1 is " << r <<(r==5 ? " Correct wrote 5 J": " failure") <<endl;
  r = c1->myFS->writeFile(f1, buf2, 5);
  cout << "rv from writeFile /e/b f1 is " << r <<(r==5 ? " Correct wrote 5 S": " failure") <<endl;
  r = c1->myFS->writeFile(f1, buf3, 5);
  cout << "rv from writeFile /e/b f1 is " << r <<(r==5 ? " Correct wrote 5 z": " failure") <<endl;
  r = c1->myFS->writeFile(f1, buf4, 5);
  cout << "rv from writeFile /e/b f1 is " << r <<(r==5 ? " Correct wrote 5 h": " failure") <<endl;
  r = c1->myFS->appendFile(f1, buf1, 5);
  cout << "rv from appendFile /e/b f1 is " << r <<(r==5 ? " Correct wrote 5 J": " failure") <<endl;

  r = c1->myFS->writeFile(f3, buf1, 37);
  cout << "rv from writeFile /e/a/a/m f3 is " << r <<(r==37 ? " Correct wrote 37 J": " failure") <<endl;
  r = c1->myFS->writeFile(f3, buf3, 91);
  cout << "rv from writeFile /e/a/a/m f3 is " << r <<(r==91 ? " Correct wrote 91 z": " failure") <<endl;


  r = c1->myFS->closeFile(f1);
  cout << "rv from closeFile /e/b is " << r <<(r==0 ? " correct": " fail") <<endl;
  r = c1->myFS->closeFile(f2);
  cout << "rv from closeFile /e/a/a/b is " << r <<(r==0 ? " correct": " fail") <<endl;
  r = c1->myFS->closeFile(f3);
  cout << "rv from closeFile /e/a/a/m is " << r <<(r==0 ? " correct": " fail") <<endl;
  r = c1->myFS->unlockFile(const_cast<char *>("/e/a/a/m"),8,l1);
  cout << "rv from unlockFile /e/a/a/m is " << r <<(r==0 ? " correct": " fail") <<endl;

  f1 = c4->myFS->openFile(const_cast<char *>("/e/b"), 4, 'r', -1);
  cout << "rv from openFile /e/b is " << f1 <<(f1>0 ? " correct": " fail") <<endl;
  f2 = c5->myFS->openFile(const_cast<char *>("/e/a/a/b"), 8, 'm', -1);
  cout << "rv from openFile /e/a/a/b is " << f2 <<(f2>0 ? " correct": " fail") <<endl;
  f3 = c4->myFS->openFile(const_cast<char *>("/e/a/a/m"), 8, 'r', -1);
  cout << "rv from openFile /e/a/a/m is " << f3 <<(f3>0 ? " correct": " fail") <<endl;

  r = c4->myFS->readFile(f1, rbuf1, 25);
  cout << "rv from readFile /e/b is " <<r <<(r==25 ? " Correct ": " failure") <<endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf1[i];
  cout << endl;

  r = c5->myFS->readFile(f2, rbuf1, 25);
  cout << "rv from readFile /e/a/a/b is " <<r<< (r==0 ? " Correct ": " failure") <<endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf1[i];
  cout << endl;

  r = c4->myFS->readFile(f3, rbuf4, 129);
  cout << "rv from readFile /e/a/a/m is " <<r<< (r==128 ? " Correct ": " failure") <<endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf4[i];
  cout << endl;

  r = c1->myFS->closeFile(f1);
  cout << "rv from closeFile /e/b is " << r <<(r==0 ? " correct": " fail") <<endl;
  r = c1->myFS->closeFile(f2);
  cout << "rv from closeFile /e/a/a/b is " << r <<(r==0 ? " correct": " fail") <<endl;
  r = c1->myFS->closeFile(f3);
  cout << "rv from closeFile /e/a/a/m is " << r <<(r==0 ? " correct": " fail") <<endl;


  cout <<"\ncreate and deletes of directories and files\n";
  
  r = c1->myFS->deleteFile(const_cast<char *>("/e/a/a/b"), 8);
  cout << "rv from deleteFile /e/a/a/b is " << r  <<(r==0 ? " correct": " fail") <<endl;

  r = c1->myFS->createDirectory(const_cast<char *>("/g/a"), 4);
  cout << "rv from createDirectory /g/a is " << r <<(r==0 ? " correct": " fail") <<endl;
  r = c1->myFS->createDirectory(const_cast<char *>("/g/a/g"), 6);
  cout << "rv from createDirectory /g/a/g is " << r <<(r==0 ? " correct": " fail") <<endl;
  r = c1->myFS->createFile(const_cast<char *>("/g/a/g/x"), 8);
  cout << "rv from createFile /g/a/g/x is " << r <<(r==0 ? " correct": " fail") <<endl;

  r = c5->myFS->deleteDirectory(const_cast<char *>("/g"), 2);
  cout << "rv from deleteDirectory /g is " << r <<(r==-2 ? " correct": " fail") <<endl;
  r = c5->myFS->deleteDirectory(const_cast<char *>("/g/a"), 4);
  cout << "rv from deleteDirectory /g/a is " << r <<(r==-2 ? " correct": " fail") <<endl;
  r = c5->myFS->deleteDirectory(const_cast<char *>("/g/a/g"), 6);
  cout << "rv from deleteDirectory /g/a/g is " << r <<(r==-2 ? " correct": " fail") <<endl;
  r = c5->myFS->deleteDirectory(const_cast<char *>("/g/a/g/x"), 8);
  cout << "rv from deleteDirectory /g/a/g/x is " << r <<(r==-1 ? " correct": " fail") <<endl;
  r = c1->myFS->deleteFile(const_cast<char *>("/g/a/g/x"), 8);
  cout << "rv from deleteFile /g/a/g/x is " << r  <<(r==0 ? " correct": " fail") <<endl;
  r = c5->myFS->deleteDirectory(const_cast<char *>("/g/a/g"), 6);
  cout << "rv from deleteDirectory /g/a/g is " << r <<(r==0 ? " correct": " fail") <<endl;
  r = c5->myFS->deleteDirectory(const_cast<char *>("/g/a"), 4);
  cout << "rv from deleteDirectory /g/a is " << r <<(r==0 ? " correct": " fail") <<endl;
  r = c5->myFS->deleteDirectory(const_cast<char *>("/g"), 2);
  cout << "rv from deleteDirectory /g is " << r <<(r==0 ? " correct": " fail") <<endl;

  cout <<"\n misc tests on directories fs1\n";
  l1 = c1->myFS->lockFile(const_cast<char *>("/h"), 2);
  cout << "rv from lockFile /h is " << l1 <<(l1==-4 ? " correct": " fail") <<endl;
  r = c1->myFS->deleteFile(const_cast<char *>("/h"), 2);
  cout << "rv from deleteFile /h is " << r  <<(r==-3 ? " correct": " fail") <<endl;
  r = c1->myFS->renameFile(const_cast<char *>("/h"), 2, const_cast<char *>("/x"), 2);
  cout << "rv from renameFile /h /x is " << r <<(r==-3 ? " correct": " fail") <<endl;
  r = c1->myFS->renameFile(const_cast<char *>("/h"), 2, const_cast<char *>("/o"), 2);
  cout << "rv from renameFile /h /o is " << r <<(r==0 ? " correct": " fail") <<endl;
  r = c1->myFS->renameFile(const_cast<char *>("/e/a/a/m"), 8, const_cast<char *>("/e/a/a/z"), 8);
  cout << "rv from renameFile /e/a/a/m /e/a/a/z is " << r <<(r==0 ? " correct": " fail") <<endl;

  cout <<"end of driver 3\n";

  return 0;
}
