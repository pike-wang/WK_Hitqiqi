#include "CHitqiqiClientWnd.h"

DUI_BEGIN_MESSAGE_MAP(CHitqiqiClientWnd, CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

void Thread_Refresh_Loop(HWND hWnd)
{
  while (true)
  {
    Sleep(10);
    ::PostMessage(hWnd, WK_MSG_HITQIQI_REFERSH, 0, 0);
  }
}

void CHitqiqiClientWnd::OnClick(TNotifyUI& msg) {
  if (msg.sType == _T("click"))
  {
    if (msg.pSender->GetName() == _T("main_panel"))
    {
      CClientCore::GetInstance()->OnHit(msg.ptMouse);
      return;
    }
    return __super::OnClick(msg);
  }
}

void CHitqiqiClientWnd::InitWindow() {

    m_panel = dynamic_cast<CQiqiPanelUI*>(m_PaintManager.FindControl(_T("main_panel")));
    m_textup = dynamic_cast<CTextUI*>(m_PaintManager.FindControl(_T("Textup")));
    m_textdown = dynamic_cast<CTextUI*>(m_PaintManager.FindControl(_T("Textdown")));
    m_texttime = dynamic_cast<CTextUI*>(m_PaintManager.FindControl(_T("time")));

    if (CClientCore::GetInstance()->GetQiqiData()->hp_max != 0) {
      int hp = (CClientCore::GetInstance()->GetQiqiData()->hp_now * 1000) / (CClientCore::GetInstance()->GetQiqiData()->hp_max);
      m_panel->SetHp(hp);
      m_panel->SetMp(CClientCore::GetInstance()->GetQiqiData()->energy * 100 / CClientCore::GetInstance()->GetQiqiData()->energy_max);
      m_panel->SetUserEnergy(CClientCore::GetInstance()->GetUserEnergy());
      OnTimeChange(CClientCore::GetInstance()->GetQiqiData()->end_sec);
   }
    m_RefreshLoop = std::thread(Thread_Refresh_Loop,m_hWnd);
    m_RefreshLoop.detach();
  }

LRESULT CHitqiqiClientWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {

    LRESULT lRes = 0;

    switch (uMsg)
    {
    case(WK_MSG_HITQIQI_UPDATAQIQIDATA): 
//       m_panel->SetHp((CClientCore::GetInstance()->GetQiqiData()->hp_now * 1000) / (CClientCore::GetInstance()->GetQiqiData()->hp_max));
//       m_panel->SetMp(CClientCore::GetInstance()->GetQiqiData()->energy);
//       m_panel->Invalidate();
//       OnTimeChange(CClientCore::GetInstance()->GetQiqiData()->end_sec);
      break;
    case(WK_MSG_HITQIQI_UPDATAUSERENERGY):
      //m_panel->SetUserEnergy(CClientCore::GetInstance()->GetUserEnergy());
      break;
    case(WK_MSG_HITQIQI_SHOWNOTIFY):
      OnShowNotify((HITQIQIMSG_PACKAGE * )wParam);
      break;
    case(WK_MSG_HITQIQI_REFERSH):
      m_panel->SetHp((CClientCore::GetInstance()->GetQiqiData()->hp_now * 1000) / (CClientCore::GetInstance()->GetQiqiData()->hp_max));
      m_panel->SetMp(CClientCore::GetInstance()->GetQiqiData()->energy * 100 / CClientCore::GetInstance()->GetQiqiData()->energy_max);
      m_panel->SetUserEnergy(CClientCore::GetInstance()->GetUserEnergy());
      m_panel->SetUserExp(CClientCore::GetInstance()->GetUserExpPer());
      m_panel->SetQiqiStatus(CClientCore::GetInstance()->GetQiqiData()->status);
      m_panel->Invalidate();
      OnTimeChange(CClientCore::GetInstance()->GetQiqiData()->end_sec);
      break;
    case(WK_MSG_HITQIQI_SHOWHITTEXT):
      OnShowHitText((HIT_TEXT_INFO*) wParam);
      break;
    default: 
      break;
    }

    if (uMsg != WM_SIZE && m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
    {
      return lRes;
    }

    return __super::HandleMessage(uMsg, wParam, lParam);
}

DuiLib::CControlUI* CHitqiqiClientWnd::CreateControl(LPCTSTR pstrClass)
{
  DuiLib::CControlUI* pControl = NULL;
  if (_tcsicmp(pstrClass, _T("QiqiPanel")) == 0)
    pControl = new CQiqiPanelUI;
  else
    return __super::CreateControl(pstrClass);

  return pControl;
}

WK_Bool CHitqiqiClientWnd::OnShowNotify(HITQIQIMSG_PACKAGE* recv_package) {
  if (m_textdown->IsVisible())
  {
    m_textup->SetText(m_textdown->GetText());
    m_textup->SetBkColor(m_textdown->GetBkColor());
    m_textup->SetBkColor2(m_textdown->GetBkColor2());
    m_textup->SetVisible(WK_True);
    m_textup->Invalidate();
  }

  m_textdown->SetText(CString(recv_package->notify.notify_text));

  switch (recv_package->notify.color)
  {
  case(QIQI_NOTIFY_BACKGROUND_NORMAL):
    m_textdown->SetBkColor(QIQI_NOTIFY_BACKGROUND_NORMAL_BK1);
    m_textdown->SetBkColor2(QIQI_NOTIFY_BACKGROUND_NORMAL_BK2);
    break;
  case(QIQI_NOTIFY_BACKGROUND_REAL):
    m_textdown->SetBkColor(QIQI_NOTIFY_BACKGROUND_REAL_BK1);
    m_textdown->SetBkColor2(QIQI_NOTIFY_BACKGROUND_REAL_BK2);
    break;
  case(QIQI_NOTIFY_BACKGROUND_EXCELLENT):
    m_textdown->SetBkColor(QIQI_NOTIFY_BACKGROUND_EXCELLENT_BK1);
    m_textdown->SetBkColor2(QIQI_NOTIFY_BACKGROUND_EXCELLENT_BK2);
    break;
  case(QIQI_NOTIFY_BACKGROUND_EPIC):
    m_textdown->SetBkColor(QIQI_NOTIFY_BACKGROUND_EPIC_BK1);
    m_textdown->SetBkColor2(QIQI_NOTIFY_BACKGROUND_EPIC_BK2);
    break;
  case(QIQI_NOTIFY_BACKGROUND_GRODEN):
    m_textdown->SetBkColor(QIQI_NOTIFY_BACKGROUND_GRODEN_BK1);
    m_textdown->SetBkColor2(QIQI_NOTIFY_BACKGROUND_GRODEN_BK2);
    break;
  case(QIQI_NOTIFY_BACKGROUND_RED):
    m_textdown->SetBkColor(QIQI_NOTIFY_BACKGROUND_RED_BK1);
    m_textdown->SetBkColor2(QIQI_NOTIFY_BACKGROUND_RED_BK2);
    break;
  case(QIQI_NOTIFY_BACKGROUND_ROT):
    m_textdown->SetBkColor(QIQI_NOTIFY_BACKGROUND_ROT_BK1);
    m_textdown->SetBkColor2(QIQI_NOTIFY_BACKGROUND_ROT_BK2);
    break;
  default: break;
  }
  m_textdown->SetVisible(WK_True);
  m_textdown->Invalidate();
  return WK_True;
}

WK_Bool CHitqiqiClientWnd::OnShowHitText(HIT_TEXT_INFO* text_info) {
  if (m_panel != NULL) {
    CDuiString strHit;
    if (text_info->hit_type == HITQIQI_HITTYPE_MISS)
      strHit = _T("MISS!");
    if (text_info->hit_type == HITQIQI_HITTYPE_BLOCK)
      strHit = _T("Block!");

    if (text_info->is_warlock_hit  && text_info->hit_type == HITQIQI_HITTYPE_NORMAL) {
      strHit.Format(_T("+%d"), text_info->hit_damage);
    }

    if (!text_info->is_warlock_hit && (text_info->hit_type == HITQIQI_HITTYPE_NORMAL || text_info->hit_type == HITQIQI_HITTYPE_CRICITAL))
      strHit.Format(_T("-%d"), text_info->hit_damage);

    m_panel->AddHitText(strHit, text_info->ptMouse, text_info->is_warlock_hit);
  }

  return WK_True;
}

void CHitqiqiClientWnd::OnTimeChange(int sec) {
  int second = sec % 60;
  int minute = sec % 3600 / 60;
  int hour = sec % (24*3600) / 3600;
  int day = sec / (24*3600);
  char buff[50] = { 0 };
  sprintf(buff, "¶áÉáµ¹¼ÆÊ±:%d:%02d:%02d:%02d", day, hour, minute, second);
  CString time(buff);
  m_texttime->SetText(time);
}