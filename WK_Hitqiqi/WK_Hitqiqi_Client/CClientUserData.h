#pragma once

#include <fstream>
#include<vector>
#include "WK_Common_Define.h"
#include "WK_Hitqiqi_def.h"


class CClientUserData
{
public:
  WK_Bool Init();
  HITQIQI_USER_DATA* GetUser() { return &m_data; }
  char* GetUserName() { return m_data.name; }
  double GetUserCricRate() { return m_data.cricital_rate; }
  double GetUserCricTimes() { return m_data.cricital_times; }
  double GetUserAttack() { return m_data.base_attack; }

  double GetUserEnergy() { return m_data.energy_now; }
  double GetUserEnergyObtain() { return m_data.energy_obtain; }
  double GetUserEnergyPer() { return m_data.energy_now/m_data.energy_max; }

  GIFT_WILD_VALUE* GetUserWildValue() { return &m_data.wild_value; }
  GIFT_ALLON_VALUE* GetUserAllonValue() { return &m_data.allon_value; }
  double* GetUserAccumulateValue() { return &m_data.accumulate_value; }
  double* GetUserBetraierTime() { return &m_data.betraier_time; }

  double GetUserHitRate() { return m_data.hit_rate; }
  double GetUserAmmorVoid() { return m_data.ammor_void; }
  double GetUserExpPer() { return m_data.exp_now / m_data.exp_max; }
  double GetUserMaxExp() { return m_data.exp_max; }

  CTime* GetUserBrokenTime() { return &m_data.broken_time; }
  void SetUserBrokenTime(CTime time) { m_data.broken_time = time; WriteIn(); }
  HITQIQI_PLAYER_GIFT GetUserGift() { return m_data.gift; }
  void ChangeUserEnergy(double energy);
  WK_Bool OnAddExp(int exp, int& type, HITQIQI_LULUP_RANDOM& reward);
  WK_Bool OnMinExp(int exp);
  void WriteIn();
  WK_Bool DealWithLVLUP(HITQIQI_LULUP_RANDOM lvlup);

private:

  WK_Bool InitUserName();
  WK_Bool InitUserGift();

  HITQIQI_USER_DATA m_data;
};

