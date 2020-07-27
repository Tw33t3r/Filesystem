
/* Driver 1*/

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
  Client *c1 = new Client(fs1);
  int r, l1, l2, f1, f2, f3, f4, f5,i;
  char buf1[37], buf2[64], buf3[100], buf4[600];
  char rbuf1[37], rbuf2[64], rbuf3[100], rbuf4[600];
  
  cout <<"driver 1 basic file system tests on fs1\n";
  cout <<"File create tests on fs1\n";
  r = c1->myFS->createFile( const_cast<char *>("/a"), 2);
  cout << "rv from createFile /a is " << r << (r==0?" Correct": " failure") <<endl;
  r = c1->myFS->createFile( const_cast<char *>("/b"), 2);
  cout << "rv from createFile /b is " << r << (r==0?" Correct": " failure") <<endl;
  r = c1->myFS->createFile( const_cast<char *>("/a"), 2);
  cout << "rv from createFile /a is " << r << (r==-1?" Correct file exists": " failure") <<endl;
  r = c1->myFS->createFile( const_cast<char *>("a"), 1);
  cout << "rv from createFile  a is " << r << (r==-3?" Correct file invalid": " failure") <<endl;
  r = c1->myFS->createFile( const_cast<char *>("aa"), 2);
  cout << "rv from createFile aa is " << r << (r==-3?" Correct file invalid": " failure") <<endl;
  r = c1->myFS->createFile( const_cast<char *>("/$"), 2);
  cout << "rv from createFile /$ is " << r << (r==-3?" Correct file invalid": " failure") <<endl;
  r = c1->myFS->createFile( const_cast<char *>("/c"), 2);
  cout << "rv from createFile /c is " << r << (r==0?" Correct": " failure") <<endl;
  r = c1->myFS->createFile( const_cast<char *>("/d"), 2);
  cout << "rv from createFile /d is " << r << (r==0?" Correct": " failure") <<endl;

  cout<<"\nlock tests on fs1\n";
  l1 = c1->myFS->lockFile( const_cast<char *>("/a"), 2);
  cout << "rv from lockFile /a is " << l1 << (l1>0 ? " Correct file locked": " failure") <<endl;
  l2 = c1->myFS->lockFile( const_cast<char *>("/a"), 2);
  cout << "rv from lockFile /a is " << l2 << (l2==-1 ? " Correct file already locked": " failure") <<endl;
  l2 = c1->myFS->lockFile( const_cast<char *>("/g"), 2);
  cout << "rv from lockFile /g is " << l2 << (l2==-2 ? " Correct file does not exist": " failure") <<endl;
  r = c1->myFS->unlockFile( const_cast<char *>("/g"),2,l1);
  cout << "rv from unlockFile /g is " << r << (r==-2 ? " Correct file does not exist and bad lock": " failure") <<endl;
  r = c1->myFS->unlockFile( const_cast<char *>("/a"),2,10023);
  cout << "rv from unlockFile /a is " << r << (r==-1 ? " Correct wrong lock id (10023)": " failure") <<endl;
  r = c1->myFS->unlockFile( const_cast<char *>("/a"),2,l1);
  cout << "rv from unlockFile /a is " << r << (r==0 ? " Correct file unlocked": " failure") <<endl;

  cout<<"\nfile open tests on fs1\n";
  f1 = c1->myFS->openFile( const_cast<char *>("/c"), 2, 'r', -1);
  cout << "rv from openFile /c is " << f1 << (f1>0 ? " Correct file open (r) f1": " failure") <<endl;
  f2 = c1->myFS->openFile( const_cast<char *>("/d"), 2, 'm', -1);
  cout << "rv from openFile /d is " << f2 << (f2>0 ? " Correct file open (m) f2": " failure") <<endl;
  f3 = c1->myFS->openFile( const_cast<char *>("/b"), 2, 'w', -1);
  cout << "rv from openFile /b is " << f3 << (f3>0 ? " Correct file open (w) f3": " failure") <<endl;
  f4 = c1->myFS->openFile( const_cast<char *>("/j"), 2, 'w', -1);
  cout << "rv from openFile /j is " << f4 << (f4==-1 ? " Correct file does not exist f4": " failure") <<endl;
  f4 = c1->myFS->openFile( const_cast<char *>("/d"), 2, 'r', -1);
  cout << "rv from openFile /d is " << f4 << (f4>0 ? " Correct file open again (r) f4": " failure") <<endl;
  f5 = c1->myFS->openFile( const_cast<char *>("/a"), 2, 'r', 1);
  cout << "rv from openFile /a is " << f5 << (f5==-3 ? " Correct file open failed, bad lock id (1) and /a is not locked f5": " failure") <<endl;
  f5 = c1->myFS->openFile( const_cast<char *>("aa"), 2, 'r', -1);
  cout << "rv from openFile aa is " << f5 << (f5==-1 ? " Correct file open failed wrong file name f5": " failure") <<endl;
  f5 = c1->myFS->openFile( const_cast<char *>("/a"), 2, 'b', -1);
  cout << "rv from openFile /a is " << f5 << (f5==-2 ? " Correct file open failed, bad mode (b) f5": " failure") <<endl;

  
  cout<<"\nfile close tests on fs1\n";
  r = c1->myFS->closeFile(f1);
  cout << "rv from closeFile f1 /c is " << r << (r==0 ? " Correct file closed f1": " failure")<<endl;
  r = c1->myFS->closeFile(f1);
  cout << "rv from closeFile f1 /c is " << r << (r==-1 ? " Correct file closed failed f1": " failure")<<endl;
  r = c1->myFS->closeFile(1000);
  cout << "rv from closeFile fileid 1000 is " << r << (r==-1 ? " Correct file closed failed bad descriptor": " failure")<<endl;
 
  cout<<"\nfile open and lock tests on fs1\n";
  l1 = c1->myFS->lockFile(const_cast<char *>("/d"), 2);
  cout << "rv from lockFile /d is " << l1 << (l1==-3 ? " Correct lock failed, file opened ": " failure") <<endl;

  l2 = c1->myFS->lockFile(const_cast<char *>("/a"), 2);
  cout << "rv from lockFile /a is " << l2 << (l2>0 ? " Correct file locked ": " failure") <<endl;
  f1 = c1->myFS->openFile(const_cast<char *>("/a"), 2, 'r', -1);
  cout << "rv from openFile /a is " << f1 << (f1==-3 ? " Correct file opened failed locked, bad id ": " failure") <<endl;
  f1 = c1->myFS->openFile(const_cast<char *>("/a"), 2, 'r', l2);
  cout << "rv from openFile /a is " << f1 << (f1>0 ? " Correct file opened (r) f1 ": " failure") <<endl;
  r = c1->myFS->closeFile(f1);
  cout << "rv from closeFile f1 /a is " << r << (r==0 ? " Correct file closed": " failure")<<endl;
  r = c1->myFS->unlockFile(const_cast<char *>("/a"),2,l2);
  cout << "rv from unlockFile /a is " << r << (r==0 ? " Correct file unlocked l2": " failure") <<endl;

  cout<<"\nclosing all the files on fs1\n";
  r = c1->myFS->closeFile(f2);
  cout << "rv from closeFile f2 /d is " << r << (r==0 ? " Correct file closed": " failure")<<endl;
  r = c1->myFS->closeFile(f3);
  cout << "rv from closeFile f3 /b is " << r << (r==0 ? " Correct file closed": " failure")<<endl;
  r = c1->myFS->closeFile(f4);
  cout << "rv from closeFile f4 /d is " << r << (r==0 ? " Correct file closed": " failure")<<endl;


  for (i = 0; i < 37; i++) buf1[i] = 's';
  for (i = 0; i < 64; i++) buf2[i] = 'b';
  for (i = 0; i < 100; i++) buf3[i] = 'K';
  for (i = 0; i < 600; i++) buf4[i] = 'M';

  cout<<"\nopen and write tests on fs1\n";
  f1 = c1->myFS->openFile(const_cast<char *>("/a"), 2, 'w', -1);
  cout << "rv from openFile /a is " << f1 << (f1>0 ? " Correct file opened (w) f1 ": " failure") <<endl;
  f2 = c1->myFS->openFile(const_cast<char *>("/b"), 2, 'm', -1);
  cout << "rv from openFile /b is " << f2 << (f2>0 ? " Correct file opened (m) f2 ": " failure") <<endl;
  r = c1->myFS->writeFile(f1, buf1, 5);
  cout << "rv from writeFile /a f1 is " << r <<(r==5 ? " Correct wrote 5 s": " failure") <<endl;
  r = c1->myFS->writeFile(f1, buf2, 5);
  cout << "rv from writeFile /a f1 is " << r <<(r==5 ? " Correct wrote 5 b": " failure") <<endl;
  r = c1->myFS->writeFile(f1, buf3, 5);
  cout << "rv from writeFile /a f1 is " << r <<(r==5 ? " Correct wrote 5 K": " failure") <<endl;
  r = c1->myFS->writeFile(f1, buf4, 5);
  cout << "rv from writeFile /a f1 is " << r <<(r==5 ? " Correct wrote 5 M": " failure") <<endl;
  r = c1->myFS->appendFile(f1, buf1, 5);
  cout << "rv from appendFile /a f1 is " << r <<(r==5 ? " Correct wrote 5 s": " failure") <<endl;

  r = c1->myFS->writeFile(f2, buf4, 600);
  cout << "rv from writeFile /b f2 is " << r <<(r==600 ? " Correct wrote 600 M": " failure") <<endl;
  
  f3 = c1->myFS->openFile(const_cast<char *>("/a"), 2, 'r', -1);
  cout << "rv from openFile /a is " << f3 << (f3>0 ? " Correct file opened (r) f3 ": " failure") <<endl;

  r = c1->myFS->writeFile(f3, buf4, 600);
  cout << "rv from writeFile /a f3 is " << r <<(r==-3 ? " Correct open is r": " failure") <<endl;
  r = c1->myFS->appendFile(f3, buf1, 5);
  cout << "rv from appendFile /a f3 is " << r <<(r==-3 ? " Correct open is r": " failure") <<endl;


  cout<<"\nread tests on fs1\n";
  f4 = c1->myFS->openFile(const_cast<char *>("/b"), 2, 'r', -1);
  cout << "rv from openFile /b is " << f4 << (f4>0 ? " Correct file opened (r) f4 ": " failure") <<endl;

  r = c1->myFS->readFile(f3, rbuf1, 25);
  cout << "rv from readFile /a f3 is " << r << (r==25 ? " Correct file read ": " failure") <<endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf1[i];
  cout << endl;

  r = c1->myFS->readFile(f3, rbuf1, 20);
  cout << "rv from readFile /a f3 is " << r << (r==0 ? " Correct file read end of file": " failure") <<endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf1[i];
  cout << endl;


  r = c1->myFS->readFile(f4, rbuf3, 65);
  cout << "rv from readFile /b f4 is " << r << (r==65 ? " Correct file read ": " failure") <<endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf3[i];
  cout << endl;

  r = c1->myFS->readFile(f2, rbuf3, 65);
  cout << "rv from readFile /b f2 is " << r << (r==0 ? " Correct file read end of file": " failure") <<endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf3[i];
  cout << endl;
 
  f5 = c1->myFS->openFile(const_cast<char *>("/c"), 2, 'r', -1);
  cout << "rv from openFile /c is " << f5 << (f5>0 ? " Correct file opened (r) f5 ": " failure") <<endl;

  r = c1->myFS->readFile(f5, rbuf1, 25);
  cout << "rv from readFile /c f5 is " << r << (r==0 ? " Correct file read ": " failure") <<endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf1[i];
  cout << endl;

  cout <<"\nSeek tests on fs1 (with read and writes)\n";
  r = c1->myFS->seekFile(f3, 0, -1);
  cout << "rv from seekFile /a f3 is " << r << (r==0 ? " Correct rw set to 0": " failure") <<endl;
  r = c1->myFS->seekFile(f3, -10 , -1);
  cout << "rv from seekFile /a f3 is " << r << (r==-1 ? " Correct failed to set rw to -10": " failure") <<endl;
  r = c1->myFS->seekFile(f3, 10, 0);
  cout << "rv from seekFile /a f3 is " << r << (r==0 ? " Correct rw offset to 10": " failure") <<endl;

  r = c1->myFS->readFile(f3, rbuf1, 20);
  cout << "rv from readFile /a f3 is " << r << (r==15 ? " Correct file read to end of file": " failure") <<endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf1[i];
  cout << endl;

  r = c1->myFS->seekFile(f2, -600, 0);
  cout << "rv from seekFile /b f2 is " << r << (r==0 ? " Correct rw set to 0, seek of -600": " failure") <<endl;
  r = c1->myFS->writeFile(f2, buf4, 10);
  cout << "rv from writeFile /b f2 is " << r <<(r==10 ? " Correct wrote 10 M": " failure") <<endl;
  r = c1->myFS->writeFile(f2, buf3, 10);
  cout << "rv from writeFile /b f2 is " << r <<(r==10 ? " Correct wrote 10 K": " failure") <<endl;
  r = c1->myFS->writeFile(f2, buf2, 10);
  cout << "rv from writeFile /b f2 is " << r <<(r==10 ? " Correct wrote 10 b": " failure") <<endl;
  r = c1->myFS->writeFile(f2, buf1, 10);
  cout << "rv from writeFile /b f2 is " << r <<(r==10 ? " Correct wrote 10 s": " failure") <<endl;

  r = c1->myFS->readFile(f2, rbuf1, 10);
  cout << "rv from readFile /b f2 is " << r << (r==10 ? " Correct file read of 10 Ms": " failure") <<endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf1[i];
  cout << endl;

  r = c1->myFS->seekFile(f2, -30, 0);
  cout << "rv from seekFile /b f2 is " << r << (r==0 ? " Correct rw set to 20, seek of -30": " failure") <<endl;
  r = c1->myFS->readFile(f2, rbuf1, 30);
  cout << "rv from readFile /b f2 is " << r << (r==30 ? " Correct file read ": " failure") <<endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf1[i];
  cout << endl;

  r = c1->myFS->seekFile(f2, 0, -1);
  cout << "rv from seekFile /b f2 is " << r << (r==0 ? " Correct rw set to 0": " failure") <<endl;
  r = c1->myFS->readFile(f2, rbuf2, 50);
  cout << "rv from readFile /b f2 is " << r << (r==50 ? " Correct file read ": " failure") <<endl;
  cout << "Data read is " << endl;
  for (i = 0; i < r; i++) cout << rbuf2[i];
  cout << endl;



  cout <<"close everything we are done in driver 1\n";
  r = c1->myFS->closeFile(f1);
  cout << "rv from closeFile f1 /a is " << r << (r==0 ? " Correct file closed": " failure")<<endl;
  r = c1->myFS->closeFile(f2);
  cout << "rv from closeFile f2 /b is " << r << (r==0 ? " Correct file closed": " failure")<<endl;
  r = c1->myFS->closeFile(f3);
  cout << "rv from closeFile f3 /a is " << r << (r==0 ? " Correct file closed": " failure")<<endl;
  r = c1->myFS->closeFile(f4);
  cout << "rv from closeFile f4 /b is " << r << (r==0 ? " Correct file closed": " failure")<<endl;
  r = c1->myFS->closeFile(f5);
  cout << "rv from closeFile f5 /c is " << r << (r==0 ? " Correct file closed": " failure")<<endl;

  cout <<"end driver 1\n";
  return 0;
}
