#ifndef __FILE_MANAGER_H__
#define __FILE_MANAGER_H__

#include "utils/macro_utils.h"
#include "utils/defines.h"

class FileManager
{
private:
  FileManager();
public:
  ~FileManager();
  DISALLOW_COPY_AND_ASSIGN(FileManager);

  static FileManager* GetInstance(){static FileManager file_manager; return &file_manager;}

  char* GetFileData(char* file_path, int *file_len);
  bool  WriteData2File(char* file_path, char* data, int data_len);
  bool  IsFileExist(char* file_path);
  bool  IsDirExist(char* dir_path);
  bool  CreateFile1(char* file_path);
  bool  CreateDir(char* dir_path);
  bool  RemoveFile(char* file_path);
  bool  RemoveDir(char* dir_path);
};

#endif