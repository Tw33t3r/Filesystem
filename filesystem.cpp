#include "disk.h"
#include "diskmanager.h"
#include "partitionmanager.h"
#include "filesystem.h"
#include <time.h>
#include <cstdlib>
#include <iostream>
#include <string.h>
using namespace std;


FileSystem::FileSystem(DiskManager *dm, char fileSystemName)
{
  //Create partitionManager to control this filesystem
  myDM = dm;
  myfileSystemSize = myDM->getPartitionSize(fileSystemName);
  //Pass information to a new partitionManager
  myPM = new PartitionManager(dm, fileSystemName, myfileSystemSize);
  myfileSystemName = fileSystemName;
  
  //Data for Directory
  char test[64] = {'0'};
  //Create directory for root
  //cout << "FS Con, writeDiskBlock " << dir << " to " << 1 << endl;
  myPM->readDiskBlock(1,test);
  if(test[1] == 'c'){
    char dir[64] = {'0'};
    myPM->writeDiskBlock(1,dir);
  }
}


int FileSystem::charToInt(int pos, char * buff) {
    char newChar[4];
    int i=pos;
    while(i-pos < 4){
        newChar[i-pos] = buff[i];
        i++;
    }
    int toReturn = atoi(newChar);
    return toReturn;
}


void FileSystem::intToChar(int pos, int num, char * buff) {
    /*
     * error checking doesn't work on this function properly
    if(num > 9999 || num < 0 || buff == NULL || pos < 0 || pos>=strlen(buff)-3)
    {
        cout<<"Error in intToChar arguments"<<endl;
    }
    */
    int temp;
    for(int i = 3; i >= 0; i--)
    {
        temp = num %10;
        num/=10;
        char c = temp + '0';
        buff[pos+i] = c;
    }
}

//-3 if can't read disk block
//-3 name less than 1
//-4 incorrect format (/n/n/n/n)
//-5 same name diff type
//-2 not in directory
//for this to work the directory needs to allocate as 0's
int FileSystem::fileExists(char* name, int len, int currentBlock, char type)
{
    //cout << "In fileExists" << endl;
    char data[64] = "";
    //cout << "Current Block is: " << currentBlock << endl;
    int test = myPM->readDiskBlock(currentBlock, data);
    //cout << "test is: " << test << endl;
    //cout << "data: " << data << endl;
    if(test !=0)
    {
        //cout << "Failure reading in fileExists read return is: " << test << " Current block is " << currentBlock << endl;
        return -3; //this had a reading failure
    }
    if(len < 1) // file name invalid
    {
        //cout << "Name less than 1 fileExists" << endl;
        return -3;
    }
    for(int i = 0; i < len; i++)
    {
        if(i % 2 == 0){
          //cout << "Checking name[i],i " << name[i] << i << endl;
            if(name[i] != '/') // name not in correct format
            {
                //cout << "Returning from fileExists not correct name format" << endl;
                return -4;
            }
        } 
        else
        {
            if(isAlphabet(name[i]) != true)
            {
                return -4; //name not in correct format
            }
            //we get here now we check for the name match, there are 10 files or directories in each block,
            for(int j = 0; j < 10; j++)
            {
                // this is where we will use the current block and data
                //get 1 byte and check name 
                char tempName = data[j*6+0];
                //cout << "tempName: " << tempName;
                //get 4 bytes and store for next pointer possibly
                int tempBlock = charToInt(j*6+1, data);
                //cout << " tempBlock: " << tempBlock;
                //get 1 byte and see if file or directory
                char tempType = data[j*6+5];
                //cout << " tempType: " << tempType << endl;
                if(tempName == name[i] && tempType == type && len -1 == i) // if it is the file then it exists 
                {
                    //cout << "FE, returning Block " << tempBlock << endl;
                    return tempBlock; // return block number inode is located
                }
                else if(tempName == name[i] && tempType != type && len -1 == i){
                  return -5;
                }
                else if(tempName == name[i] && tempType == 'd' && len -1 != i)
                {
                    //we will recall this function with tempBlock
                    //send with truncated name and new length
                    
                    char newName[len-2];
                    //cout << "newName is: ";
                    for(int k = 0; k < len-2; k++)
                    {
                        newName[k] = name[i+1+k];
                        //cout << newName[k];
                    }
                    //cout << endl;
                    int newLength = len-2;
                    //cout << "tempType: " << tempType;
                    //cout << " tempBlock: " << tempBlock << endl;
                    //cout << "start for recursion" << endl;
                    int temp = fileExists(newName, newLength, tempBlock, type);
                    //cout << "Return is: " << temp << endl;
                    return temp;
                }
            }
            int tempBlock = charToInt(60, data);
            //cout << "-------------------------------TempBlock is: " << tempBlock << endl;
            if(tempBlock == 0)
            {
                if(currentBlock == 1 && i < len -2)
                {
                    return -6;   
                }
                //cout << "FE, not in directory returning -2" << endl;
                return -2; //the file is not in the directories
            }
            else
            {
                //cout << "start for recursion part 2" << endl;
                int temp = fileExists(name, len, tempBlock, type);
                return temp;
            }
        }
    }
}

bool FileSystem::isAlphabet(char test)
{
    //checks if char is upper or lowercase alphabet
    if((test >= 97 && test <= 122)||(test >= 65 && test <= 90))
    {
        //cout << test << " is Alphabet true" << endl;
        return true;
    }
    else
    {
        //cout << test << " is is Alphabet false" << endl;
        return false;
    }
}
       
//Creates an Inode, and put link to inode in the root directory
int FileSystem::createFile(char *filename, int fnameLen)
{
  for(int i = 0; i < fnameLen; i++){
    if(i % 2 == 0){
      if(filename[i]!= '/'){
        return -3;
      }
    }
    else{
      if(!isAlphabet(filename[i])){
        return -3;
      }
    }
  }

    char data[64] = "";
    int exist = fileExists(filename, fnameLen, 1, 'f');
    //cout << "exist: " << exist << endl;
    if(exist == -3 || exist == -4)
    {
        //cout << "create returned a -3" << endl;
        return -3;
    }
    else if(exist == -5){
      return -4;
    }
    else if(exist == -6)
    {
        return -4;
    }
    else if(exist == -2)
    {
        // we will create the file iNode
        //check for a block, if cant get a block return -2
        int blocknumber = myPM->getFreeDiskBlock();
        if(blocknumber == -1)
        {
            return -2;
        }
        else
        {
            //here we get another block for the first block for data
            //create the file iNode
            char name = filename[fnameLen-1];
            data[0] = name;
            data[1] = 'f';
            int containAddr = 1;
            myPM->writeDiskBlock(blocknumber, data);
            
            if(fnameLen > 2){
                //find place to link iNode
                char containName[fnameLen-2];
                for(int i=0;i<(fnameLen-2);i++){
                    containName[i] = filename[i];
                }
                //Adress of the directory I need to put my inode in
                containAddr = fileExists(containName, fnameLen-2, 1, 'd');
            }
            //cout << "containAddr " << containAddr << endl;
            //Create link in directory
            int addToDirectoryReturn = addToDirectory('f', blocknumber, name, containAddr);
            if(addToDirectoryReturn == -2){
                return -2;
            }
            // no need to make the next two address or the indirect block yet         
            //now write to the disk the name, type, size and datablock in this blocknumber
            myPM->writeDiskBlock(blocknumber, data);
            lockTable lt;
            lt.name = filename;
            lt.isLocked = false;
            lt.lockid = -1;
            lt.flength = 0;
            fileLockTable.push_back(lt);
            return 0;
        }
    }
    else if(exist > 1)
    {
        // the file exists already
        return -1;
    }
    else
    {
        return -4; 
    }
}
       
//Creates directory Inode, link to inode in directory
int FileSystem::createDirectory(char *dirname, int dnameLen)
{
  for(int i = 0; i < dnameLen; i++){
    if(i % 2 == 0){
      if(dirname[i]!= '/'){
        //cout << dirname[i] << " doesn't exist" << endl;
        return -3;
      }
    }
    else{
      if(!isAlphabet(dirname[i])){
        //cout << dirname[i] << " doesn't exist" << endl;
        return -3;
      }
    }
  }

  int valid;
  valid = fileExists(dirname, dnameLen, 1, 'd');
  //cout << "After First File Exist call" << endl;
  //cout << "valid is : " << valid << endl;
  if(valid == -3|| valid == -4){
    //fileName invalid
      //cout << "createDirectory Filename invalid" << endl;
    return -3;
  }
  else if(valid == -2){
    //Create inode
    int blocknumber = myPM->getFreeDiskBlock();
    if(blocknumber == -1){
        return -2;
    }
    else{
        //create iNode
        char name = dirname[dnameLen-1];
        int containAddr = 1;
        if(dnameLen > 2){
        //find place to link iNode
            char containName[dnameLen-2];
            //cout << "containName is: ";
            for(int i=0;i<(dnameLen-2);i++){
                containName[i] = dirname[i];
                //cout << containName[i];
            }
            //cout << endl;
            //Adress of the directory I need to put my inode in
            containAddr = fileExists(containName, dnameLen-2, 1, 'd');
        }
        //Create link in directory
        //cout<<"ContainAddr is: " << containAddr << endl;
        int addToDirectoryReturn = addToDirectory('d', blocknumber, name, containAddr);
        if(addToDirectoryReturn == -2){
            return -2;
        }
    }
  }
  else if(valid == -5){
    return -4;
  }
  //if name already exists
  else if(valid > 0){
      return -1;
  }
  //Add to directory(helper function?)
  else{
    return -4;
  }
}

//returns -2 if not enough space for linked directory block
//Adds a link and related information to a directory
int FileSystem::addToDirectory(char type, int address, char name, int containAddr){
    //cout << "AddToDirectory: " << name << ", " << address << ", " << type << endl; 
    bool inserted = false;
    //Create a node to work with for this cycle through directory iNode
    char directoryNode[64] = "";
    int readContainVal = myPM->readDiskBlock(containAddr, directoryNode);
    if (readContainVal < 0){
        return -4;
    }
    //Check 0-59 on directory iNode
    for(int i=0;i<10;i++){
        //look at names of files
        if(!(isAlphabet(directoryNode[i*6]))){
            inserted = true;
            directoryNode[i*6] = name;
            intToChar(i*6+1,address,directoryNode);
            directoryNode[i*6+5] = type;
            //cout << "inserting: " << name << " to " << containAddr << endl;
            int toReturn = myPM->writeDiskBlock(containAddr, directoryNode);
            return toReturn;
        }
    }
    //No linked directory block
    int link = charToInt(60,directoryNode);
    if(link == 0){
      //cout << "linkdir" << endl;
        //newBlock will become new directory block.
        int newBlock = myPM->getFreeDiskBlock();
        //cout << "free disk block is: " << newBlock << endl;
        if (newBlock == -1){
            return -2;
        }
        else{
          //cout<<"adding " << newBlock << " to " << directoryNode << endl;
            intToChar(60,newBlock,directoryNode);
            myPM->writeDiskBlock(containAddr, directoryNode);
            char temp[64] = "";
            //for some reason we get c's so write 0's instead
            myPM->writeDiskBlock(newBlock, temp);
            return addToDirectory(type, address, name, newBlock);
        }
    }
    //access linked directory block
    else{
        int linkedAddr = charToInt(60, directoryNode);
        return addToDirectory(type, address, name, linkedAddr);
    }
}
            
       
int FileSystem::lockFile(char *filename, int fnameLen)
{
    bool filefound = false;
    for(int i = fileOpenTable.size()-1; i >=0 ; i--){
        if(fileOpenTable[i].name == filename){ return -3;} //The file is already open
    }
    //If we have reached this loop the file isn't in fileOpenTable
    for(int i = fileLockTable.size()-1; i >= 0; i--){
        if(fileLockTable[i].name == filename){ //If the file is found
            if(fileLockTable[i].isLocked){
                return -1; //File is already locked
            }
            fileLockTable[i].isLocked = true;
            fileLockTable[i].lockid = incunique();
            filefound = true;
            return fileLockTable[i].lockid;
        }
    }

    if(!filefound){ 
        int blknum = fileExists(filename, fnameLen, 1, 'f');
        if(blknum == -5)
        {
            return -4;   
        }
        if(blknum < 0)
        {
            return -2;
        }
        else
        {
            lockTable temp;
            temp.name = filename;
            char data[64] ={'0'};
            myPM->readDiskBlock(blknum, data);
            temp.flength = charToInt(2, data);
            temp.lockid = incunique();
            temp.isLocked = true;
            fileLockTable.push_back(temp);
            return temp.lockid;
        }        
    } //File doesn't exist
    return -4; //Something else happened
}
       
int FileSystem::unlockFile(char *filename, int fnameLen, int lockId)
{ 
    for(int i = fileOpenTable.size()-1; i >= 0; i--){
        if(fileOpenTable[i].name == filename){ return -3;} //The file is already open
    }
    //If we reach this loop, the file isn't in fileOpenTable
    for(int i = fileLockTable.size()-1; i >= 0 ; i--){
        if(fileLockTable[i].name == filename){ //If the file is found
           if(!fileLockTable[i].isLocked){ return -2; } //File is already unlocked
           //Verifies lock id is valid
           if(fileLockTable[i].lockid == lockId){
                fileLockTable[i].isLocked = false;
                fileLockTable[i].lockid = -1;
                return 0; //successful
           }
           else{
               return -1; // Invalid lock id
           }
        }
     }
    return -2; //Something else happened e.g. file was not found
}
    
//Remove indirect blocks, then direct blocks, then inode, then space from root  
int FileSystem::deleteFile(char *filename, int fnameLen)
{
  for(int i = 0; i < fnameLen; i++){
    if(i % 2 == 0){
      if(filename[i]!= '/'){
        return -3;
      }
    }
    else{
      if(!isAlphabet(filename[i])){
        return -3;
      }
    }
  }
  for(int i = fileOpenTable.size()-1; i>=0; i--)
  {
    if(filename == fileOpenTable[i].name)
    {
      return -2;
    }
  }
  for(int i = fileLockTable.size()-1; i >=0; i--)
  {
    if(filename == fileLockTable[i].name && fileLockTable[i].isLocked == true)
    {
      return -2;
    }
  }
  int blknum = fileExists(filename, fnameLen, 1, 'f');
  if(blknum == -4)
  {
    return -3;
  }
  else if (blknum == -2){
    return -1;
  }
  char temp[64];
  myPM->readDiskBlock(blknum, temp);
  int indirect = charToInt(18, temp);
  if(indirect != 0)
  {
    myPM->readDiskBlock(indirect, temp);
    for(int i = 0; i < 16; i++)
    {
      int location = charToInt(i*4, temp);
      if(location != 0)
      {
        myPM->returnDiskBlock(location);
      }
    }
    myPM->returnDiskBlock(indirect);
  }
  myPM->readDiskBlock(blknum, temp);
  //Get rid of direct in inode
  for(int i = 0; i < 3; i++)
  {
    int direct = charToInt(6+(i*4), temp);
    if(direct != 0)
    {
      myPM->returnDiskBlock(direct);
    }
  }
  myPM->returnDiskBlock(blknum);
      
  int containAddr = 1;
  if(fnameLen > 2){
    //find place to remove inode
    char containName[fnameLen-2];
    for(int i=0;i<(fnameLen-2);i++){
      containName[i] = filename[i];
    }
    //Adress of the directory I need to remove inode link
    containAddr = fileExists(containName, fnameLen-2, 1, 'd');
    //cout << "containAddr in delete file is: " << containAddr << endl;
  }
  int prevContain = -1;
  bool inserted = false;
  while(!inserted)
  {
    //read dr
    char cur[64] = "";
    myPM->readDiskBlock(containAddr, cur);
    //for each name in directory
    for(int i = 0; i < 10; i++)
    {
      //check the name for equals
      //cout << "Target is: "<< filename[fnameLen-1] << endl;
      if(cur[i*6] == filename[fnameLen-1] && cur[i*6+5] == 'f'){
        cur[i*6] = 0;
        cur[i*6+1] = 0;
        cur[i*6+2] = 0;
        cur[i*6+3] = 0;
        cur[i*6+4] = 0;
        cur[i*6+5] = 0;
        inserted = true;
        myPM->writeDiskBlock(containAddr,cur);
        int next = charToInt(60,cur);
        for(int j = 0; j < 10; j++){
          if(isAlphabet(cur[j])){
            break;
          }
          else if(j == 9 && prevContain > 0){
            char changed[64] = "";
            myPM->returnDiskBlock(containAddr);
            myPM->readDiskBlock(prevContain,changed);
            intToChar(60, next, changed);
            myPM->writeDiskBlock(prevContain,changed);
          }
        }
        break;
      }
    }
    prevContain = containAddr;
    containAddr = charToInt(60, cur);
    if(containAddr == 0 && inserted == false){
      return -3;
    }
  }
  return 0;
}
       
int FileSystem::deleteDirectory(char *dirname, int dnameLen)
{
    int blknum = fileExists(dirname, dnameLen, 1, 'd');
    //cout << "blknum is: " << blknum << endl;
    if(blknum == -3 || blknum == -4)
    {
        return -3;
    }
    else if(blknum < 0){
      return -1;
    }
    char data[64];
    myPM->readDiskBlock(blknum, data);
    bool flag = false;
    int last =charToInt(60, data);
    //cout << "Data is : ";
    for(int i = 0; i < 64; i++)
    {
        if(isAlphabet(data[i]))
        {
            flag = true;
        }
        else
        {
            flag = flag;
        }
        //cout << data[i];
    }
    //cout << endl;
    //cout << "flag and last are: " << flag  << " " << last << endl;
    if(flag == true || last != 0)
    {
        return -2;
    }
    myPM->returnDiskBlock(blknum);
    int containAddr = 1;
    if(dnameLen > 2)
    {
        //find place to link iNode
        char containName[dnameLen-2];
        for(int i=0;i<(dnameLen-2);i++){
            containName[i] = dirname[i];
        }
        //Adress of the directory I need to put my inode in
        containAddr = fileExists(containName, dnameLen-2, 1, 'd');
    }
    bool inserted = false;
    while(!inserted)
    {
        //read dr
        char cur[64] = "";
        myPM->readDiskBlock(containAddr, cur);
        //for each name in directory
        for(int i = 0; i < 10; i++)
        {
            //check the name for equals
            //cout << "name and type are: " << cur[i*6] << " " << cur[i*6+5]<< endl;
            if(cur[i*6] == dirname[dnameLen-1] && cur[i*6+5] == 'd')
            {
                cur[i*6] = 0;
                cur[i*6+1] = 0;
                cur[i*6+2] = 0;
                cur[i*6+3] = 0;
                cur[i*6+4] = 0;
                cur[i*6+5] = 0;
                inserted = true;
                myPM->writeDiskBlock(containAddr,cur);
                break;
            }
        }
        containAddr = charToInt(60, cur);
        if(containAddr == 0 && inserted == false)
        {
            return -3;
        }
    }
    return 0;
}
       
//Recursively look through directory until either file found or file/directory doesn't exist
int FileSystem::openFile(char *filename, int fnameLen, char mode, int lockId)
{
    int exists = fileExists(filename, fnameLen, 1, 'f');
    int flen=0;
    if(exists == -3||exists == -4 || exists == -5) // file name has improper name
    {
        return -1;
    }
    else if(exists == -2) // file does not exist
    {
        return -1;
    }
    else if(!(mode == 'w' || mode == 'r' || mode == 'm'))
    {
        return -2;
    }
    else 
    {
        // need to check locking restrictions
        bool flag = false;
        for(int i = fileLockTable.size()-1; i >= 0; i--)
        {
          //cout << "fltSize " << endl;
            if(fileLockTable[i].name == filename && fileLockTable[i].isLocked == true)
            {
              //cout << "fltName, isL "<< endl;
                if(fileLockTable[i].lockid == lockId)
                {
                  //cout<<"lockID"<<endl;
                    flag = true;
		    char temp[64]={'0'};
		    myPM->readDiskBlock(exists, temp);
		    flen = charToInt(2, temp);
                    break;
                }
                else
                {
                    return -3;
                }
            }
	    else if(fileLockTable[i].name == filename && fileLockTable[i].isLocked == false && lockId == -1)
	    {
              //cout << "LockID: " << lockId << endl;
		char temp[64]={'0'};
		myPM->readDiskBlock(exists, temp);
		flen = charToInt(2, temp);
		flag = true;
		break;
	    }
        }
        if(lockId != -1 && flag == false)
        {
            return -3;
        }
        
        //now if make it here then write to openfileTable
        openTable temp;
        temp.mode = mode;
        temp.rwPointer = 0;
        temp.name = filename;
        fileOpenTable.push_back(temp);
        return fileOpenTable.size(); // return the file descriptor       
    }
}
       

/*Close file with file descriptor filedesc.
 * Return -1 if filedesc is invalid
 * Return 0 if successful
 * Return -2 for anything else*/
int FileSystem::closeFile(int fileDesc)
{
  fileDesc = fileDesc -1;
    if(fileDesc > fileOpenTable.size() -1 || fileDesc < 0) {return -1;} //Checks validity of fileDesc
    else if(fileOpenTable[fileDesc].mode == '0'){
      return -1;
    }
    else{
      //fileOpenTable
      char fOTName[1] = {'0'};
        fileOpenTable[fileDesc].name = fOTName;
        fileOpenTable[fileDesc].rwPointer = 0;
        fileOpenTable[fileDesc].mode = '0';
        fileOpenTable[fileDesc].lockid = -1;
        //lockTable
        lockTable temp;
        temp.name = fileOpenTable[fileDesc].name; //Adds file to locktable
        temp.isLocked = false;
        temp.lockid = -1;
        fileLockTable.push_back(temp);
        return 0; //successful
    }
    return -2; //If we are here, something went wrong so return -2
    
}
       
/*
returns:
-1 if filedes is invalid
-2 if len is negative
-3 if operation is not permitted
number of bytes if successful
*/
int FileSystem::readFile(int fileDesc, char *data, int len)
{
  fileDesc = fileDesc -1;
    if(fileDesc > fileOpenTable.size() -1 || fileDesc < 0) {return -1;}
    if(len < 0) {return -2;}
    if(fileOpenTable[fileDesc].mode == 'w'||fileOpenTable[fileDesc].mode == '0') {return -3;}
    char* fname = fileOpenTable[fileDesc].name;
    //find rwPointer
    int ptr = fileOpenTable[fileDesc].rwPointer;
    int homeDirAddr = fileExists(fname, strlen(fname), 1, 'f');
    char temp[64] = {'0'};
    char home[64] = {'0'};
    char indirectNode[64] = {'0'};
    //read Inode
    myPM->readDiskBlock(homeDirAddr, home);
    int size = charToInt(2, home);
    //get address of indirect node
    int indirectAddr = charToInt(18, home);
    int remaining = 0;

    int read = 0;
    int start = ptr;
    while(read < len){
      int iNodeBlocknum = ptr / 64;
      if(iNodeBlocknum < 3){
        myPM->readDiskBlock(homeDirAddr,home);
        //location of our data address
        int dataAddrLoc = (iNodeBlocknum*4)+6;
        //address of data
        int dataAddr = charToInt(dataAddrLoc,home);
        //check if block exists
        if(dataAddr==0){
          fileOpenTable[fileDesc].rwPointer = ptr;
          return read;
        }
        else{
          //read data from block
          myPM->readDiskBlock(dataAddr,temp);
          int startPtr = ptr - (64*iNodeBlocknum);
          for(int i = startPtr; i < 64; i++){
            if(read == len){
              break;
            }
            if(read == size - start){
              fileOpenTable[fileDesc].rwPointer = ptr;
              return read;
            }
            data[read] = temp[i];
            read++;
            ptr++;
          }
        }
      }
      else{
        if(indirectAddr == 0){
          fileOpenTable[fileDesc].rwPointer = ptr;
          return read;
        }
        //get indirect node
        myPM->readDiskBlock(indirectAddr, indirectNode);

        //Find location of indirect data
        int dataIndrAddrLoc = ((iNodeBlocknum-3)*4);
        //find address of indirect dataAddr
        int dataIndrAddr = charToInt(dataIndrAddrLoc,indirectNode);
        //check for data
        if(dataIndrAddr == 0){
          fileOpenTable[fileDesc].rwPointer = ptr;
          return read;
        }
        //read data from indirect
        myPM->readDiskBlock(dataIndrAddr,temp);
        int startPtr = ptr - (64*iNodeBlocknum);
        for(int i = startPtr; i < 64; i++){
          if(read == len){
            break;
          }
          if(read == size - start){
            fileOpenTable[fileDesc].rwPointer = ptr;
            return read;
          }
          data[read] = temp[i];
          read++;
          ptr++;
        }
      }
    }
    fileOpenTable[fileDesc].rwPointer = ptr;
    return read;
}

/*
 * 
returns:
-1 if filedes is invalid
-2 if len is negative
-3 if operation is not permitted
number of bytes if successful
*/
int FileSystem::writeFile(int fileDesc, char *data, int len)
{
    fileDesc = fileDesc-1;
    if(fileDesc > fileOpenTable.size() -1 || fileDesc < 0) {return -1;}
    if(len < 0) {return -2;}
    if(fileOpenTable[fileDesc].mode == 'r'||fileOpenTable[fileDesc].mode == '0') {return -3;}
    char* fname = fileOpenTable[fileDesc].name;
    //Find address for fileExists
    int homeDirAddr = fileExists(fname, strlen(fname), 1, 'f');
    //check the size before from iNode
    char preSizeV[64] = "";
    myPM->readDiskBlock(homeDirAddr,preSizeV);
    int preSize = charToInt(2, preSizeV);
    int newSize = preSize;
    //Find rwPointer to write data here.
    int ptr = fileOpenTable[fileDesc].rwPointer;
    //cout << "ptr is: " << ptr << endl;
    if(ptr+len > 1216){
      //cout << "ptr plus len invalid" << endl;
      //cout << ptr << " " << len << endl;
      return -3;
    }
    int written = 0;
    char temp[64] = "";
    char home[64] = "";
    char indirectNode[64] = "";
    //readInode
    myPM->readDiskBlock(homeDirAddr, home);
    int indirectAddr = charToInt(18,home);
    while(written < len){
      //find which blocknum of the inode we are in
      int iNodeBlocknum = ptr / 64;
      //cout << "iNodeBlocknum " << iNodeBlocknum << endl;
      if(iNodeBlocknum < 3){
        //Find location of dataAddr
        int dataAddrLoc = (iNodeBlocknum*4)+6;
        //Find the address of data
        int dataAddr = charToInt(dataAddrLoc,home);
        //Check if there's an available block
        if(dataAddr == 0){
          dataAddr = myPM->getFreeDiskBlock();
          if(dataAddr == -1)
          {
            if(written == 0){
              //cout << "no Available block for direct" << endl;
              return -3;
            }
            //set FileOpenTableValues
            int writeEndSize = ptr;
            if(writeEndSize > preSize){
              newSize = writeEndSize;
            }
            fileOpenTable[fileDesc].rwPointer = ptr;
            //Write new length to homeDirectory
            myPM->readDiskBlock(homeDirAddr, home);
            intToChar(2, newSize, home);
            myPM->writeDiskBlock(homeDirAddr, home);
            return written;  
          }
          //Link new Data block
          intToChar(dataAddrLoc,dataAddr,home);
          //cout << "Linking New DataBlock" << endl;
          myPM->writeDiskBlock(homeDirAddr, home);
        }
        //Read Data Block
        myPM->readDiskBlock(dataAddr, temp);
        int startPtr = ptr - (64*iNodeBlocknum);
        for(int i = startPtr; i < 64; i++){
          if(written == len){
            break;
          }
          temp[i] = data[written];
          written++;
          ptr++;
        }
        //cout << "wrote: " << written << " block in file: " << iNodeBlocknum << endl;
        //write to our dataBlock
        myPM->writeDiskBlock(dataAddr, temp);
      }
      //indirect
      else{
        if(indirectAddr == 0){
          //create an indirectAddr
          indirectAddr = myPM->getFreeDiskBlock();
          if(indirectAddr == -1){
            if (written == 0){
              return -3;
            }
            //set FileOpenTableValues
            int writeEndSize = ptr;
            if(writeEndSize > preSize){
              newSize = writeEndSize;
            }
            fileOpenTable[fileDesc].rwPointer = ptr;
            //Write new length to homeDirectory
            myPM->readDiskBlock(homeDirAddr, home);
            intToChar(2, newSize, temp);
            myPM->writeDiskBlock(homeDirAddr, home);
            return written;
          }
          //link new indirect
          intToChar(18,indirectAddr,home);
          //cout << "Linking new indirectBlock" << endl;
          myPM->writeDiskBlock(homeDirAddr, home);
        }
        
        myPM->readDiskBlock(indirectAddr,indirectNode);
        //Find location of Indirect Data
        int dataIndrAddrLoc = ((iNodeBlocknum-3)*4);
        //Find the address of Indirect Data
        int dataIndrAddr = charToInt(dataIndrAddrLoc,indirectNode);
        //cout << "dataIndrAddr, dataindradddr " << dataIndrAddrLoc << dataIndrAddr << endl;
        //Check if there's an available block
        if(dataIndrAddr == 0){
          dataIndrAddr = myPM->getFreeDiskBlock();
          if(dataIndrAddr == -1)
          {
            if(written == 0){
              return -3;
            }
            //set FileOpenTableValues
            int writeEndSize = ptr;
            if(writeEndSize > preSize){
              newSize = writeEndSize;
            }
            fileOpenTable[fileDesc].rwPointer = ptr;
            //Write new length to homeDirectory
            myPM->readDiskBlock(homeDirAddr, home);
            intToChar(2, newSize, home);
            myPM->writeDiskBlock(homeDirAddr, home);
            return written;  
          }
          //Link new Indirect Data block
          intToChar(dataIndrAddrLoc,dataIndrAddr,indirectNode);
          //cout << "Linking New DataBlock" << endl;
          myPM->writeDiskBlock(indirectAddr, indirectNode);
        }
        //Read Indirect Data Block
        myPM->readDiskBlock(dataIndrAddr, temp);
        int startPtr = ptr - (64*iNodeBlocknum);
        for(int i = startPtr; i < 64; i++){
          if(written == len){
            break;
          }
          temp[i] = data[written];
          written++;
          ptr++;
        }
        myPM->writeDiskBlock(dataIndrAddr, temp);
      }
    }
    //set FileOpenTableValues
    int writeEndSize = ptr;
    if(writeEndSize > preSize){
      newSize = writeEndSize;
    }
    fileOpenTable[fileDesc].rwPointer = ptr;
    //Write new length to homeDirectory
    myPM->readDiskBlock(homeDirAddr, home);
    intToChar(2, newSize, home);
    myPM->writeDiskBlock(homeDirAddr, home);
    //cout << ptr;
    return written;
}

/*
returns:
-1 if filedes is invalid
-2 if len is negative
-3 if operation is not permitted
number of bytes if successful
*/
int FileSystem::appendFile(int fileDesc, char *data, int len)
{
  fileDesc = fileDesc-1;
  if(fileDesc > fileOpenTable.size() -1 || fileDesc < 0) {return -1;}
  if(len < 0) {return -2;}
  if(fileOpenTable[fileDesc].mode == 'r'||fileOpenTable[fileDesc].mode == '0') {return -3;}
  char* fname = fileOpenTable[fileDesc].name;
  int homeDirAddr = fileExists(fname, strlen(fname), 1, 'f');
  char home[64] = "";
  myPM->readDiskBlock(homeDirAddr,home);
  int fSize = charToInt(2, home);
  int check = seekFile(fileDesc+1, fSize,-1);
  //cout << check << endl;
  //cout << "append, fileSize: " << fSize << endl;
  return writeFile(fileDesc+1,data,len);
}
       
/*If flag = 0, move rwpointer offset bytes forward
 *Otherwise, rwPointer is set to offset.
 *Return -1 if flag, filedesc, offset, is invalid  (offset can be negative!) 
 *Return -2 if outside of bounds (end or beginning of file)
 *Return 0 if successful*/  
int FileSystem::seekFile(int fileDesc, int offset, int flag)
{
    fileDesc = fileDesc-1;
    if(fileDesc > fileOpenTable.size() -1 || fileDesc < 0) {return -1;} //Checks filedesc
    if(flag == 0)
    {
        int pos = fileOpenTable[fileDesc].rwPointer;
        char * name = fileOpenTable[fileDesc].name;
        int location = fileExists(name, strlen(name), 1, 'f');
        char data[64] = "";
        myPM->readDiskBlock(location, data);
        int size = charToInt(2, data);

        if(pos+offset > size || pos+offset < 0)
        {
            return -2;
        }
        else
        {
            fileOpenTable[fileDesc].rwPointer = pos + offset;
            //cout << "rwPointer is: " << fileOpenTable[fileDesc].rwPointer<< endl;
            return 0;     
        }
    }
    else
    {
        //cout << "Size is: " << fileOpenTable[fileDesc].flength << endl;
        char * name = fileOpenTable[fileDesc].name;
        int location = fileExists(name, strlen(name), 1, 'f');
        char data[64] = "";
        myPM->readDiskBlock(location, data);
        int size = charToInt(2, data);
        if(offset < 0 || offset > size)
        {
            return -1;
        }
        else
        {
            fileOpenTable[fileDesc].rwPointer = offset;
            //cout << "rwPointer is: " << fileOpenTable[fileDesc].rwPointer << endl;
            return 0;
        }
    }
}
       
/* Return 0 if successful
 * Return -1 for invalid file name
 * Return -2 if file does not exist
 * Return -3 if filename2 already exists
 * Return -4 if file is open or locked
 * Return -5 for anything else
*/
int FileSystem::renameFile(char *filename1, int fnameLen1, char *filename2, int fnameLen2)
{ 
    int blknum1 = fileExists(filename1, fnameLen1, 1, 'f');
    if(blknum1 == -3||blknum1 ==-4)
    {
        return -1; //invalid file name
    }
    else if(blknum1 == -2)
    {
        return -2; //file doesnt exist	
    }
    if(fnameLen1 != fnameLen2)
    {
        return -1;
    }
    for(int i = 0; i <= fnameLen1-2; i++)
    {
        if(filename1[i] != filename2[i])
        {
            return -1;
        }
    }
    int blknum2 = fileExists(filename2, fnameLen2, 1, 'f');
    if(blknum2 == -3 || blknum2 == -4)
    {
        return -1; //invalid file name
    }
    else if(blknum2 > 0)
    {
        return -3; //file already exists
    }
    for(int i = fileOpenTable.size()-1; i >=0; i--)
    {
        if(fileOpenTable[i].name == filename1)
        {
            return -4;
        }
    }
    for(int i = fileLockTable.size()-1; i >=0; i--)
    {
        if(fileLockTable[i].name == filename1 && fileLockTable[i].isLocked == true)
        {
            return -4;	
        }
    }
    char data[64];
    char fn2 = filename2[fnameLen2-1];
    char fn1 = filename1[fnameLen1-1];
    myPM->readDiskBlock(blknum1, data);
    data[0] = fn2;
    myPM->writeDiskBlock(blknum1, data);
    int containAddr = 1;
    if(fnameLen1 > 2)
    {
        //find place to link iNode
        char containName[fnameLen1-2];
        for(int i=0;i<(fnameLen1-2);i++){
            containName[i] = filename1[i];
        }
        //Adress of the directory I need to put my inode in
        containAddr = fileExists(containName, fnameLen1-2, 1, 'd');
    }
    bool inserted = false;
    while(!inserted)
    {
        //read dr
        char cur[64] = {'0'};
        myPM->readDiskBlock(containAddr, cur);
        //for each name in directory
        for(int i = 0; i < 10; i++)
        {
            //check the name for equals
            if(cur[i*6] == fn1)
            {
                cur[i*6] = fn2;
                //cout << "cur is " << cur << endl;
                //cout << "fn1,2 " << fn1 << fn2 << endl;
                inserted = true;
                myPM->writeDiskBlock(containAddr,cur);
                break;
            }
        }
        containAddr = charToInt(60, cur);
        if(containAddr == 0 && inserted == false)
        {
            return -5;
        }      
    }
    return 0;
        
}
 

int FileSystem::getAttributes(char *filename, int fnameLen, int request) //can only get one attribute at a time
{
    int blknum = fileExists(filename, fnameLen, 1, 'f');
    if(blknum < 0)
    {
        return -1;
    }
    char data[64] = "";
    myPM->readDiskBlock(blknum, data);
    if(request == 1)
    {
        //return attribute 1
        if(data[39] == 'r' || data[39] == 'y' || data[39] == 'b')
        {
            return data[39];
        }
        else
        {
            return -1;
        }
    }
    else if(request == 2)
    {
        //return attribute 2
        if(data[40] == 0)
        {
            return -1;
        }
        else
        {
            return charToInt(40, data);
        }
    }
    else
    {
        return -1;
    }
    

}
       
int FileSystem::setAttributes(char *filename, int fnameLen, char color, int filerank, int request) // request 1 for set color, 2 for set rank, 3 for set both
{
    int fileLoc = fileExists(filename, fnameLen, 1, 'f');
    if(fileLoc < 0)
    {
        return -1;
    }
    char data[64];
    myPM->readDiskBlock(fileLoc, data);
    if(request == 1 || request == 2 || request == 3)
    {
        if(request == 1 || request == 3)
        {
            if(color == 'r' || color == 'y' || color == 'b')
            {
                data[39] = color;
                myPM->writeDiskBlock(fileLoc, data);
            }
            else
            {
                return -1;
            }
        }
        if(request==2 || request == 3)
        {
            if(filerank >= 0 && filerank <= 9999)
            {
                intToChar(40, filerank, data);
                myPM->writeDiskBlock(fileLoc, data);
            }
            else
            {
                return -1;
            }
        }
        return 1;
    }
    else
    {
        return -1;
    }
}


int FileSystem::incunique(){
  uniqueid = uniqueid + 1;
  return uniqueid;
}
