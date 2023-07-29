#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include<atltime.h>
#include <thread>
#include <vector>
#include <set>
#include <mutex>
//#include <string>

#include "WK_Common_Define.h"
#include "WK_Hitqiqi_def.h"
#include "WK_LogCore_Interface.h"
#include "CServerNet.h"

#pragma comment(lib, "WK_LogCore.lib") 

class CServerCore
{
public:
  static CServerCore* GetInstance();

  WK_Bool Init();
  WK_Bool StratServer();
  WK_Bool ServerLoop();
  WK_Bool HandleClientMsg(HITQIQIMSG_PACKAGE* recv_package, void* socketin);
  WK_Bool OnCommandOffline();

private:

  WK_Bool OnSyncQiqi();

  WK_Bool ReadQiqiData();
  WK_Bool StartServerLoop();
  WK_Bool ReportQiqiStatus();
  WK_Bool OnQiqiHPChange(int value);
  WK_Bool OnPostQuitMsg(HITQIQIMSG_PACKAGE* recv_package, void* socketin);
  WK_Bool OnAskQiqiData(HITQIQIMSG_PACKAGE* recv_package, void* socketin);
  WK_Bool OnBetraier(HITQIQIMSG_PACKAGE* recv_package, void* socketin);
  WK_Bool OnAskAllon(HITQIQIMSG_PACKAGE* recv_package, void* socketin);
  WK_Bool OnTryBroken(HITQIQIMSG_PACKAGE* recv_package, void* socketin);
  WK_Bool OnHit(HITQIQIMSG_PACKAGE* recv_package, void* socketin);
  WK_Bool OnLvlupGet(HITQIQIMSG_PACKAGE* recv_package);
  WK_Bool SendNotify(HITQIQIMSG_PACKAGE* recv_package, void* socket = nullptr);
  WK_Bool OnRotAmmor(HITQIQIMSG_PACKAGE* recv_package);
  WK_Bool RecordHit(HITQIQIMSG_PACKAGE* recv_package);
  WK_Bool OnQiqiTimeMin();
  WK_Bool OnQiqiEnergyChange(int value);
  WK_Bool OnQiqiDead();
  WK_Bool RefershHitRecord();
  WK_Bool TestAllon(char* name);

  WK_Bool OnEnergyCharger(HITQIQIMSG_PACKAGE* recv_package);
  WK_Bool OnIntoDefense();
  WK_Bool InitDefense(QIQI_STATUS  status);
  WK_Bool OnDefense_Time();
  WK_Bool OnDefense_Hit(HIT_INFO& hit_info);
  WK_Bool OnQuitDefense(QIQI_STATUS  status);
  WK_Bool CheckShieldBroken();
  WK_Bool CheckFishBroken();
  WK_Bool CheckBladeAmmorBroken();
  WK_Bool OnSentryHit(HITQIQIMSG_PACKAGE* recv_package, void* socketin);
  WK_Bool OnObserverHit(HITQIQIMSG_PACKAGE* recv_package, void* socketin);
  WK_Bool OnChangeAbleReward(HITQIQIMSG_PACKAGE* recv_package, void* socketin);
 
  static CServerCore* g_instance;

  CTime m_ServerStartTime;
  CServerNet m_Net;
  QIQI_DATA m_qiqi;
  HITQIQI_DEFENSE_VALUE m_qiqi_def;

  int command_cnt = 0;
  std::mutex hit_record_lock;
  std::vector<HITQIQI_SERVER_HIT_RECORD> m_vHitRecord;
 
  std::thread m_ServerLoop;
};

