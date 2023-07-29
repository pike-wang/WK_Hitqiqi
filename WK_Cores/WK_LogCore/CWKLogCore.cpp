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
#include "CWKLogCore.h"

CWKLogCore log_core_;
using namespace std;

void LogLoop() {
  while (true) {
    Sleep(WK_LOG_CORE_NORMAL_DELAY * 1000);
    if (CWKLogCore::GetInstance()->GetEndMark())
      return;

    CWKLogCore::GetInstance()->OnTimerCheck();
  }
}

CWKLogCore::CWKLogCore() {
  log_map_.clear();

  char exeFullPath[MAX_PATH +1] = { 0 };
  GetModuleFileName(NULL, exeFullPath, MAX_PATH);

  std::string forlder_path = std::string(exeFullPath);
  int pos = forlder_path.find_last_of('\\', forlder_path.length());
  forlder_path = forlder_path.substr(0, pos+1);
  logfolderpath_ = forlder_path + _T("Log");

  StartLogLoop();
}

CWKLogCore::~CWKLogCore() {
  OnRelease();
}

CWKLogCore* CWKLogCore::GetInstance() {
  return &log_core_;
}

void CWKLogCore::OnRelease() {
  WritingLog("WK_LOG", "OnRelease");
  end_mark_ = true;
  OnTimerCheck(true);
}

void CWKLogCore::OnTimerCheck(bool force_write) {
  log_lock_.lock();

  if (log_map_.empty()) {
    log_lock_.unlock();
    return;
  }

  for (auto log_cell = log_map_.begin() ; log_cell != log_map_.end(); log_cell++) {
    if (log_cell->second.logs.empty())
      continue;

    WritingCell(&log_cell->second);
  }

  log_lock_.unlock();
}

void CWKLogCore::WritingLog(const char* log_type, const char* str_log) {
  if (!str_log || *str_log == 0)
    return;

  log_lock_.lock();

  std::string logtype;
  if (!log_type || *log_type == 0)
    logtype = _T("");
  else
    logtype = log_type;

  if (log_map_.find(logtype) == log_map_.end()) {
    Log_Cell log_cell(logfolderpath_ + _T("\\") + logtype);
    log_cell.log_type = log_type;
    log_map_.insert(make_pair(logtype, log_cell));
  }

  CTime time_now = CTime::GetCurrentTime();
  std::string log = time_now.Format("%Y-%m-%d %H:%M:%S").GetBuffer();
  log += _T(" ") + std::string(str_log);
  Logs log_in;
  log_in.log = log;

  auto iter = log_map_.find(logtype);
  iter->second.logs.push_back(log_in);

  log_lock_.unlock();
  return;
}

void CWKLogCore::StartLogLoop() {
  log_loop_ = std::thread(LogLoop);
  log_loop_.detach();
}

void CWKLogCore::CheckCreateFolder(std::string folder_path) {
  if (0 != _chdir(folder_path.c_str())) {
    _mkdir(folder_path.c_str());
  }
}

void CWKLogCore::WritingCell(Log_Cell* log_cell) {
  if (!log_cell || log_cell->logs.empty())
    return;

  CheckCreateFolder(logfolderpath_);
  if (!log_cell->log_folder.empty())
    CheckCreateFolder(log_cell->log_folder);

  std::string log_file_path = _T("");
  std::ofstream hFile;

  for (auto logs : log_cell->logs)
  {
    std::string log_type_path = _T("");
    if (!log_cell->log_type.empty())
      log_type_path = _T("\\") + log_cell->log_type + _T("-");

    std::string log_file_path_new = log_cell->log_folder + log_type_path + logs.log_time.Format("%Y-%m-%d_(%H-00-00)").GetBuffer() +_T(".wklog");

    if (!log_file_path._Equal(log_file_path_new)) {
      hFile.close();
      log_file_path = log_file_path_new;
      hFile.open(log_file_path.c_str(), std::ios::app | std::ios::out);
    }

    hFile.write(logs.log.c_str(), logs.log.length());
    hFile.write("\n", 1);
  }

  hFile.close();
  log_cell->logs.clear();
}