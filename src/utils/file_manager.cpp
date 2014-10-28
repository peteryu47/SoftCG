#include "utils/file_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include <windows.h>

FileManager::FileManager()
{

}

FileManager::~FileManager()
{

}

char* FileManager::GetFileData(char* file_path, int *file_len)
{
  //WCHAR buffer[400];
  //GetModuleFileName(NULL, buffer, 400);

  FILE *file = fopen(file_path, "rb");
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

bool FileManager::WriteData2File(char* file_path, char* data, int data_len)
{
  return true;
}

bool FileManager::IsFileExist(char* file_path)
{
  return true;
}

bool FileManager::IsDirExist(char* dir_path)
{
  return true;
}

bool FileManager::CreateFile1(char* file_path)
{
  return true;
}

bool FileManager::CreateDir(char* dir_path)
{
  return true;
}

bool FileManager::RemoveFile(char* file_path)
{
  return true;
}

bool FileManager::RemoveDir(char* dir_path)
{
  return true;
}
