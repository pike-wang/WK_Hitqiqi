#include <UIlib.h>
#include "CClientUserData.h"
#include "CHitqiqiClientWnd.h"
#include "CClientSelectWnd.h"
#include "WK_EncryptCore_Interface.h"

#pragma comment(lib,"WK_EncryptCore.lib")

using namespace DuiLib;
using namespace std;

WK_Bool CClientUserData::Init() {

  fstream _file;
  _file.open(HITQIQI_DATA_FILENAME, ios::in | ios::binary);

  if (!_file) {

    if (!InitUserName() || !InitUserGift())
      return WK_False;

    m_data.exp_max = HITQIQI_USERDEFAULT_EXP_MAX;
    m_data.base_attack = HITQIQI_USERDEFAULT_BASE_ATTACK;
    m_data.cricital_rate = HITQIQI_USERDEFAULT_CRICITAL_RATE;
    m_data.cricital_times = HITQIQI_USERDEFAULT_CRICITAL_TIMES;
    m_data.energy_max = HITQIQI_USERDEFAULT_ENERGY_MAX;
    m_data.energy_now = HITQIQI_USERDEFAULT_ENERGY_MAX;
    m_data.energy_obtain = HITQIQI_USERDEFAULT_ENERGY_OBTAIN;
    //m_data.hit_rate = 2.00;     //WK_Mark: for_test
    WriteIn();
    return WK_True;
  }

  byte* buff = new byte[sizeof(HITQIQI_USER_DATA) + sizeof(int) * 2];
  memset(buff, 0, sizeof(HITQIQI_USER_DATA) + sizeof(int) * 2);
  _file.read((char*)buff, sizeof(HITQIQI_USER_DATA) + sizeof(int) * 2);
  if (_file.gcount() != sizeof(HITQIQI_USER_DATA) + sizeof(int) * 2)
    return WK_False;

  WK_EncryptCore_Decrypt(buff, sizeof(HITQIQI_USER_DATA) + sizeof(int) * 2);

  int version = 0;
  int access_code = 0;

  memcpy(&version, buff ,sizeof(int));
  memcpy(&access_code, buff+sizeof(int), sizeof(int));

  if (version != HITQIQI_VERSION || access_code != HITQIQI_ACCESSCODE)
    return WK_False;

  memcpy(&m_data, buff+sizeof(int)*2, sizeof(HITQIQI_USER_DATA));

  delete[] buff;
  return WK_True;
}

void CClientUserData::WriteIn() {
  byte *buff = new byte[sizeof(HITQIQI_USER_DATA)+sizeof(int)*2];
  memset(buff, 0, sizeof(HITQIQI_USER_DATA) + sizeof(int) * 2);
  int version = HITQIQI_VERSION;
  int access_code = HITQIQI_ACCESSCODE;

  memcpy(buff, &version, sizeof(int));
  memcpy(buff+sizeof(int), &access_code, sizeof(int));
  memcpy(buff + sizeof(int)*2, &m_data, sizeof(HITQIQI_USER_DATA));

  WK_EncryptCore_Encrypt(buff, sizeof(HITQIQI_USER_DATA) + sizeof(int) * 2);

  fstream _file;
  _file.open(HITQIQI_DATA_FILENAME, ios::out | ios::trunc | ios::binary);
  _file.write((char*)buff, sizeof(HITQIQI_USER_DATA) + sizeof(int) * 2);
  _file.close();
  delete [] buff;
}

WK_Bool CClientUserData::InitUserName() {
  CClientSelectWnd SelectFrame;
  SelectFrame.SetMode(CClientSelectWnd::SELECT_NAME);
  SelectFrame.Create(NULL, _T("WK_Hitqiqi_Select"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
  SelectFrame.CenterWindow();
  SelectFrame.ShowModal();

  if (!SelectFrame.IsSuccess())
    return WK_False;

  CString name = SelectFrame.GetInputName().GetData();
  int n = name.GetLength();
  int len = WideCharToMultiByte(CP_ACP, 0, name, name.GetLength(), NULL, 0, NULL, NULL);
  WideCharToMultiByte(CP_ACP, 0, name, name.GetLength(),m_data.name, len, NULL, NULL);
  return WK_True;
}

WK_Bool CClientUserData::InitUserGift() {
  CClientSelectWnd SelectFrame;
  SelectFrame.SetMode(CClientSelectWnd::SELECT_GIFT);
  SelectFrame.Create(NULL, _T("WK_Hitqiqi_Select"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
  SelectFrame.CenterWindow();
  SelectFrame.ShowModal();

  if (!SelectFrame.IsSuccess())
    return WK_False;

  m_data.gift = (HITQIQI_PLAYER_GIFT)SelectFrame.GetChoose();

  return WK_True;
}

WK_Bool CClientUserData::OnAddExp(int exp , int &type, HITQIQI_LULUP_RANDOM& reward) {

  m_data.exp_now += exp;

  if (m_data.exp_now < m_data.exp_max)
    return WK_False;

  srand((int)time(0));

  WK_Bool need_report = WK_False;

  while (m_data.exp_now >= m_data.exp_max) {

    m_data.exp_now -= m_data.exp_max;
    m_data.exp_max *= HITQIQI_USERDEFAULT_EXP_GROWUP;
    m_data.level++;
    WriteIn();

    std::vector<HITQIQI_LULUP_RANDOM> rewardVec;
    rewardVec.clear();

    int reward_add = m_data.gift == HITQIQI_PLAYER_GIFT_WARLOCK ? HITQIQI_PLAYER_GIFT_VALUE_WARLOCK_REWARD_ADD  : 0;
    int reward_cnt = 0;
    while (reward_cnt < HITQIQI_LVLUP_REWARD_CNT + reward_add)
    {
      HITQIQI_LULUP_RANDOM random;
      random.reward = (HITQIQI_LVLUP_REWARD)((rand() % 7) + 1);

      int rate = (rand() % 1000) + 1;

      for (int index = HITQIQI_RANDOM_RETE_CNT; index > 0; index--)
      {
        if (rate <= g_RandomRates[index] * 1000) {
          random.type = (HITQIQI_RANDOM_TYPE)index;
          break;
        }
        rate -= g_RandomRates[index] * 1000;
      }

      WK_Bool has_this_type = WK_False;
      for (auto it : rewardVec) {
        if (it.reward ==random.reward )
        {
          has_this_type = WK_True;
          break;
        }
      }

      if (has_this_type)
        continue;

      rewardVec.push_back(random);
      reward_cnt++;
      Sleep(1);
    }

    CClientSelectWnd SelectFrame;
    SelectFrame.SetMode(CClientSelectWnd::SELECT_LEVELUP);
    SelectFrame.SetVec(&rewardVec);
    SelectFrame.Create(NULL, _T("WK_Hitqiqi_Select"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
    SelectFrame.CenterWindow();
    SelectFrame.ShowModal();

    if (!SelectFrame.IsSuccess())
      return WK_False;

    DealWithLVLUP(rewardVec[SelectFrame.GetChoose()]);
    WriteIn();
    type = (int)rewardVec[SelectFrame.GetChoose()].type;
    need_report = (int)rewardVec[SelectFrame.GetChoose()].type >= 4;

    reward = rewardVec[SelectFrame.GetChoose()];
  }

  return need_report;
}

WK_Bool CClientUserData::DealWithLVLUP(HITQIQI_LULUP_RANDOM lvlup) {
  switch (lvlup.reward)
  {
  case HITQIQI_LVLUP_REWARD_ATTACK: 
    switch (lvlup.type)
    {
    case HITQIQI_RANDOM_TYPE_TINY: m_data.base_attack += 1.5; break;
    case HITQIQI_RANDOM_TYPE_LITTLE: m_data.base_attack += 3.0; break;
    case HITQIQI_RANDOM_TYPE_CONSIDE:  m_data.base_attack += 5.0; break;
    case HITQIQI_RANDOM_TYPE_SIGIN: m_data.base_attack += 7.0; break;
    case HITQIQI_RANDOM_TYPE_SUBSTANT: m_data.base_attack += 9.0; break;
    case HITQIQI_RANDOM_TYPE_HUGE: m_data.base_attack += 15.0; break;
    default: break;
    }
    break;
  case HITQIQI_LVLUP_REWARD_CRIC_RATE: 
    switch (lvlup.type)
    {
    case HITQIQI_RANDOM_TYPE_TINY: m_data.cricital_rate += 0.01; break;
    case HITQIQI_RANDOM_TYPE_LITTLE: m_data.cricital_rate += 0.02; break;
    case HITQIQI_RANDOM_TYPE_CONSIDE: m_data.cricital_rate += 0.04; break;
    case HITQIQI_RANDOM_TYPE_SIGIN: m_data.cricital_rate += 0.05; break;
    case HITQIQI_RANDOM_TYPE_SUBSTANT: m_data.cricital_rate += 0.1; break;
    case HITQIQI_RANDOM_TYPE_HUGE: m_data.cricital_rate += 0.25; break;
    default: break;
    }
    break;
  case HITQIQI_LVLUP_REWARD_CRIC_TIME: 
    switch (lvlup.type)
    {
    case HITQIQI_RANDOM_TYPE_TINY: m_data.cricital_times += 0.10; break;
    case HITQIQI_RANDOM_TYPE_LITTLE: m_data.cricital_times += 0.15; break;
    case HITQIQI_RANDOM_TYPE_CONSIDE: m_data.cricital_times += 0.25; break;
    case HITQIQI_RANDOM_TYPE_SIGIN: m_data.cricital_times += 0.3; break;
    case HITQIQI_RANDOM_TYPE_SUBSTANT: m_data.cricital_times += 0.5; break;
    case HITQIQI_RANDOM_TYPE_HUGE: m_data.cricital_times += 0.7;  break;
    default: break;
    }
    break;
  case HITQIQI_LVLUP_REWARD_ENERGY_MAX: 
    switch (lvlup.type)
    {
    case HITQIQI_RANDOM_TYPE_TINY: m_data.energy_max += 30.0; break;
    case HITQIQI_RANDOM_TYPE_LITTLE: m_data.energy_max += 50.0; break;
    case HITQIQI_RANDOM_TYPE_CONSIDE: m_data.energy_max += 100.0; break;
    case HITQIQI_RANDOM_TYPE_SIGIN: m_data.energy_max += 140.0; break;
    case HITQIQI_RANDOM_TYPE_SUBSTANT: m_data.energy_max += 300.0; break;
    case HITQIQI_RANDOM_TYPE_HUGE: m_data.energy_max += 1000.0; break;
    default: break;
    }
    break;
  case HITQIQI_LVLUP_REWARD_ENERGY_OBTAIN: 
    switch (lvlup.type)
    {
    case HITQIQI_RANDOM_TYPE_TINY: m_data.energy_obtain += 0.2; break;
    case HITQIQI_RANDOM_TYPE_LITTLE: m_data.energy_obtain += 0.3; break;
    case HITQIQI_RANDOM_TYPE_CONSIDE: m_data.energy_obtain += 0.50; break;
    case HITQIQI_RANDOM_TYPE_SIGIN: m_data.energy_obtain += 0.9; break;
    case HITQIQI_RANDOM_TYPE_SUBSTANT: m_data.energy_obtain += 1.2; break;
    case HITQIQI_RANDOM_TYPE_HUGE: m_data.energy_obtain += 2.0; break;
    default: break;
    }
    break;
  case HITQIQI_LVLUP_REWARD_HIT_RATE: 
    switch (lvlup.type)
    {
    case HITQIQI_RANDOM_TYPE_TINY: m_data.hit_rate += 0.005; break;
    case HITQIQI_RANDOM_TYPE_LITTLE: m_data.hit_rate += 0.007; break;
    case HITQIQI_RANDOM_TYPE_CONSIDE: m_data.hit_rate += 0.01; break;
    case HITQIQI_RANDOM_TYPE_SIGIN: m_data.hit_rate += 0.014; break;
    case HITQIQI_RANDOM_TYPE_SUBSTANT: m_data.hit_rate += 0.03; break;
    case HITQIQI_RANDOM_TYPE_HUGE: m_data.hit_rate += 0.10; break;
    default: break;
    }
    break;
  case HITQIQI_LVLUP_REWARD_AMMOR_VOID:
    switch (lvlup.type)
    {
    case HITQIQI_RANDOM_TYPE_TINY: m_data.ammor_void += 0.3; break;
    case HITQIQI_RANDOM_TYPE_LITTLE: m_data.ammor_void += 0.5; break;
    case HITQIQI_RANDOM_TYPE_CONSIDE:  m_data.ammor_void += 0.7; break;
    case HITQIQI_RANDOM_TYPE_SIGIN: m_data.ammor_void += 1.0; break;
    case HITQIQI_RANDOM_TYPE_SUBSTANT: m_data.ammor_void += 1.5; break;
    case HITQIQI_RANDOM_TYPE_HUGE: m_data.ammor_void += 3.0; break;
    default: break;
    }
    break;
  default: 
    break;
  }

  m_data.base_attack += HITQIQI_USERDEFAULT_BASE_ATTACK_GROW;
  return WK_True;
}

void  CClientUserData::ChangeUserEnergy(double energy) {
  m_data.energy_now += energy;
  if (m_data.energy_now < 0) m_data.energy_now = 0;
  if (m_data.energy_now > m_data.energy_max) m_data.energy_now = m_data.energy_max;
}

WK_Bool CClientUserData::OnMinExp(int exp) {
  if (m_data.exp_now > exp)
  {
    m_data.exp_now -= exp;
  }
  else {
    m_data.exp_now = 0;
  }
  return WK_True;
}