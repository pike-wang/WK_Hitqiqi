#include <UIlib.h>
#include <thread>
#include "WK_Hitqiqi_def.h"
#include "CClientCore.h"
#include "UIQiqiPanel.h"
#pragma comment(lib, "DuiLib_u.lib")

using namespace DuiLib;

class CHitqiqiClientWnd : public WindowImplBase
{
public:
  virtual LPCTSTR    GetWindowClassName() const { return _T("WK_Hitqiqi_Client"); }
  virtual CDuiString GetSkinFile() { return _T("WK_HitqiqiMainFrame.xml"); }
  virtual CDuiString GetSkinFolder() { return _T("xml"); }

  virtual void InitWindow();
  virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
  virtual DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass) override;
//   void SetQiqiData(QIQI_DATA* data);

  DUI_DECLARE_MESSAGE_MAP()

private:
  void OnClick(TNotifyUI& msg);
  WK_Bool OnShowNotify(HITQIQIMSG_PACKAGE* recv_package);
  WK_Bool OnShowHitText(HIT_TEXT_INFO* text_info);
  void OnTimeChange(int sec);
  std::thread m_RefreshLoop;
  CQiqiPanelUI* m_panel = nullptr;
  CTextUI* m_textup = nullptr;
  CTextUI* m_textdown = nullptr;
  CTextUI* m_texttime = nullptr;
};

