/* -----------------------------------------------------------------------------------------------------------------
COPYRIGHT (c) 2020 - 2120
Wave Knight Industry.,
ALL RIGHTS RESERVED

This software is a copyrighted work and/or information protected as a trade
secret. Legal rights of Wave Knight Industry. in this software are distinct from
ownership of any medium in which the software is embodied. Copyright or trade
secret notices included must be reproduced in any copies authorized by
Wave Knight Industry. The information in this software is subject to change without
notice and should not be considered as a commitment by Wave Knight Industry.
----------------------------------------------------------------------------------------------------------------- */
#pragma once
#include <fstream>
#include <iostream>
#include <direct.h>
#include<atltime.h>
#include <thread>
#include <map>
#include <vector>
#include <mutex>
#include <string>

#define WK_LOG_CORE_NORMAL_DELAY 60

struct Logs
{
  CTime log_time;
  std::string log;
  Logs() {
    log_time = CTime::GetCurrentTime();
  }
};

struct Log_Cell
{
  std::string log_type;
  std::string log_folder;
  std::vector<Logs> logs;

  Log_Cell(std::string logfolder) {
    log_folder = logfolder;
    logs.clear();
  }
};

class CWKLogCore
{
public:
  CWKLogCore();
  ~CWKLogCore();

  static CWKLogCore* GetInstance();

  void OnRelease();
  void OnTimerCheck(bool force_write = false);
  void WritingLog(const char* log_type, const char* str_log);

  bool GetEndMark() { return end_mark_; }

private:
  void StartLogLoop();
  void WritingCell(Log_Cell *log_cell);
  void CheckCreateFolder(std::string folder_path);

  bool end_mark_ = false;
  std::string logfolderpath_;
  std::map<std::string, Log_Cell> log_map_;
  std::mutex log_lock_;
  std::thread log_loop_;
};

