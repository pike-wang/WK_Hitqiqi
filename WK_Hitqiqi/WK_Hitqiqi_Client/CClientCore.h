#pragma once

//#define _CRT_SECURE_NO_WARNINGS

#include <thread>
#include <cstdlib>
#include "WK_Common_Define.h"
#include "WK_LogCore_Interface.h"
#include "CClientUserData.h"
#include "CClientNet.h"

#pragma comment(lib, "WK_LogCore.lib") 

class CClientCore
{
public:
  static CClientCore* GetInstance();
  WK_Bool Init();
  WK_Bool MsgLoop();
  WK_Bool UserLoop();

  QIQI_DATA* GetQiqiData() { return &m_qiqi;}
  void SetHWND(HWND hwnd) { m_hwnd = hwnd; }

  double GetUserEnergy(){ return m_User.GetUserEnergy(); }
  double GetUserExpPer() { return m_User.GetUserExpPer(); }

  WK_Bool OnHit(POINT ptMouse);

  void SaveAll() { m_User.WriteIn(); }
private:

  WK_Bool StartMsgLoop();
  WK_Bool StartUserLoop();
  WK_Bool OnConnectError();

  WK_Bool HandleServerMsg(HITQIQIMSG_PACKAGE *recv_package);
  WK_Bool OnUpdataQiqiData(HITQIQIMSG_PACKAGE* recv_package);
  WK_Bool OnShowNotify(HITQIQIMSG_PACKAGE* recv_package);
  WK_Bool OnAllonReply(HITQIQIMSG_PACKAGE* recv_package);
  WK_Bool OnCommandStauts(HITQIQIMSG_PACKAGE* recv_package);
  WK_Bool OnBetraier(HITQIQIMSG_PACKAGE* recv_package);
  WK_Bool OnBetraierObtain(HITQIQIMSG_PACKAGE* recv_package);
  WK_Bool OnUpdataBrokenTime(HITQIQIMSG_PACKAGE* recv_package);
  WK_Bool OnBladeAmmorEnergyMin(HITQIQIMSG_PACKAGE* recv_package);
  WK_Bool OnSmokeExpMin(HITQIQIMSG_PACKAGE* recv_package);
  WK_Bool OnChangeAbleReward(HITQIQIMSG_PACKAGE* recv_package);

  WK_Bool UserEnergyObtain();
  void OnGift_Time();
  void OnAddExp(int exp);
  WK_Bool ChangeUserEnergy(double energy);
  void OnGift_Hit(const HIT_INFO& hit_info);
  WK_Bool OnEnergyBeenCharger(HITQIQIMSG_PACKAGE* recv_package);
  WK_Bool AskIfAllon(int &damage);

  WK_Bool SendRequest(
    HITQIQIMSG_EVENT event, 
    HITQIQIMSG_SUBEVENT subevent = HITQIQIMSG_SUBEVENT_ERROR, 
    HITQIQIMSG_SUBSUBEVENT subsubevent = HITQIQIMSG_SUBSUBEVENT_ERROR, 
    void* data = nullptr);

  void WorkoutActiveHitNum(
    double &active_dodge_rate,
    double &active_attack,
    double &active_cric_rate,
    double &active_cric_times,
    double &active_ammor);


 WK_Bool WorkOutDamage(HITQIQI_HITTYPE& type, int& damage);

  static CClientCore* g_instance;
  HWND m_hwnd = nullptr;
  QIQI_DATA m_qiqi;
  CClientNet m_Net;
  CClientUserData m_User;
  std::thread m_msgLoop;
  std::thread m_UserLoop;
  WK_Bool m_InitFlag= WK_False;
  WK_Bool m_CommandFlag = WK_False;
};

