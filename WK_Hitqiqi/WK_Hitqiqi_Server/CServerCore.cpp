#include "CServerCore.h"

CServerCore* CServerCore::g_instance = nullptr;

void Thread_Start_ServerLoop()
{
  CServerCore::GetInstance()->ServerLoop();
}

CServerCore* CServerCore::GetInstance()
{
  if (!g_instance)
    g_instance = new CServerCore();

  return g_instance;
}

WK_Bool CServerCore::Init() {

  if (!ReadQiqiData())
    return WK_False;

  if(!m_Net.Init())
    return WK_False;

  m_ServerStartTime = CTime::GetCurrentTime();
  m_vHitRecord.clear();

  WKLOG("ServerLog", "CServerCore::Init()");
  return WK_True;
}

WK_Bool CServerCore::StratServer() {

  StartServerLoop();

  m_Net.BeginTCPListen();
  return WK_True;
}

WK_Bool CServerCore::ReadQiqiData() {

  m_qiqi.status = QIQI_STATUS_NORMAL;
  m_qiqi.hp_max = 2000000;
  m_qiqi.hp_now = 2000000;
  m_qiqi.energy =  50.0;
  m_qiqi.energy_max = 100.0;
  m_qiqi.ammor = 7.5;
  m_qiqi.dodge_rate = 0.17;
  m_qiqi.end_sec = 604800;

  return WK_True;
}

WK_Bool CServerCore::StartServerLoop() {
  m_ServerLoop = std::thread(Thread_Start_ServerLoop);
  m_ServerLoop.detach();
  return WK_True;
}

WK_Bool CServerCore::ServerLoop() {

  int loop_sec = 1;

  while (WK_True)
  {
    Sleep(1000);
    if (OnQiqiTimeMin())
    {
      break;
    }
    OnDefense_Time();
    OnSyncQiqi();
    if (loop_sec % 60 == 0) RefershHitRecord();

    loop_sec++;
  }
  return WK_True;
}

WK_Bool CServerCore::ReportQiqiStatus() {
  using namespace std;
  cout << "琦琦当前状态：" << m_qiqi.status<<endl;
  cout << "琦琦当前血量：" << m_qiqi.hp_now <<" / " << m_qiqi.hp_max << endl;
  cout << "琦琦当前能量：" << m_qiqi.energy << endl;
  cout << "琦琦当前护甲：" << m_qiqi.ammor << endl;
  cout << "琦琦当前闪避：" << m_qiqi.dodge_rate << endl;
  cout << "琦琦剩余存活：" << m_qiqi.end_sec << endl<<endl;
  return WK_True;
}

WK_Bool CServerCore::HandleClientMsg(HITQIQIMSG_PACKAGE* recv_package, void* socketin) {

  if (recv_package->version != HITQIQI_VERSION || recv_package->accesscode != HITQIQI_ACCESSCODE) {
    OnPostQuitMsg(recv_package, socketin);
    return WK_False;
  }

  recv_package->event_time = CTime::GetCurrentTime();

  switch (recv_package->event)
  {
  case(HITQIQIMSG_EVENT_ASKQIQIDATA): OnAskQiqiData(recv_package, socketin); break;
  case(HITQIQIMSG_EVENT_HIT): OnHit(recv_package, socketin); break;
  case(HITQIQIMSG_EVENT_LVLUPGET): OnLvlupGet(recv_package); break;
  case(HITQIQIMSG_EVENT_ROT_SUCCESS): OnRotAmmor(recv_package); break;
  case(HITQIQIMSG_EVENT_ASKALLON): OnAskAllon(recv_package, socketin); break;
  case(HITQIQIMSG_EVENT_CHARGER): OnEnergyCharger(recv_package); break;
  case(HITQIQIMSG_EVENT_BETRAIER): OnBetraier(recv_package, socketin); break;
  case(HITQIQIMSG_EVENT_TRY_BROKEN): OnTryBroken(recv_package, socketin); break;
  case(HITQIQIMSG_EVENT_CHANGEABLE_REWARD): OnChangeAbleReward(recv_package, socketin); break;
  default: break;
  }

  return WK_True;
}

WK_Bool CServerCore::OnPostQuitMsg(HITQIQIMSG_PACKAGE* recv_package, void* socketin) {
  HITQIQIMSG_PACKAGE send_package;
  send_package.event = HITQIQIMSG_EVENT_WRONG_CLIENT_VERSION;
  send_package.subevent = recv_package->subevent;
  send_package.subsubevent = recv_package->subsubevent;
  send_package.event_time = CTime::GetCurrentTime();
  memcpy(&send_package.qiqidata, &m_qiqi, sizeof(QIQI_DATA));
  m_Net.SendClientMsg(&send_package, socketin);
  return WK_True;
}

WK_Bool CServerCore::OnAskQiqiData(HITQIQIMSG_PACKAGE* recv_package, void* socketin) {
  HITQIQIMSG_PACKAGE send_package;
  send_package.event = HITQIQIMSG_EVENT_UPDATAQIQIDATA;
  send_package.subevent = recv_package->subevent;
  send_package.subsubevent = recv_package->subsubevent;
  send_package.event_time = CTime::GetCurrentTime();
  memcpy(&send_package.qiqidata, &m_qiqi, sizeof(QIQI_DATA));
  m_Net.SendClientMsg(&send_package, socketin);

  if (command_cnt > 0)
    {
      HITQIQIMSG_PACKAGE send_package_command_init;
      send_package_command_init.event = HITQIQIMSG_EVENT_COMMAND;
      send_package_command_init.subevent = HITQIQIMSG_SUBEVENT_FLAG_TRUE;
      send_package_command_init.event_time = CTime::GetCurrentTime();
      m_Net.SendClientMsg(&send_package_command_init,socketin);
    }

  Sleep(200);
  SendNotify(recv_package, socketin);

  if (recv_package->subsubevent == HITQIQIMSG_SUBSUBEVENT_COMMAND_ONLINE) {
    command_cnt++;
    m_Net.AddCommandSocket(socketin);
    HITQIQIMSG_PACKAGE send_package_command;
    send_package_command.event = HITQIQIMSG_EVENT_COMMAND;
    send_package_command.subevent = HITQIQIMSG_SUBEVENT_FLAG_TRUE;
    send_package_command.event_time = CTime::GetCurrentTime();
    m_Net.SendClientMsg(&send_package_command);
    SendNotify(recv_package);
  }

  return WK_True;
}

WK_Bool CServerCore::OnChangeAbleReward(HITQIQIMSG_PACKAGE* recv_package, void* socketin) {
  HITQIQIMSG_PACKAGE send_package;
  send_package.event = HITQIQIMSG_EVENT_CHANGEABLE_REWARD;
  send_package.event_time = CTime::GetCurrentTime();
  memcpy(&send_package.changeable_reward, &recv_package->changeable_reward, sizeof(HITQIQI_LULUP_RANDOM));
  m_Net.SendClientMsg(&send_package, socketin, WK_True);
  return WK_True;
}

WK_Bool CServerCore::OnObserverHit(HITQIQIMSG_PACKAGE* recv_package, void* socketin) {
  HITQIQIMSG_PACKAGE send_package;
  send_package.event = HITQIQIMSG_EVENT_NOTIFY;
  send_package.event_time = CTime::GetCurrentTime();
  send_package.notify.color = QIQI_NOTIFY_BACKGROUND_NORMAL;
  sprintf(send_package.notify.notify_text, "琦琦血量:%d 怒气:%.1f 护甲:%.1f 闪避:%.1f", m_qiqi.hp_now, m_qiqi.energy, m_qiqi.ammor, m_qiqi.dodge_rate);
  m_Net.SendClientMsg(&send_package);
  return WK_True;
}

WK_Bool CServerCore::OnSentryHit(HITQIQIMSG_PACKAGE* recv_package, void* socketin) {
  if (recv_package->hit_info.hit_type == HITQIQI_HITTYPE_MISS || recv_package->hit_info.hit_type == HITQIQI_HITTYPE_BLOCK || recv_package->hit_info.hit_type == HITQIQI_HITTYPE_CRICITAL)
    return WK_True;

  hit_record_lock.lock();
  if (m_vHitRecord.empty()) {
    hit_record_lock.unlock();
    return WK_True;
  }

  HITQIQI_SERVER_HIT_RECORD last_record = m_vHitRecord.back();
  if (last_record.hit_info.hit_type == HITQIQI_HITTYPE_CRICITAL) {
    HITQIQIMSG_PACKAGE send_package;
    send_package.event = HITQIQIMSG_EVENT_SENTRY_SUCCESS;
    send_package.event_time = CTime::GetCurrentTime();
    m_Net.SendClientMsg(&send_package, socketin);
  }

  hit_record_lock.unlock();
  return WK_True;
}

WK_Bool CServerCore::OnHit(HITQIQIMSG_PACKAGE* recv_package, void* socketin) {
  SendNotify(recv_package);

  if (recv_package->subevent == HITQIQIMSG_SUBEVENT_WARLOCK_HIT && recv_package->hit_info.hit_type == HITQIQI_HITTYPE_NORMAL) {
    OnQiqiHPChange(recv_package->hit_info.hit_damage);
    return WK_True;
  }

  if (recv_package->subevent == HITQIQIMSG_SUBEVENT_SENTRY_HIT)
    OnSentryHit(recv_package, socketin);

  if (recv_package->subevent == HITQIQIMSG_SUBEVENT_OBSERVER_HIT)
    OnObserverHit(recv_package, socketin);

  OnQiqiHPChange(recv_package->hit_info.hit_damage * -1);
  RecordHit(recv_package);
  if (m_qiqi.status != QIQI_STATUS_DEAD)
    OnDefense_Hit(recv_package->hit_info);

  CString hit_log_str;
  hit_log_str.Format(_T("%S: %d"), recv_package->hit_info.player_name, recv_package->hit_info.hit_damage);

  char *m_str = NULL;
  int m_len = 0;
  m_len = WideCharToMultiByte(CP_ACP, 0, hit_log_str, hit_log_str.GetLength(), NULL, 0, NULL, NULL);
  if (m_len)
  {
    m_str = new char[m_len + 1];
    WideCharToMultiByte(CP_ACP, 0, hit_log_str, hit_log_str.GetLength(), m_str, m_len, NULL, NULL);
    m_str[m_len] = 0;
  }

  WKLOG("HitLog", m_str);
  delete[] m_str;
  return WK_True;
}

WK_Bool CServerCore::OnQiqiDead() {
  m_qiqi.hp_now = 0;
  m_qiqi.status = QIQI_STATUS_DEAD;
  HITQIQIMSG_PACKAGE send_package;
  send_package.event = HITQIQIMSG_EVENT_NOTIFY;
  send_package.event_time = CTime::GetCurrentTime();
  send_package.notify.color = QIQI_NOTIFY_BACKGROUND_GRODEN;
  sprintf(send_package.notify.notify_text, "恭喜全体玩家点杀了琦琦！");
  m_Net.SendClientMsg(&send_package);
  return WK_True;
}

WK_Bool CServerCore::OnQiqiHPChange(int value) {

  if (value == 0)
    return WK_True;

  m_qiqi.hp_now += value;

  if (m_qiqi.hp_now > m_qiqi.hp_max)
    m_qiqi.hp_now = m_qiqi.hp_max;

  if (m_qiqi.hp_now <= 0)
    OnQiqiDead();

  OnQiqiEnergyChange(value * -1);
  ReportQiqiStatus();

  HITQIQIMSG_PACKAGE send_package;
  send_package.event = HITQIQIMSG_EVENT_UPDATAQIQIDATA;
  send_package.event_time = CTime::GetCurrentTime();
  memcpy(&send_package.qiqidata, &m_qiqi, sizeof(QIQI_DATA));
  m_Net.SendClientMsg(&send_package);

  return WK_True;
}

WK_Bool CServerCore::SendNotify(HITQIQIMSG_PACKAGE* recv_package, void* socket) {
  HITQIQIMSG_PACKAGE send_package;
  send_package.event = HITQIQIMSG_EVENT_NOTIFY;
  send_package.event_time = CTime::GetCurrentTime();

  //std::string notify(50,0);
  switch (recv_package->event)
  {
  case(HITQIQIMSG_EVENT_HIT):
    if (recv_package->hit_info.hit_type == HITQIQI_HITTYPE_NORMAL) {
      if (recv_package->subevent == HITQIQIMSG_SUBEVENT_WARLOCK_HIT)
      {
        send_package.notify.color = QIQI_NOTIFY_BACKGROUND_NORMAL;
        sprintf(send_package.notify.notify_text, "玩家%s治疗了琦琦，恢复%d点生命。", recv_package->hit_info.player_name, recv_package->hit_info.hit_damage);
      }
      else
      {
        send_package.notify.color = QIQI_NOTIFY_BACKGROUND_NORMAL;
        sprintf(send_package.notify.notify_text, "玩家%s砍了琦琦一刀，造成%d点伤害。", recv_package->hit_info.player_name, recv_package->hit_info.hit_damage);
      }
    }
    if (recv_package->hit_info.hit_type == HITQIQI_HITTYPE_CRICITAL) {
      send_package.notify.color = QIQI_NOTIFY_BACKGROUND_REAL;
      sprintf(send_package.notify.notify_text, "玩家%s砍了琦琦一刀，造成%d点暴击伤害！", recv_package->hit_info.player_name, recv_package->hit_info.hit_damage);
    }
    if (recv_package->hit_info.hit_type == HITQIQI_HITTYPE_MISS) {
      send_package.notify.color = QIQI_NOTIFY_BACKGROUND_NORMAL;
      sprintf(send_package.notify.notify_text, "可惜啊！玩家%s没有砍中琦琦。", recv_package->hit_info.player_name);
    }
    if (recv_package->hit_info.hit_type == HITQIQI_HITTYPE_BLOCK) {
      send_package.notify.color = QIQI_NOTIFY_BACKGROUND_NORMAL;
      sprintf(send_package.notify.notify_text, "玩家%s的攻击被琦琦护盾抵挡了！", recv_package->hit_info.player_name);
    }
    break;
  case(HITQIQIMSG_EVENT_ASKQIQIDATA):
    if (recv_package->subsubevent == HITQIQIMSG_SUBSUBEVENT_COMMAND_ONLINE && socket == nullptr)
    {
      send_package.notify.color = QIQI_NOTIFY_BACKGROUND_EXCELLENT;
      sprintf(send_package.notify.notify_text, "指挥官%s上线了，为所有玩家提供了全属性。", recv_package->user_data.name);
    } else {
      send_package.notify.color = QIQI_NOTIFY_BACKGROUND_NORMAL;
      sprintf(send_package.notify.notify_text, "欢迎游玩点杀琦琦，目前共有%d名玩家在线。", m_Net.GetConnectCnt());
    }
    break;
  case(HITQIQIMSG_EVENT_ROT_SUCCESS):
    send_package.notify.color = QIQI_NOTIFY_BACKGROUND_ROT;
    sprintf(send_package.notify.notify_text, "玩家%s腐蚀了琦琦的护甲！", recv_package->hit_info.player_name);
    break;
  case(HITQIQIMSG_EVENT_LVLUPGET):
    switch (recv_package->subevent)
    {
    case HITQIQIMSG_SUBEVENT_LVLUPGET_SIGIN: send_package.notify.color = QIQI_NOTIFY_BACKGROUND_EXCELLENT; break;
    case HITQIQIMSG_SUBEVENT_LVLUPGET_SUBSTANT:  send_package.notify.color = QIQI_NOTIFY_BACKGROUND_EPIC; break;
    case HITQIQIMSG_SUBEVENT_LVLUPGET_HUGE: send_package.notify.color = QIQI_NOTIFY_BACKGROUND_GRODEN; break;
    default: send_package.notify.color = QIQI_NOTIFY_BACKGROUND_NORMAL; break;
    }
    sprintf(send_package.notify.notify_text, "恭喜玩家%s获取了一项%s升级！", recv_package->user_data.name, g_RandomTypeDes[recv_package->subevent+ (HITQIQI_RANDOM_TYPE_SIGIN - HITQIQIMSG_SUBEVENT_LVLUPGET_SIGIN)]);
    break;
  case(HITQIQIMSG_EVENT_CHARGER):
    send_package.notify.color = QIQI_NOTIFY_BACKGROUND_REAL;
    sprintf(send_package.notify.notify_text, "玩家%s为所有玩家恢复了精力！", recv_package->hit_info.player_name);
    break;
  case(HITQIQIMSG_EVENT_COMMAND):
    if (recv_package->subevent = HITQIQIMSG_SUBEVENT_FLAG_FALSE)
    {
      send_package.notify.color = QIQI_NOTIFY_BACKGROUND_EXCELLENT;
      sprintf(send_package.notify.notify_text, "所有指挥官都下线了，玩家失去了增益效果。");
    } else {
      return WK_False;
    }
    break;
  case(HITQIQIMSG_EVENT_BETRAIER):
    send_package.notify.color = QIQI_NOTIFY_BACKGROUND_RED;
    sprintf(send_package.notify.notify_text, "有背叛者吸取了大家的精力！");
    break;
  case(HITQIQIMSG_EVENT_TRY_BROKEN):
    send_package.notify.color = QIQI_NOTIFY_BACKGROUND_EPIC;
    sprintf(send_package.notify.notify_text, "破坏者驱散了琦琦的状态！");
    break;
  default:
    break;
  }

  m_Net.SendClientMsg(&send_package, socket);
  return WK_True;
}

WK_Bool CServerCore::OnLvlupGet(HITQIQIMSG_PACKAGE* recv_package) {
  SendNotify(recv_package);
  return WK_True;
}

WK_Bool CServerCore::OnRotAmmor(HITQIQIMSG_PACKAGE* recv_package) {

  if (m_qiqi.ammor > 0)
    m_qiqi.ammor = m_qiqi.ammor * (1- HITQIQI_PLAYER_GIFT_VALUE_ROT_AMMOR) - HITQIQI_PLAYER_GIFT_VALUE_ROT_AMMOR;
  else
    m_qiqi.ammor -= HITQIQI_PLAYER_GIFT_VALUE_ROT_AMMOR;

  HITQIQIMSG_PACKAGE send_package;
  send_package.event = HITQIQIMSG_EVENT_UPDATAQIQIDATA;
  send_package.event_time = CTime::GetCurrentTime();
  memcpy(&send_package.qiqidata, &m_qiqi, sizeof(QIQI_DATA));
  m_Net.SendClientMsg(&send_package);

  SendNotify(recv_package);
  return WK_True;
}

WK_Bool CServerCore::RecordHit(HITQIQIMSG_PACKAGE* recv_package) {

  HITQIQI_SERVER_HIT_RECORD record;
  record.time = recv_package->event_time;
  memcpy(&record.hit_info, &recv_package->hit_info, sizeof(HIT_INFO));
  hit_record_lock.lock();
  m_vHitRecord.push_back(record);
  hit_record_lock.unlock();
  return WK_True;
}

WK_Bool CServerCore::OnQiqiTimeMin() {
  if (m_qiqi.end_sec > 0 && m_qiqi.status != QIQI_STATUS_SUCCESS) m_qiqi.end_sec--;

  if (m_qiqi.end_sec <= 0)
  {
    m_qiqi.status = QIQI_STATUS_SUCCESS;
    OnSyncQiqi();

    HITQIQIMSG_PACKAGE send_package;
    send_package.event = HITQIQIMSG_EVENT_NOTIFY;
    send_package.event_time = CTime::GetCurrentTime();
    send_package.notify.color = QIQI_NOTIFY_BACKGROUND_RED;
    sprintf(send_package.notify.notify_text, "琦琦夺舍成功了，所有玩家都失败了");
    m_Net.SendClientMsg(&send_package);

    return WK_True;
  }

  return WK_False;
}

WK_Bool CServerCore::RefershHitRecord() {

  CTime time_now = CTime::GetCurrentTime();
  hit_record_lock.lock();
  if (m_vHitRecord.empty()) { hit_record_lock.unlock(); return WK_True; }

  for (std::vector<HITQIQI_SERVER_HIT_RECORD>::const_iterator iter = m_vHitRecord.cbegin(); iter != m_vHitRecord.cend(); )
  {
    if (time_now - (iter->time) > 1800) {
      iter = m_vHitRecord.erase(iter);
    }
    else {
      hit_record_lock.unlock();
      return WK_True;
    }
  }

  hit_record_lock.unlock();
  return WK_True;
}

WK_Bool CServerCore::OnAskAllon(HITQIQIMSG_PACKAGE* recv_package, void* socketin) {
  HITQIQIMSG_PACKAGE send_package;
  send_package.event = HITQIQIMSG_EVENT_ASKALLON;
  send_package.subevent = HITQIQIMSG_SUBEVENT_FLAG_FALSE;
  if(TestAllon(recv_package->user_data.name))
    send_package.subevent = HITQIQIMSG_SUBEVENT_FLAG_TRUE;
  send_package.event_time = CTime::GetCurrentTime();
  m_Net.SendClientMsg(&send_package, socketin);
  return WK_True;
}

WK_Bool CServerCore::TestAllon(char* name) {
  CTime time_now = CTime::GetCurrentTime();
  hit_record_lock.lock();
  if (m_vHitRecord.empty()) { hit_record_lock.unlock(); return WK_True; }

  for (std::vector<HITQIQI_SERVER_HIT_RECORD>::const_iterator iter = m_vHitRecord.cbegin(); iter != m_vHitRecord.cend(); iter++)
  {
    if (time_now - iter->time > 600)
      continue;

    if (strcmp(name,iter->hit_info.player_name)!=0) {
      hit_record_lock.unlock();
      return WK_False;
    } else {
        hit_record_lock.unlock();
        return WK_True;
    }
  }

  hit_record_lock.unlock();
  return WK_True;
}

WK_Bool CServerCore::OnEnergyCharger(HITQIQIMSG_PACKAGE* recv_package) {

  HITQIQIMSG_PACKAGE send_package;
  send_package.event = HITQIQIMSG_EVENT_CHARGER;
  send_package.event_time = CTime::GetCurrentTime();
  m_Net.SendClientMsg(&send_package);

  SendNotify(recv_package);
  return WK_True;
}

WK_Bool CServerCore::OnCommandOffline() {
  command_cnt--;
  if (command_cnt == 0)
  {
    HITQIQIMSG_PACKAGE send_package_command;
    send_package_command.event = HITQIQIMSG_EVENT_COMMAND;
    send_package_command.subevent = HITQIQIMSG_SUBEVENT_FLAG_FALSE;
    send_package_command.event_time = CTime::GetCurrentTime();
    m_Net.SendClientMsg(&send_package_command);
    SendNotify(&send_package_command);
  }
  return WK_True;
}

WK_Bool CServerCore::OnBetraier(HITQIQIMSG_PACKAGE* recv_package, void* socketin) {
  std::cout << "侦测到背叛者： "<<recv_package->user_data.name <<std::endl;
  int user_cnt = m_Net.GetConnectCnt();
  HITQIQIMSG_PACKAGE send_package;
  send_package.event = HITQIQIMSG_EVENT_BETRAIER;
  send_package.event_time = CTime::GetCurrentTime();
  send_package.betraier_obtain = user_cnt * HITQIQI_PLAYER_GIFT_VALUE_BETRAIER_AMONT;
  m_Net.SendClientMsg(&send_package);
  send_package.event = HITQIQIMSG_EVENT_BETRAIER_OBTAIN;
  m_Net.SendClientMsg(&send_package,socketin);
  send_package.event = HITQIQIMSG_EVENT_BETRAIER;
  SendNotify(&send_package);
  return WK_True;
}

WK_Bool CServerCore::OnQiqiEnergyChange(int value) {
  if (m_qiqi.status != QIQI_STATUS_NORMAL || value <= 0)
    return WK_True;

  double energyadd = (double)value / 200;
  //double energyadd = (double)value;  //WK_Mark: for_test
  m_qiqi.energy += energyadd;

  if (m_qiqi.energy >= m_qiqi.energy_max) {
    OnIntoDefense();
  }

  return WK_True;
}

WK_Bool CServerCore::OnIntoDefense() {
  srand((unsigned int)time(NULL));
  int st_int = rand() % 9;
  QIQI_STATUS  status = (QIQI_STATUS)(st_int + QIQI_STATUS_TEA);
  //QIQI_STATUS  status = QIQI_STATUS_SMOKE;  //WK_Mark: for_test
  InitDefense(status);
  m_qiqi.status = status;
  m_qiqi.energy = 0;

  OnSyncQiqi();

  HITQIQIMSG_PACKAGE send_package;
  send_package.event = HITQIQIMSG_EVENT_NOTIFY;
  send_package.event_time = CTime::GetCurrentTime();
  send_package.notify.color = QIQI_NOTIFY_BACKGROUND_RED;
  sprintf(send_package.notify.notify_text, "琦琦十分生气，进入了%s状态！", g_StatusDes[(int)status]);
  m_Net.SendClientMsg(&send_package);

  return WK_True;
}

WK_Bool CServerCore::InitDefense(QIQI_STATUS  status) {

  memset(&m_qiqi_def, 0, sizeof(HITQIQI_DEFENSE_VALUE));
  switch (status)
  {
  case QIQI_STATUS_TEA:
    m_qiqi_def.tea_value.time_rest = HITQIQI_DEFENSE_VALUE_TEA_TOTAL_TIME;
    m_qiqi_def.tea_value.time_delay = HITQIQI_DEFENSE_VALUE_TEA_DELAY_TIME;
  break;
  case QIQI_STATUS_RUN:
    m_qiqi_def.run_value.time_rest= HITQIQI_DEFENSE_VALUE_RUN_TOTAL_TIME;
    break;
  case QIQI_STATUS_FITNESS:
    m_qiqi_def.fitness_value.time_rest = HITQIQI_DEFENSE_VALUE_FITNESS_TOTAL_TIME;
    break;
  case QIQI_STATUS_SHADOW:
    m_qiqi_def.shadow_value.nomiss_hit = -1;
    m_qiqi.dodge_rate += HITQIQI_DEFENSE_VALUE_SHADOW_DODGE;
    break;
  case QIQI_STATUS_TATTOO:
    m_qiqi_def.tattoo_value.circ_cnt = 0;
    break;
  case QIQI_STATUS_FISH:
    m_qiqi_def.fish_value.start_time = CTime::GetCurrentTime();
    break;
  case QIQI_STATUS_BLADEAMMOR:
    m_qiqi_def.bladeammor_value.start_time = CTime::GetCurrentTime();
    m_qiqi_def.bladeammor_value.time_delay = HITQIQI_DEFENSE_VALUE_BLADEAMMOR_TIME;
    break;
  case QIQI_STATUS_SMOKE:
    m_qiqi_def.smoke_value.time_rest = HITQIQI_DEFENSE_VALUE_SMOKE_TIME_CNT;
    m_qiqi_def.smoke_value.time_delay = HITQIQI_DEFENSE_VALUE_SMOKE_TIME_MIN;
    break;
  case QIQI_STATUS_SHIELD:
  default: break;
  }
  return WK_True;
}

WK_Bool CServerCore::OnDefense_Time() {
  switch (m_qiqi.status)
  {
  case QIQI_STATUS_TEA:
    m_qiqi_def.tea_value.time_rest--;
    if (m_qiqi_def.tea_value.time_rest <= 0)
      OnQuitDefense(QIQI_STATUS_TEA);

    m_qiqi_def.tea_value.time_delay--;
    if (m_qiqi_def.tea_value.time_delay <= 0) {
      m_qiqi_def.tea_value.time_delay = HITQIQI_DEFENSE_VALUE_TEA_DELAY_TIME;

      m_qiqi.hp_now = m_qiqi.hp_now + (double)m_qiqi.hp_max * HITQIQI_DEFENSE_VALUE_TEA_HP_OBTAIN > m_qiqi.hp_max ? m_qiqi.hp_max : m_qiqi.hp_now + (double)m_qiqi.hp_max * HITQIQI_DEFENSE_VALUE_TEA_HP_OBTAIN;
    }
    break;
  case QIQI_STATUS_RUN:
    m_qiqi_def.run_value.time_rest--;
    if (m_qiqi_def.run_value.time_rest <= 0) {
      m_qiqi.hp_now = m_qiqi.hp_now + (double)m_qiqi.hp_max * HITQIQI_DEFENSE_VALUE_RUN_TOTAL_TIME > m_qiqi.hp_max ? m_qiqi.hp_max : m_qiqi.hp_now + (double)m_qiqi.hp_max * HITQIQI_DEFENSE_VALUE_RUN_TOTAL_TIME;
      OnQuitDefense(QIQI_STATUS_RUN);
    }
    break;
  case QIQI_STATUS_FITNESS:
    m_qiqi_def.fitness_value.time_rest--;
    if (m_qiqi_def.fitness_value.time_rest <= 0) {
      m_qiqi.ammor += HITQIQI_DEFENSE_VALUE_FITNESS_AMMOR_OBTAIN;
      OnQuitDefense(QIQI_STATUS_FITNESS);
    }
    break;
  case QIQI_STATUS_FISH:
    m_qiqi.end_sec -= HITQIQI_DEFENSE_VALUE_FISH_TIMEMIN;
    break;
  case QIQI_STATUS_BLADEAMMOR:
    m_qiqi_def.bladeammor_value.time_delay--;
    if (m_qiqi_def.bladeammor_value.time_delay<=0)
    {
      m_qiqi_def.bladeammor_value.time_delay = HITQIQI_DEFENSE_VALUE_BLADEAMMOR_TIME;

      HITQIQIMSG_PACKAGE send_package;
      send_package.event = HITQIQIMSG_EVENT_BLADEAMMOR_ENERGY_MIN;
      send_package.event_time = CTime::GetCurrentTime();
      m_Net.SendClientMsg(&send_package);

      HITQIQIMSG_PACKAGE send_package_notify;
      send_package_notify.event = HITQIQIMSG_EVENT_NOTIFY;
      send_package_notify.event_time = CTime::GetCurrentTime();
      send_package_notify.notify.color = QIQI_NOTIFY_BACKGROUND_RED;
      sprintf(send_package_notify.notify.notify_text, "琦琦削减了大家的精力！");

      m_Net.SendClientMsg(&send_package_notify);
    }
    break;
  case QIQI_STATUS_SMOKE:
    if (m_Net.GetConnectCnt()< HITQIQI_DEFENSE_VALUE_SMOKE_PLAYER_CNT)
      m_qiqi_def.smoke_value.time_rest = HITQIQI_DEFENSE_VALUE_SMOKE_TIME_CNT;
    
    m_qiqi_def.smoke_value.time_rest--;
    if (m_qiqi_def.smoke_value.time_rest <= 0) {
      OnQuitDefense(QIQI_STATUS_SMOKE);
      break;
    }

    m_qiqi_def.smoke_value.time_delay--;
    if (m_qiqi_def.smoke_value.time_delay <= 0)
    {
      m_qiqi_def.smoke_value.time_delay = HITQIQI_DEFENSE_VALUE_SMOKE_TIME_MIN;

      HITQIQIMSG_PACKAGE send_package;
      send_package.event = HITQIQIMSG_EVENT_SMOKE_EXP_MIN;
      send_package.event_time = CTime::GetCurrentTime();
      m_Net.SendClientMsg(&send_package);

      HITQIQIMSG_PACKAGE send_package_notify;
      send_package_notify.event = HITQIQIMSG_EVENT_NOTIFY;
      send_package_notify.event_time = CTime::GetCurrentTime();
      send_package_notify.notify.color = QIQI_NOTIFY_BACKGROUND_RED;
      sprintf(send_package_notify.notify.notify_text, "琦琦扣除了大家的经验值！");

      m_Net.SendClientMsg(&send_package_notify);
    }
    break;
  default: break;
  }
  return WK_True;
}

WK_Bool CServerCore::OnDefense_Hit(HIT_INFO& hit_info) {
  switch (m_qiqi.status)
  {
  case QIQI_STATUS_ERROR:
    break;
  case QIQI_STATUS_NORMAL:
    break;
  case QIQI_STATUS_TEA:
    m_qiqi_def.tea_value.time_delay = HITQIQI_DEFENSE_VALUE_TEA_DELAY_TIME;
    break;
  case QIQI_STATUS_RUN:
    m_qiqi_def.run_value.damage_sum += hit_info.hit_damage;
    if (m_qiqi_def.run_value.damage_sum>= HITQIQI_DEFENSE_VALUE_RUN_TOTAK_DAMAGE)
      OnQuitDefense(QIQI_STATUS_RUN);
    break;
  case QIQI_STATUS_FITNESS:
    if(hit_info.hit_damage >= HITQIQI_DEFENSE_VALUE_FITNESS_BROKEN_LIMIT)
      OnQuitDefense(QIQI_STATUS_FITNESS);
    break;
  case QIQI_STATUS_SHIELD:
    if(CheckShieldBroken())
      OnQuitDefense(QIQI_STATUS_SHIELD);
    break;
  case QIQI_STATUS_SHADOW:
    if (hit_info.hit_type != HITQIQI_HITTYPE_MISS)
      m_qiqi_def.shadow_value.nomiss_hit++;
    else
      m_qiqi_def.shadow_value.nomiss_hit = 0;

    if (m_qiqi_def.shadow_value.nomiss_hit >= HITQIQI_DEFENSE_VALUE_SHADOW_HIT_CNT) {
      m_qiqi.dodge_rate -= HITQIQI_DEFENSE_VALUE_SHADOW_DODGE;
      OnQuitDefense(QIQI_STATUS_SHADOW);
    }
    break;
  case QIQI_STATUS_TATTOO:
    if (hit_info.hit_type == HITQIQI_HITTYPE_CRICITAL)
      m_qiqi_def.tattoo_value.circ_cnt++;

    if (m_qiqi_def.tattoo_value.circ_cnt >= HITQIQI_DEFENSE_VALUE_TATTOO_CIRC_CNT)
      OnQuitDefense(QIQI_STATUS_TATTOO);
    break;
  case QIQI_STATUS_FISH:
    if (CheckFishBroken())
      OnQuitDefense(QIQI_STATUS_FISH);
    break;
  case QIQI_STATUS_BLADEAMMOR:
    if (CheckBladeAmmorBroken())
      OnQuitDefense(QIQI_STATUS_BLADEAMMOR);
    break;
  case QIQI_STATUS_SMOKE:
    m_qiqi_def.smoke_value.time_rest = HITQIQI_DEFENSE_VALUE_SMOKE_TIME_CNT;
    break;
  case QIQI_STATUS_DEAD:
    break;
  default:
    break;
  }
  return WK_True;
}

WK_Bool CServerCore::OnQuitDefense(QIQI_STATUS  status) {
  m_qiqi.status = QIQI_STATUS_NORMAL;
  m_qiqi.energy_max = m_qiqi.energy_max * HITQIQI_ENERGY_VALUE_GROW;
  m_qiqi.ammor += HITQIQI_AMMOR_VALUE_GROW;
  m_qiqi.dodge_rate += HITQIQI_DODGE_VALUE_GROW;
  memset(&m_qiqi_def, 0, sizeof(HITQIQI_DEFENSE_VALUE));

  OnSyncQiqi();

  HITQIQIMSG_PACKAGE send_package_notify;
  send_package_notify.event = HITQIQIMSG_EVENT_NOTIFY;
  send_package_notify.event_time = CTime::GetCurrentTime();
  send_package_notify.notify.color = QIQI_NOTIFY_BACKGROUND_REAL;
  sprintf(send_package_notify.notify.notify_text, "琦琦退出了%s状态。", g_StatusDes[(int)status]);

  m_Net.SendClientMsg(&send_package_notify);
  return WK_True;
}

WK_Bool CServerCore::OnSyncQiqi() {
  HITQIQIMSG_PACKAGE send_package;
  send_package.event = HITQIQIMSG_EVENT_UPDATAQIQIDATA;
  send_package.event_time = CTime::GetCurrentTime();
  memcpy(&send_package.qiqidata, &m_qiqi, sizeof(QIQI_DATA));
  m_Net.SendClientMsg(&send_package);
  return WK_True;
}

WK_Bool CServerCore::OnTryBroken(HITQIQIMSG_PACKAGE* recv_package, void* socketin) {
  if (m_qiqi.status == QIQI_STATUS_NORMAL || m_qiqi.status == QIQI_STATUS_DEAD)
    return WK_True;

  if (recv_package->broken_time.GetDay() == CTime::GetCurrentTime().GetDay())
    return WK_True;

  HITQIQIMSG_PACKAGE send_package;
  send_package.event = HITQIQIMSG_EVENT_UPDATA_BROKEN_TIME;
  send_package.event_time = CTime::GetCurrentTime();
  m_Net.SendClientMsg(&send_package, socketin);

  OnQuitDefense(m_qiqi.status);
  SendNotify(recv_package);
  OnSyncQiqi();
  return WK_True;
}

WK_Bool CServerCore::CheckShieldBroken() {
  int hit_cnt = 0;
  CTime time_now = CTime::GetCurrentTime();

  hit_record_lock.lock();

  if (m_vHitRecord.empty() || m_vHitRecord.size()< HITQIQI_DEFENSE_VALUE_SHIELD_HIT_CNT)
  {
    hit_record_lock.unlock();
    return WK_False;
  }

  for (std::vector<HITQIQI_SERVER_HIT_RECORD>::const_iterator iter = m_vHitRecord.cend(); iter != m_vHitRecord.cbegin(); )
  {
    iter--;
    if (time_now  - iter->time > HITQIQI_DEFENSE_VALUE_SHILED_HIT_INTIME)
      break;

    if (iter->hit_info.hit_type == HITQIQI_HITTYPE_BLOCK)
      hit_cnt++;
  }

  hit_record_lock.unlock();

  return hit_cnt >= HITQIQI_DEFENSE_VALUE_SHIELD_HIT_CNT;

}

WK_Bool CServerCore::CheckFishBroken() {
  int player_cnt = 0;
  std::set<CString> player_name;
  CString name;
  player_name.clear();

  hit_record_lock.lock();

  if (m_vHitRecord.empty() || m_vHitRecord.size()< HITQIQI_DEFENSE_VALUE_FISH_HITCNT) {
    hit_record_lock.unlock();
    return WK_False;
  }

  for (std::vector<HITQIQI_SERVER_HIT_RECORD>::const_iterator iter = m_vHitRecord.cend();iter != m_vHitRecord.cbegin();) {
    iter--;

    if (iter->time < m_qiqi_def.fish_value.start_time)
      break;

    if (iter->hit_info.hit_type == HITQIQI_HITTYPE_MISS)
      continue;

    name = iter->hit_info.player_name;

    if (player_name.find(name) == player_name.end())
    {
      player_name.insert(name);
      player_cnt++;
    }
    else {
      break;
    }

  }
  player_name.clear();
  hit_record_lock.unlock();
  return player_cnt >= HITQIQI_DEFENSE_VALUE_FISH_HITCNT;
}

WK_Bool CServerCore::CheckBladeAmmorBroken() {
  int player_cnt = 0;
  double base_damage = -1.0;
  hit_record_lock.lock();

  if (m_vHitRecord.empty() || m_vHitRecord.size() < HITQIQI_DEFENSE_VALUE_BLADEAMMOR_HITCNT) {
    hit_record_lock.unlock();
    return WK_False;
  }

  for (std::vector<HITQIQI_SERVER_HIT_RECORD>::const_iterator iter = m_vHitRecord.cend(); iter != m_vHitRecord.cbegin();) {
    iter--;

    if (iter->time < m_qiqi_def.bladeammor_value.start_time)
      break;

    if (iter->hit_info.hit_damage >= base_damage)
    {
      base_damage = iter->hit_info.hit_damage;
      player_cnt++;
    }
    else {
      break;
    }
  }

  hit_record_lock.unlock();
  return player_cnt >= HITQIQI_DEFENSE_VALUE_BLADEAMMOR_HITCNT;
}