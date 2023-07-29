#pragma once

#include <UIlib.h>
#include "CClientCore.h"
#include "CWKStartWnd.h"
#include "CHitqiqiClientWnd.h"

using namespace DuiLib;

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
  CPaintManagerUI::SetInstance(hInstance);
  CPaintManagerUI::SetResourceDll(hInstance);

  HRESULT Hr = ::CoInitialize(NULL);
  if (FAILED(Hr)) return 0;

  CWKStartWnd StartFrame;
  StartFrame.Create(NULL, _T("WK_Start"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
  StartFrame.CenterWindow();
  if (!StartFrame.ShowModal())
    return 0;

  HWND  hwnd;

  hwnd=::FindWindow(_T("WK_Hitqiqi_Client"), _T("WK_Hitqiqi_Client"));
  if (hwnd!= nullptr)
  {
    ::PostMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
    ::CoUninitialize();
    return 0;
  }

  if (!CClientCore::GetInstance()->Init()) {
    ::MessageBox(NULL, L"无法与服务器建立连接！", L"错误" ,NULL);
    return 0;
  }

  CHitqiqiClientWnd MainFrame;
  hwnd = MainFrame.Create(NULL, _T("WK_Hitqiqi_Client"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
  CClientCore::GetInstance()->SetHWND(hwnd);
  MainFrame.SetIcon(IDI_ICO__HITQIQI);
  MainFrame.CenterWindow();
  MainFrame.ShowModal();

  CClientCore::GetInstance()->SaveAll();

  ::CoUninitialize();
  return 0;
}
