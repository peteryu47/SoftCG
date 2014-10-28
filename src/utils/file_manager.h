#ifndef __FILE_MANAGER_H__
#define __FILE_MANAGER_H__

#include "utils/macro_utils.h"
#include "utils/defines.h"

#include <vector>

using namespace std;

class FileManager
{
private:
  FileManager();
public:
  ~FileManager();
  DISALLOW_COPY_AND_ASSIGN(FileManager);

  static FileManager* GetInstance(){static FileManager file_manager; return &file_manager;}

  char* GetFileData(const char* file_path, int *file_len);                //r-path
  bool  WriteData2File(const char* file_path, char* data, int data_len);  //a-path
  bool  IsFileExist(const char* file_path);                               //a-path
  bool  IsDirExist(const char* dir_path);                                 //a-path
  bool  CreateDir(const char* dir_path);                                  //a-path
  bool  RemoveFile(const char* file_path);                                //a-path
  bool  RemoveDir(const char* dir_path);                                  //a-path

  char* GetFullPath(const char* path);                                    //r-path, return a-path
  void  AddSearchPath(const char* path);                                  //a-path

private:
  vector<string> search_paths_;
};

#endif