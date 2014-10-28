#include "utils/file_manager.h"

#include <io.h>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>

FileManager::FileManager()
{
  char szPathTemp[1000] = {0};
  GetModuleFileNameA(NULL, szPathTemp, 1000);
  int szPathLen = strlen(szPathTemp);
  for(int i = szPathLen - 1; i >= 0; --i)
  {
    if(szPathTemp[i] == '\\')
    {
      szPathTemp[i] = '\0';
      break;
    }
  }
  string path = szPathTemp;
  path.append("\\..\\..\\..\\res\\");
  search_paths_.push_back(path);
}

FileManager::~FileManager()
{

}

char* FileManager::GetFileData(const char* file_path, int *file_len)
{
  char* path = GetFullPath(file_path);
  FILE *file = fopen(path, "rb");
  SAFE_DELETE_N(path);

  if(!file)
    return NULL;

  fseek(file, 0, SEEK_SET);
  fseek(file, 0, SEEK_END);
  *file_len = ftell(file);// longBytes就是文件的长度
  char* data = new char[*file_len + 1];
  fseek(file, 0, SEEK_SET);
  fread(data, sizeof(char), *file_len, file);
  data[*file_len] = '\0';

  fclose(file);
  return data;
}

bool FileManager::WriteData2File(const char* file_path, char* data, int data_len)
{
  return true;
}

bool FileManager::IsFileExist(const char* file_path)
{
  if(_access(file_path, 0) != -1)
    return true;
  return false;
}

bool FileManager::IsDirExist(const char* dir_path)
{
  if(_access(dir_path, 0) != -1)
    return true;
  return false;
}

bool FileManager::CreateDir(const char* dir_path)
{
  if(mkdir(dir_path) == -1)
    return false;
  return true;
}

bool FileManager::RemoveFile(const char* file_path)
{
  return true;
}

bool FileManager::RemoveDir(const char* dir_path)
{
  return true;
}

char* FileManager::GetFullPath(const char* path )
{
  for(vector<string>::iterator itr = search_paths_.begin(); itr != search_paths_.end(); ++itr)
  {
    string full_path = itr->append(path);
    if(IsFileExist(full_path.c_str()))
    {
      char *str = new char[full_path.size() + 1];
      memcpy(str, full_path.c_str(), sizeof(char) * full_path.size());
      str[full_path.size()] = '\0';
      return str;
    }
    else
      return NULL;
  }
  return NULL;
}

void FileManager::AddSearchPath(const char* path )
{
  search_paths_.push_back(path);
}
