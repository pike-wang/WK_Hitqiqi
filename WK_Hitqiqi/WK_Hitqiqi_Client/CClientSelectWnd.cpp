#include "CClientSelectWnd.h"

DUI_BEGIN_MESSAGE_MAP(CClientSelectWnd, CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

void CClientSelectWnd::InitWindow() {

	m_mainblock = dynamic_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("main_block")));
	m_desText = dynamic_cast<CTextUI*>(m_PaintManager.FindControl(_T("des")));

	switch (m_mode)
	{
	case CClientSelectWnd::SELECT_ERROR: break;
	case CClientSelectWnd::SELECT_NAME: InitWindowbyName(); break;
	case CClientSelectWnd::SELECT_GIFT: InitWindowbyGift(); break;
	case CClientSelectWnd::SELECT_LEVELUP: InitWindowbyLVLUP();  break;
	default: break;
	}
}

void CClientSelectWnd::SetButtonStyle(CHitqiqiButtonUI* btn, 
                                                                const wchar_t* btn_name,
                                                                const wchar_t* btn_Text,
                                                                int btn_left,
                                                                int btn_top,
                                                                int btn_width,
                                                                int btn_height,
                                                               DWORD Btn_bkcolor1,
                                                               DWORD Btn_bkcolor2) {
  SIZE leftTop = { 0,0 };

  btn->SetFloat(true);

  if (btn_name!= nullptr)
    btn->SetName(btn_name);

  btn->SetText(btn_Text);
  leftTop = { btn_left,btn_top};
  btn->SetFixedXY(leftTop);
  btn->SetFixedWidth(btn_width);
  btn->SetFixedHeight(btn_height); 
  btn->SetFont(1);
  btn->SetTextPadding({ 4,3,4,3 });
  btn->SetBkColor(Btn_bkcolor1);
  btn->SetBkColor2(Btn_bkcolor2);
  leftTop = { 4,4 };
  btn->SetBorderRound(leftTop);
}

void CClientSelectWnd::InitWindowbyName() {
  ResizeClient(600, 180);
  m_desText->SetText(L"	欢迎来到点杀琦琦，请输入玩家姓名：");

  DuiLib::CEditUI* NameEdit = new DuiLib::CEditUI;
  NameEdit->SetFloat(true);
  NameEdit->SetName(L"NameEdit");
  NameEdit->SetText(L"");
  SIZE leftTop = { 0,0 };
  NameEdit->SetFixedXY(leftTop);
  NameEdit->SetFixedWidth(600);
  NameEdit->SetFixedHeight(60);
  NameEdit->SetFont(1);
  NameEdit->SetTextPadding({ 4,3,4,3 });
  NameEdit->SetMaxChar(9);
  m_mainblock->Add(NameEdit);

  CHitqiqiButtonUI* btnOK = new CHitqiqiButtonUI;
  SetButtonStyle(btnOK, L"btnOK", L"确定", 260, 70, 80, 40, 0xFFa5d8ff, 0xFFa5d8ff);
  m_mainblock->Add(btnOK);
}

void CClientSelectWnd::InitWindowbyGift() {
  ResizeClient(600, 60 * HITQIQI_GIFT_CNT + 60);
  m_desText->SetText(L"	请从以下天赋中选择一项：");

  int begin_top = 0;
  for (int index = 0; index<HITQIQI_GIFT_CNT; index++) {
    CHitqiqiButtonUI* btn = new CHitqiqiButtonUI;
    SetButtonStyle(btn, nullptr, g_GiftDes[index], 20, begin_top, 560, 40, 0xFFFFFFFF, 0xFFFFFFFF);
    btn->SetValue(index + 1);
    m_mainblock->Add(btn);
    begin_top += 60;
  }

}

void CClientSelectWnd::InitWindowbyLVLUP() {
  if (!m_rewardVec || m_rewardVec->empty())
    return;

  ResizeClient(600, 60 + m_rewardVec->size() * 60);
  m_desText->SetText(L"	恭喜您升级啦，请从以下增强中选择一项：");

  int begin_top = 0;
  CDuiString des;
  DWORD bk1 = 0xFFFFFFFF, bk2 = 0xFFFFFFFF;
  for (int index = 0; index < m_rewardVec->size(); index++) {
    CHitqiqiButtonUI* btn = new CHitqiqiButtonUI;

    des = L"";
    des += g_RandomDes[(*m_rewardVec)[index].type];
    des += g_LVLUPDes[(*m_rewardVec)[index].reward];

    switch ((*m_rewardVec)[index].type)
    {
    case HITQIQI_RANDOM_TYPE_HUGE: bk1 = QIQI_NOTIFY_BACKGROUND_GRODEN_BK1; bk2 = QIQI_NOTIFY_BACKGROUND_GRODEN_BK2; break;
    case HITQIQI_RANDOM_TYPE_SUBSTANT: bk1 = QIQI_NOTIFY_BACKGROUND_EPIC_BK1; bk2 = QIQI_NOTIFY_BACKGROUND_EPIC_BK2; break;
    case HITQIQI_RANDOM_TYPE_SIGIN:  bk1 = QIQI_NOTIFY_BACKGROUND_EXCELLENT_BK1; bk2 = QIQI_NOTIFY_BACKGROUND_EXCELLENT_BK2; break;
    case HITQIQI_RANDOM_TYPE_CONSIDE:  bk1 = QIQI_NOTIFY_BACKGROUND_REAL_BK1; bk2 = QIQI_NOTIFY_BACKGROUND_REAL_BK2; break;
    default:  bk1 = QIQI_NOTIFY_BACKGROUND_NORMAL_BK1;  bk2 = QIQI_NOTIFY_BACKGROUND_NORMAL_BK2; break;
    }
    SetButtonStyle(btn, nullptr, des, 20, begin_top, 560, 40, bk1, bk2);
    btn->SetValue(index);
    m_mainblock->Add(btn);
    begin_top += 60;
  }

}

LRESULT CClientSelectWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {

  LRESULT lRes = 0;
  if (uMsg != WM_SIZE && m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
  {
    return lRes;
  }

  return __super::HandleMessage(uMsg, wParam, lParam);
}

void CClientSelectWnd::OnClick(TNotifyUI& msg) {
  if (msg.pSender->GetName() == _T("btnOK"))
  {
    m_name = dynamic_cast<CEditUI*>(m_PaintManager.FindControl(_T("NameEdit")))->GetText();
    if (m_name.GetLength() != 0)
    {
      m_success = WK_True;
      ::PostMessage(m_hWnd, WM_CLOSE, 0, 0 );
    }
    return;
  }
  if (m_mode == SELECT_GIFT || m_mode == SELECT_LEVELUP)
  {
    m_Choose = msg.wParam;
    m_success = WK_True;
    ::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
    return;
  }
}