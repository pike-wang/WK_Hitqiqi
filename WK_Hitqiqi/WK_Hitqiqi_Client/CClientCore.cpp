#include "CClientCore.h"

CClientCore* CClientCore::g_instance = nullptr;

void Thread_Start_MsgLoop()
{
  CClientCore::GetInstance()->MsgLoop();
}

void Thread_Start_UserLoop()
{
  CClientCore::GetInstance()->UserLoop();
}

CClientCore* CClientCore::GetInstance()
{
  if (!g_instance)
    g_instance = new CClientCore();

  return g_instance;
}

WK_Bool CClientCore::Init() {

  srand((int)time(0));

  if (!m_User.Init())
    return WK_False;

  m_Net.Init();

  if (!m_Net.ReConnectServer())
    return WK_False;

  StartMsgLoop();
  HITQIQIMSG_SUBSUBEVENT subsubevent = 
    m_User.GetUserGift() == HITQIQI_PLAYER_GIFT_COMMAND ? 
    HITQIQIMSG_SUBSUBEVENT_COMMAND_ONLINE : (HITQIQIMSG_SUBSUBEVENT)0;

  SendRequest(HITQIQIMSG_EVENT_ASKQIQIDATA, HITQIQIMSG_SUBEVENT_DATAINIT,subsubevent,m_User.GetUser());

  while (!m_InitFlag) { Sleep(50); }

  StartUserLoop();

  return m_InitFlag;
}

WK_Bool CClientCore::StartMsgLoop() {
  m_msgLoop = std::thread(Thread_Start_MsgLoop);
  m_msgLoop.detach();
  return WK_True;
}

WK_Bool CClientCore::MsgLoop() {

  int recv_len = 0;

  while (WK_True)
  {
    HITQIQIMSG_PACKAGE recv_package;
    recv_len = m_Net.RecvServerMsg(&recv_package);

    if (recv_len > 0) {
      HandleServerMsg(&recv_package);
    }
    else {
      break;
    }
  }

  OnConnectError();
  return WK_True;
}

WK_Bool CClientCore::StartUserLoop() {
  m_UserLoop = std::thread(Thread_Start_UserLoop);
  m_UserLoop.detach();
  return WK_True;
}

WK_Bool CClientCore::UserLoop() {

  while (true)
  {
    Sleep(100);
    UserEnergyObtain();
    OnGift_Time();
  }

  return WK_True;
}

WK_Bool CClientCore::HandleServerMsg(HITQIQIMSG_PACKAGE *recv_package) {

//   if (recv_package->version != HITQIQI_VERSION || recv_package->accesscode != HITQIQI_ACCESSCODE)
//     return WK_False;

  switch (recv_package->event) {
  case(HITQIQIMSG_EVENT_UPDATAQIQIDATA): OnUpdataQiqiData(recv_package); break;
  case(HITQIQIMSG_EVENT_NOTIFY): OnShowNotify(recv_package); break;
  case(HITQIQIMSG_EVENT_ASKALLON): OnAllonReply(recv_package); break;
  case(HITQIQIMSG_EVENT_CHARGER): OnEnergyBeenCharger(recv_package); break;
  case(HITQIQIMSG_EVENT_COMMAND): OnCommandStauts(recv_package); break;
  case(HITQIQIMSG_EVENT_BETRAIER): OnBetraier(recv_package); break;
  case(HITQIQIMSG_EVENT_BETRAIER_OBTAIN): OnBetraierObtain(recv_package); break;
  case(HITQIQIMSG_EVENT_UPDATA_BROKEN_TIME): OnUpdataBrokenTime(recv_package); break;
  case(HITQIQIMSG_EVENT_BLADEAMMOR_ENERGY_MIN): OnBladeAmmorEnergyMin(recv_package); break;
  case(HITQIQIMSG_EVENT_SMOKE_EXP_MIN): OnSmokeExpMin(recv_package); break;
  case(HITQIQIMSG_EVENT_WRONG_CLIENT_VERSION): ::MessageBox(NULL, L"请更新客户端！", L"错误", NULL); exit(0); break;
  case(HITQIQIMSG_EVENT_SENTRY_SUCCESS): OnAddExp(m_User.GetUserMaxExp() * HITQIQI_PLAYER_GIFT_VALUE_SENTRY_EXP_REWARD); break;
  case(HITQIQIMSG_EVENT_CHANGEABLE_REWARD): OnChangeAbleReward(recv_package); break;
  default: break;
  }

  return WK_True;
}

WK_Bool CClientCore::SendRequest(
  HITQIQIMSG_EVENT event,
  HITQIQIMSG_SUBEVENT subevent,
  HITQIQIMSG_SUBSUBEVENT subsubevent,
  void* data)
{
  HITQIQIMSG_PACKAGE send_package;
  send_package.event = event;
  send_package.subevent = subevent;
  send_package.subsubevent = subsubevent;
  send_package.event_time = CTime::GetCurrentTime();

  switch (event)
  {
  case(HITQIQIMSG_EVENT_HIT): 
    memcpy(&send_package.hit_info, data,sizeof(HIT_INFO));
    break;
  case(HITQIQIMSG_EVENT_ROT_SUCCESS):
  case(HITQIQIMSG_EVENT_CHARGER):
  case(HITQIQIMSG_EVENT_ASKALLON):
  case(HITQIQIMSG_EVENT_LVLUPGET):
  case(HITQIQIMSG_EVENT_ASKQIQIDATA):
  case(HITQIQIMSG_EVENT_BETRAIER):
    memcpy(&send_package.user_data, data, sizeof(HITQIQI_USER_DATA));
    break;
  case(HITQIQIMSG_EVENT_TRY_BROKEN):
    memcpy(&send_package.broken_time, data, sizeof(CTime));
    break;
  case(HITQIQIMSG_EVENT_CHANGEABLE_REWARD):
    memcpy(&send_package.changeable_reward, data, sizeof(HITQIQI_LULUP_RANDOM));
    break;
  default: break;
  }

  char send_buf[HITQIQ_MAX_BUF] = { 0 };
  memcpy(send_buf, &send_package, HITQIQIMSG_PACKAGE_LEN);

  m_Net.SendServerMsg(send_buf);
  return WK_True;
}

WK_Bool CClientCore::OnConnectError() {
  ::MessageBox(NULL,L"已经与服务器断开连接",L"点杀琦琦",NULL);
  ::exit(0);
  return WK_True;
}

WK_Bool CClientCore::OnUpdataQiqiData(HITQIQIMSG_PACKAGE* recv_package) {
  memcpy(&m_qiqi, &recv_package->qiqidata,sizeof(QIQI_DATA));
  if (recv_package->subevent == HITQIQIMSG_SUBEVENT_DATAINIT)
    m_InitFlag = WK_True;

//   if (m_hwnd != nullptr) {
//     ::SendMessage(m_hwnd, WK_MSG_HITQIQI_UPDATAQIQIDATA,0,0);
//   }

  return WK_True;
}

WK_Bool CClientCore::OnHit(POINT ptMouse) {

  if (GetUserEnergy() < 100)
    return WK_False;

  if (m_qiqi.status == QIQI_STATUS_SUCCESS)
  {
    HITQIQIMSG_PACKAGE package;
    package.event = HITQIQIMSG_EVENT_NOTIFY;
    package.event_time = CTime::GetCurrentTime();
    package.notify.color = QIQI_NOTIFY_BACKGROUND_RED;
    sprintf(package.notify.notify_text, "琦琦已经夺舍成功，所有玩家都失败了");
    OnShowNotify(&package);
    return WK_False;
  }

  if (m_qiqi.status == QIQI_STATUS_DEAD) {
    HITQIQIMSG_PACKAGE package;
    package.event = HITQIQIMSG_EVENT_NOTIFY;
    package.event_time = CTime::GetCurrentTime();
    package.notify.color = QIQI_NOTIFY_BACKGROUND_REAL;
    sprintf(package.notify.notify_text, "琦琦已经死啦，请不要欺负他。");
    OnShowNotify(&package);
    return WK_False;
  }

  ChangeUserEnergy(-100.0);                       

  HIT_INFO hit_info;
  strcpy(hit_info.player_name, m_User.GetUserName());
  WorkOutDamage(hit_info.hit_type, hit_info.hit_damage);

  int sub_event = -1;
  if (m_User.GetUserGift() == HITQIQI_PLAYER_GIFT_WARLOCK)
    sub_event = HITQIQIMSG_SUBEVENT_WARLOCK_HIT;

  if (m_User.GetUserGift() == HITQIQI_PLAYER_GIFT_SENTRY)
    sub_event = HITQIQIMSG_SUBEVENT_SENTRY_HIT;

  if (m_User.GetUserGift() == HITQIQI_PLAYER_GIFT_OBSERVER)
    sub_event = HITQIQIMSG_SUBEVENT_OBSERVER_HIT;

  SendRequest(HITQIQIMSG_EVENT_HIT, (HITQIQIMSG_SUBEVENT)sub_event, (HITQIQIMSG_SUBSUBEVENT)-1,&hit_info);

  HIT_TEXT_INFO text_info;
  text_info.hit_type = hit_info.hit_type;
  text_info.hit_damage = hit_info.hit_damage;
  text_info.is_warlock_hit = m_User.GetUserGift() == HITQIQI_PLAYER_GIFT_WARLOCK;
  text_info.ptMouse = ptMouse;

  if (m_hwnd != NULL )
    ::SendMessage(m_hwnd, WK_MSG_HITQIQI_SHOWHITTEXT, (WPARAM)&text_info, 0);

  OnGift_Hit(hit_info);

  if (hit_info.hit_damage != 0)
    OnAddExp(hit_info.hit_damage);

  return WK_True;
}

void CClientCore::OnAddExp(int exp) {
  int type = -1;
  if (m_CommandFlag) {
    exp = exp * (1 + HITQIQI_PLAYER_GIFT_VALUE_COMMAND_EXP_GET);
  }
  exp = m_User.GetUserGift() == HITQIQI_PLAYER_GIFT_KNOWLEDGE ? exp * HITQIQI_PLAYER_GIFT_VALUE_KNOWLEDGE : exp;
  HITQIQI_LULUP_RANDOM reward;
  if (!m_User.OnAddExp(exp, type, reward))
    return;

  HITQIQIMSG_SUBEVENT subevent;
  switch (type)
  {
  case (int)HITQIQI_RANDOM_TYPE_SIGIN: subevent = HITQIQIMSG_SUBEVENT_LVLUPGET_SIGIN; break;
  case (int)HITQIQI_RANDOM_TYPE_SUBSTANT:  subevent = HITQIQIMSG_SUBEVENT_LVLUPGET_SUBSTANT; break;
  case (int)HITQIQI_RANDOM_TYPE_HUGE: subevent = HITQIQIMSG_SUBEVENT_LVLUPGET_HUGE; break;
  default: break;
  }
  SendRequest(HITQIQIMSG_EVENT_LVLUPGET, subevent, (HITQIQIMSG_SUBSUBEVENT)0, m_User.GetUser());
  SendRequest(HITQIQIMSG_EVENT_CHANGEABLE_REWARD, (HITQIQIMSG_SUBEVENT)0, (HITQIQIMSG_SUBSUBEVENT)0, &reward);
}

WK_Bool CClientCore::WorkOutDamage(HITQIQI_HITTYPE& type, int &damage) {

  if (m_qiqi.status == QIQI_STATUS_SHIELD) {
    type = HITQIQI_HITTYPE_BLOCK;
    damage = 0;
    return WK_True;
  }

  double active_dodge_rate = 0;
  double active_attack =0;
  double active_cric_rate =0;
  double active_cric_times = 0;
  double active_ammor = 0;
  WorkoutActiveHitNum(active_dodge_rate, active_attack, active_cric_rate, active_cric_times, active_ammor);

  WK_Bool Ishit;
  if (active_dodge_rate <= 0)
    Ishit = WK_True;
  else
  Ishit = (rand() % 1000 + 1) > (active_dodge_rate * 1000);

  if (!Ishit) {
    type = HITQIQI_HITTYPE_MISS;
    damage = 0;
    return WK_True;
  }

  Sleep(1);
  int offset = rand() % 10 - 4;
  type = (rand()%1000 +1) <= active_cric_rate * 1000 ? HITQIQI_HITTYPE_CRICITAL : HITQIQI_HITTYPE_NORMAL;
  damage = type == HITQIQI_HITTYPE_NORMAL ? active_attack +offset: active_attack * active_cric_times +offset;

  if (active_ammor >= 0) {
    damage = (double)damage * (1.0 - (0.06*active_ammor) / (1+ 0.06 * active_ammor));
  } else {
    damage = (double)damage * (2.0 - pow(0.94,active_ammor*-1));
  }

  if (m_qiqi.status == QIQI_STATUS_TATTOO)
    damage = damage * (1.0 - HITQIQI_DEFENSE_VALUE_TATTOO_DEFINE);

  if (m_User.GetUserGift() == HITQIQI_PLAYER_GIFT_ALONE)
    AskIfAllon(damage);

  return WK_True;
}

WK_Bool CClientCore::OnShowNotify(HITQIQIMSG_PACKAGE* recv_package) {
  if (m_hwnd != nullptr)
  {
    ::SendMessage(m_hwnd, WK_MSG_HITQIQI_SHOWNOTIFY, (WPARAM)recv_package, 0);
  }
  else
  {
    return WK_False;
  }
  return WK_True;
}

WK_Bool CClientCore::OnChangeAbleReward(HITQIQIMSG_PACKAGE* recv_package) {
  if (m_User.GetUserGift() != HITQIQI_PLAYER_GIFT_CHANGEABLE)
    return WK_False;

  if (recv_package->changeable_reward.type < HITQIQI_RANDOM_TYPE_SIGIN)
    return WK_False;

  m_User.DealWithLVLUP(recv_package->changeable_reward);
  SaveAll();

  HITQIQIMSG_PACKAGE package;
  package.event = HITQIQIMSG_EVENT_NOTIFY;
  package.event_time = CTime::GetCurrentTime();
  package.notify.color = QIQI_NOTIFY_BACKGROUND((int)recv_package->changeable_reward.type - 1);
  sprintf(package.notify.notify_text, "成功复制了一项升级！");
  OnShowNotify(&package);
}

WK_Bool CClientCore::ChangeUserEnergy(double energy) {
  m_User.ChangeUserEnergy(energy);
//   if (m_hwnd != nullptr) {
//     ::SendMessage(m_hwnd, WK_MSG_HITQIQI_UPDATAUSERENERGY, 0, 0);
//   }
  return WK_True;
}

void CClientCore::WorkoutActiveHitNum(double &active_dodge_rate,
                                                                   double &active_attack,
                                                                   double &active_cric_rate,
                                                                   double &active_cric_times,
                                                                   double &active_ammor) {
  /*------active_dodge_rate------------------------------------------------------------------------------------------------------*/

  active_dodge_rate = m_qiqi.dodge_rate - m_User.GetUserHitRate();
  if (m_CommandFlag)
    active_dodge_rate -= HITQIQI_PLAYER_GIFT_VALUE_COMMAND_HIT_RATE;

  /*------active_attack------------------------------------------------------------------------------------------------------*/

  active_attack = m_User.GetUserAttack();

  if (m_User.GetUserGift() == HITQIQI_PLAYER_GIFT_SENTRY)
    active_attack = active_attack + (active_attack  * (m_User.GetUserCricTimes() - 1.0 )) / ( 1.0 / m_User.GetUserCricRate());

  if (m_CommandFlag)
    active_attack = active_attack * (1+ HITQIQI_PLAYER_GIFT_VALUE_COMMAND_ATTACK);

  if (m_User.GetUserGift() == HITQIQI_PLAYER_GIFT_ACCUMULATE)
    active_attack += *m_User.GetUserAccumulateValue();

  /*------active_cric_rate------------------------------------------------------------------------------------------------------*/

  active_cric_rate = m_User.GetUserCricRate();
  if(m_CommandFlag)
    active_cric_rate += HITQIQI_PLAYER_GIFT_VALUE_COMMAND_CRIC_RATE;

  if (m_User.GetUserGift() == HITQIQI_PLAYER_GIFT_SENTRY)
    active_cric_rate = 0;

  /*------active_cric_times------------------------------------------------------------------------------------------------------*/

  active_cric_times = m_User.GetUserCricTimes();

  /*------active_ammor------------------------------------------------------------------------------------------------------*/

  if (m_qiqi.ammor < 0){
    active_ammor = m_qiqi.ammor;
  } else {
    active_ammor = m_qiqi.ammor - m_User.GetUserAmmorVoid() < 0 ? 0 : m_qiqi.ammor - m_User.GetUserAmmorVoid();
  }

  if (active_ammor > 0 && m_User.GetUserGift() == HITQIQI_PLAYER_GIFT_OBSERVER)
    active_ammor = 0;

  /*------other------------------------------------------------------------------------------------------------------*/

  switch (m_User.GetUserGift())
  {
  case HITQIQI_PLAYER_GIFT_WILD:
    active_attack += m_User.GetUserWildValue()->wild_attack;
    active_dodge_rate -= m_User.GetUserWildValue()->wild_hit_rate;
  default: break;
  }

}


void CClientCore::OnGift_Hit(const HIT_INFO &hit_info) {
  switch (m_User.GetUserGift())
  {
  case HITQIQI_PLAYER_GIFT_ROT:
    if (hit_info.hit_type == HITQIQI_HITTYPE_MISS) break;
    if ((rand() % 1000 + 1) <= HITQIQI_PLAYER_GIFT_VALUE_ROT * 1000)
      SendRequest(HITQIQIMSG_EVENT_ROT_SUCCESS,(HITQIQIMSG_SUBEVENT)0,(HITQIQIMSG_SUBSUBEVENT)0,m_User.GetUser());
    break;
  case HITQIQI_PLAYER_GIFT_WILD:
    if (hit_info.hit_type == HITQIQI_HITTYPE_MISS) {
      m_User.GetUserWildValue()->wild_attack = 0;
      m_User.GetUserWildValue()->wild_hit_rate = 0;
      break;
    }
    if (m_User.GetUserWildValue()->wild_attack == 0)
    {
      m_User.GetUserWildValue()->wild_attack = 5;
      m_User.GetUserWildValue()->wild_hit_rate = -0.05;
    } else {
      m_User.GetUserWildValue()->wild_attack = 1.5 * m_User.GetUserWildValue()->wild_attack;
      m_User.GetUserWildValue()->wild_hit_rate = 2 * m_User.GetUserWildValue()->wild_hit_rate;
    }
    break;
  case HITQIQI_PLAYER_GIFT_ACCUMULATE:
    *m_User.GetUserAccumulateValue() = 0;
    break;
  case HITQIQI_PLAYER_GIFT_CHARGER:
    if ((rand() % 1000 + 1) <= HITQIQI_PLAYER_GIFT_VALUE_CHARGER_RATE * 1000)
      SendRequest(HITQIQIMSG_EVENT_CHARGER, (HITQIQIMSG_SUBEVENT)0, (HITQIQIMSG_SUBSUBEVENT)0, m_User.GetUser());
    break;
    //case HITQIQI_PLAYER_GIFT_BROKEN:
    //SendRequest(HITQIQIMSG_EVENT_TRY_BROKEN, (HITQIQIMSG_SUBEVENT)0, (HITQIQIMSG_SUBSUBEVENT)0, m_User.GetUserBrokenTime());
    //break;
  default: break;
  }
}

void CClientCore::OnGift_Time() {
  switch (m_User.GetUserGift())
  {
  case HITQIQI_PLAYER_GIFT_ACCUMULATE:
    *m_User.GetUserAccumulateValue() += HITQIQI_PLAYER_GIFT_VALUE_ACCUMULATE / 10;
    if (*m_User.GetUserAccumulateValue() > HITQIQI_PLAYER_GIFT_VALUE_ACCUMULATE_MAX)
      *m_User.GetUserAccumulateValue() = HITQIQI_PLAYER_GIFT_VALUE_ACCUMULATE_MAX;
    break;
  case HITQIQI_PLAYER_GIFT_BETRAIER:
    *m_User.GetUserBetraierTime() += 0.1;
    if (*m_User.GetUserBetraierTime() >= HITQIQI_PLAYER_GIFT_VALUE_BETRAIER_TIME) {
      *m_User.GetUserBetraierTime() = 0;
      SendRequest(HITQIQIMSG_EVENT_BETRAIER,(HITQIQIMSG_SUBEVENT)0,(HITQIQIMSG_SUBSUBEVENT)0,m_User.GetUser());
    }
    break;
  default: break;
  }
}

WK_Bool CClientCore::AskIfAllon(int &damage) {

  m_User.GetUserAllonValue()->ALLON_UPDATA = WK_False;
  SendRequest(HITQIQIMSG_EVENT_ASKALLON, (HITQIQIMSG_SUBEVENT)0, (HITQIQIMSG_SUBSUBEVENT)0, m_User.GetUser());
  while (!m_User.GetUserAllonValue()->ALLON_UPDATA) {}
  if (m_User.GetUserAllonValue()->ALLON_FLAG)
    damage = damage * 2;

  return WK_True;
}

WK_Bool CClientCore::OnAllonReply(HITQIQIMSG_PACKAGE* recv_package) {
  m_User.GetUserAllonValue()->ALLON_FLAG = recv_package->subevent == HITQIQIMSG_SUBEVENT_FLAG_TRUE;
  m_User.GetUserAllonValue()->ALLON_UPDATA = WK_True;
  return WK_True;
}

WK_Bool CClientCore::UserEnergyObtain() {
  double base_energy_obtian = m_User.GetUserEnergyObtain();

  if (m_User.GetUserGift() == HITQIQI_PLAYER_GIFT_REENERGY)
    base_energy_obtian += base_energy_obtian * (HITQIQI_PLAYER_GIFT_VALUE_REENERGY * (1.0 - m_User.GetUserEnergyPer()));


//   if (m_User.GetUserGift() == HITQIQI_PLAYER_GIFT_BROKEN && m_qiqi.status != QIQI_STATUS_NORMAL)
//     base_energy_obtian = base_energy_obtian * HITQIQI_PLAYER_GIFT_VALUE_BROKEN;

  if (m_CommandFlag)
    base_energy_obtian = base_energy_obtian * (1 + HITQIQI_PLAYER_GIFT_VALUE_COMMAND_ENERGY_OBTAIN);

  m_User.ChangeUserEnergy(base_energy_obtian / 10);
  return WK_True;
}

WK_Bool CClientCore::OnEnergyBeenCharger(HITQIQIMSG_PACKAGE* recv_package) {
  if (m_User.GetUserGift() != HITQIQI_PLAYER_GIFT_CHARGER)
    ChangeUserEnergy(HITQIQI_PLAYER_GIFT_VALUE_CHARGER_AMONT);
  return WK_True;
}

WK_Bool CClientCore::OnCommandStauts(HITQIQIMSG_PACKAGE* recv_package) {
  m_CommandFlag = recv_package->subevent == HITQIQIMSG_SUBEVENT_FLAG_TRUE;
  return WK_True;
}

WK_Bool CClientCore::OnBetraier(HITQIQIMSG_PACKAGE* recv_package) {
  ChangeUserEnergy(HITQIQI_PLAYER_GIFT_VALUE_BETRAIER_AMONT * -1);
  return WK_True;
}

WK_Bool CClientCore::OnBetraierObtain(HITQIQIMSG_PACKAGE* recv_package) {
  ChangeUserEnergy(recv_package->betraier_obtain);
  return WK_True;
}

WK_Bool CClientCore::OnUpdataBrokenTime(HITQIQIMSG_PACKAGE* recv_package) {
//   if (m_User.GetUserGift() == HITQIQI_PLAYER_GIFT_BROKEN)
//     m_User.SetUserBrokenTime(recv_package->event_time);
  return WK_True;
}

WK_Bool CClientCore::OnBladeAmmorEnergyMin(HITQIQIMSG_PACKAGE* recv_package) {
  ChangeUserEnergy(HITQIQI_DEFENSE_VALUE_BLADEAMMOR_ENERGYMIN * -1);
  return WK_True;
}

WK_Bool CClientCore::OnSmokeExpMin(HITQIQIMSG_PACKAGE* recv_package) {
  m_User.OnMinExp(HITQIQI_DEFENSE_VALUE_SMOKE_EXP_MIN);
  return WK_True;
}