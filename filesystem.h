#include <vector>

using namespace std;
class FileSystem {
  DiskManager *myDM;
  PartitionManager *myPM;
  char myfileSystemName;
  int myfileSystemSize;
  int fileTable;
  
  
  /* declare other private members here */
  private:
    int uniqueid; //Used to generate unique IDs
    bool isOpen(char *filename);
  
    struct openTable{
		char mode;
		int rwPointer;
		char* name;
		int lockid; //-1 if not locked, or positive if locked
    };
    vector<openTable> fileOpenTable; //position number is filedescriptor
  
    struct lockTable{
		char* name;
		bool isLocked;
		int lockid;
		int flength;
    };
    vector<lockTable> fileLockTable;
    int incunique(/*This will generate a new unique id */);
    int fileExists(char* name, int len, int currentBlock, char type);
    bool isAlphabet(char test);
    int addToDirectory(char type, int address, char name, int containAddr);
    int readinghelp(int location, int end, int size, int &remaining, int &ptr, char* temp, char* data);
    int writinghelp(int location, int position, int &remaining, int len, int &ptr, char* temp, char* data);
    void intToChar(int pos, int num, char * buff);
    int charToInt(int pos, char * buff);



  public:

    FileSystem(DiskManager *dm, char fileSystemName);
    int createFile(char *filename, int fnameLen);
    int createDirectory(char *dirname, int dnameLen);
    int lockFile(char *filename, int fnameLen);
    int unlockFile(char *filename, int fnameLen, int lockId);
    int deleteFile(char *filename, int fnameLen);
    int deleteDirectory(char *dirname, int dnameLen);
    int openFile(char *filename, int fnameLen, char mode, int lockId);
    int closeFile(int fileDesc);
    int readFile(int fileDesc, char *data, int len);
    int writeFile(int fileDesc, char *data, int len);
    int appendFile(int fileDesc, char *data, int len);
    int seekFile(int fileDesc, int offset, int flag);
    int renameFile(char *filename1, int fnameLen1, char *filename2, int fnameLen2);
    int getAttributes(char *filename, int fnameLen, int request);
    int setAttributes(char *filename, int fnameLen, char color, int filerank, int request);
    /* declare other public members here */

};
