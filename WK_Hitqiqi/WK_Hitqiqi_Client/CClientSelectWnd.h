#pragma once
#include <UIlib.h>
#include <windows.h>
#include "WK_Common_Define.h"
#include "WK_Hitqiqi_def.h"
#include "CHitqiqiButtonUI.h"

#pragma comment(lib, "DuiLib_u.lib")

using namespace DuiLib;

class CClientSelectWnd : public WindowImplBase
{
public:
  virtual LPCTSTR    GetWindowClassName() const { return _T("WK_Select_Wnd"); }
  virtual CDuiString GetSkinFile() { return _T("WK_HitqiqiSelectFrame.xml"); }
  virtual CDuiString GetSkinFolder() { return _T("xml"); }

  virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

  enum SELECT_MODE
  {
    SELECT_ERROR = 0,
    SELECT_NAME,
    SELECT_GIFT,
    SELECT_LEVELUP,
  };
  void SetMode(SELECT_MODE mode) { m_mode = mode; }
  void SetVec(std::vector<HITQIQI_LULUP_RANDOM>* Vec) { m_rewardVec = Vec; }

  virtual void InitWindow();
  void InitWindowbyName();
  void InitWindowbyGift();
  void InitWindowbyLVLUP();

  CDuiString GetInputName() { return m_name; }
  int GetChoose() { return m_Choose; }
  WK_Bool IsSuccess() { return m_success; }

  DUI_DECLARE_MESSAGE_MAP()
private:

  void OnClick(TNotifyUI& msg);
  void SetButtonStyle(CHitqiqiButtonUI* btn,
    const wchar_t* btn_name,
    const wchar_t* btn_Text,
    int btn_left,
    int btn_top,
    int btn_width,
    int btn_height,
    DWORD Btn_bkcolor1,
    DWORD Btn_bkcolor2);

  WK_Bool m_success = WK_False;
  CDuiString m_name;
  int m_Choose = -1;
  CVerticalLayoutUI* m_mainblock = nullptr;
  CTextUI* m_desText = nullptr;
  SELECT_MODE m_mode = SELECT_ERROR;
  std::vector<HITQIQI_LULUP_RANDOM> *m_rewardVec =nullptr;
};

