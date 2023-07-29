/* -----------------------------------------------------------------------------------------------------------------
COPYRIGHT (c) 2020 - 2120
Wave Knight Industry.,
ALL RIGHTS RESERVED

This software is a copyrighted work and/or information protected as a trade
secret. Legal rights of Wave Knight Industry. in this software are distinct from
ownership of any medium in which the software is embodied. Copyright or trade
secret notices included must be reproduced in any copies authorized by
Wave Knight Industry. The information in this software is subject to change without
notice and should not be considered as a commitment by Wave Knight Industry.
----------------------------------------------------------------------------------------------------------------- */
#pragma once

#include <UIlib.h>
#include "resource.h"
#pragma comment(lib, "DuiLib_u.lib")

using namespace DuiLib;

static DuiLib::CDuiString g_starttext[] = { L"冷却准备。",L"右腕重新固定完成。",L"机架内所有对接位置无误。" ,
L"明白，停止信号插口已经排出。", L"了解，插入插入栓。", L"开放脊椎传导系统，准备连接。",
L"插入栓固定完毕，开启第一次接触。", L"LCL开始注入。", L"LCL注入中，距完成还有20。", L"LCL浓度没有异常，压力正常。",
L"主电源接通，动力传送至所有回路，没有问题。", L"进行第二次接触。", L"A10神经连接无异常。", L"LCL，电量正常。思考形态以日语为基础。",
L"初期接触没有任何问题。", L"双向回路开通，同步率41.3 % 。", L"谐波均为正常值，内部电压维持现状，充电中。",
L"没有失控现象。", L"各部联动系统，确认运作。", L"准备发射！", L" 第一锁定栓释放。", L"确认解除。", L"脐带式管线桥开始移动。",
L"第二锁定栓释放，第一拘束器以及第二拘束器移除。", L"解除从一号到十五号的安全装置。", L"确认解除。", L"现在初号机的状态是[Free]。",
L"内部电源充电完毕，外部电源插座一切正常。", L"EVA初号机移动至发射口。", L"ALL GREEN线路通畅。",
L"发射准备完成！", L"発进！！!", };

class CWKStartWnd : public  DuiLib::WindowImplBase
{
public:
  virtual LPCTSTR    GetWindowClassName() const { return _T("WK_Start"); }
  virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  virtual  DuiLib::CDuiString GetSkinFile() { return _T("WK_StartFrame.xml"); }
  virtual  DuiLib::CDuiString GetSkinFolder() { return _T("xml"); }

  virtual void InitWindow();
  virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
  
  DuiLib::CUIRectProgress* m_progress = nullptr;
  DuiLib::CTextUI* m_text = nullptr;
  int m_num = 0;

};

void CWKStartWnd::InitWindow() {
  m_progress = dynamic_cast<DuiLib::CUIRectProgress*>(m_PaintManager.FindControl(_T("Progress")));
  m_text = dynamic_cast<DuiLib::CTextUI*>(m_PaintManager.FindControl(_T("maintext")));
  m_progress->SetValue(0);
  ::SetTimer(m_hWnd, 0, 1, NULL);
}

LRESULT CWKStartWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {

  LRESULT lRes = 0;

  switch (uMsg)
  {
  case(WM_TIMER):
    if (wParam == 1) {
      ::KillTimer(m_hWnd, 1);
      Close(true);
      return lRes;
    }
    m_num += 4;
    if (m_num >= 400) {
      ::KillTimer(m_hWnd, 0);
      m_text->SetText(g_starttext[31]);
      m_progress->SetValue(400);
      ::SetTimer(m_hWnd, 1, 500, NULL);
      return lRes;
    }
    m_progress->SetValue(m_num);
    m_text->SetText(g_starttext[(int)((double)m_num / 400.0 * 31)]);
    return lRes;
  default: break;
  }


  if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
  return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CWKStartWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
  styleValue &= ~WS_CAPTION;
  ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
  RECT rcClient;
  ::GetClientRect(*this, &rcClient);
  ::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

  m_PaintManager.Init(m_hWnd);
  m_PaintManager.AddPreMessageFilter(this);

  CDialogBuilder builder;
  CDuiString strResourcePath = m_PaintManager.GetResourcePath();
  if (strResourcePath.IsEmpty())
  {
    strResourcePath = m_PaintManager.GetInstancePath();
    strResourcePath += GetSkinFolder().GetData();
  }
  m_PaintManager.SetResourcePath(strResourcePath.GetData());

  switch (GetResourceType())
  {
  case UILIB_ZIP:
    m_PaintManager.SetResourceZip(GetZIPFileName().GetData(), true);
    break;
  case UILIB_ZIPRESOURCE:
  {
    HRSRC hResource = ::FindResource(m_PaintManager.GetResourceDll(), GetResourceID(), _T("ZIPRES"));
    if (hResource == NULL)
      return 0L;
    DWORD dwSize = 0;
    HGLOBAL hGlobal = ::LoadResource(m_PaintManager.GetResourceDll(), hResource);
    if (hGlobal == NULL)
    {
#if defined(WIN32) && !defined(UNDER_CE)
      ::FreeResource(hResource);
#endif
      return 0L;
    }
    dwSize = ::SizeofResource(m_PaintManager.GetResourceDll(), hResource);
    if (dwSize == 0)
      return 0L;
    m_lpResourceZIPBuffer = new BYTE[dwSize];
    if (m_lpResourceZIPBuffer != NULL)
    {
      ::CopyMemory(m_lpResourceZIPBuffer, (LPBYTE)::LockResource(hGlobal), dwSize);
    }
#if defined(WIN32) && !defined(UNDER_CE)
    ::FreeResource(hResource);
#endif
    m_PaintManager.SetResourceZip(m_lpResourceZIPBuffer, dwSize);
  }
  break;
  }

  CControlUI* pRoot = NULL;
  if (GetResourceType() == UILIB_RESOURCE)
  {
    STRINGorID xml(_ttoi(GetSkinFile().GetData()));
    pRoot = builder.Create(xml, _T("xml"), this, &m_PaintManager);
  }
  else
    pRoot = builder.Create(IDF_XML_START_FRAME, _T("xml"), this, &m_PaintManager);
  ASSERT(pRoot);
  if (pRoot == NULL)
  {
    MessageBox(NULL, _T("加载资源文件失败"), _T("Duilib"), MB_OK | MB_ICONERROR);
    ExitProcess(1);
    return 0;
  }
  m_PaintManager.AttachDialog(pRoot);
  m_PaintManager.AddNotifier(this);

  InitWindow();
  return 0;
}