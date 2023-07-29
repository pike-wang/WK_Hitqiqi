#include "stdafx.h"
#include "UIRectProgress.h"
namespace DuiLib
{
  LPCTSTR CUIRectProgress::GetClass() const
  {
    return DUI_CTR_RECTPROGRESS;
  }

  LPVOID CUIRectProgress::GetInterface(LPCTSTR pstrName) {
    if (_tcscmp(pstrName, DUI_CTR_RECTPROGRESS) == 0) return static_cast<CUIRectProgress*>(this);
    return CLabelUI::GetInterface(pstrName);
  }

  void CUIRectProgress::PaintStatusImage(HDC hDC) {

    if (m_nMax <= m_nMin) m_nMax = m_nMin + 1;
    if (m_nValue > m_nMax) m_nValue = m_nMax;
    if (m_nValue < m_nMin) m_nValue = m_nMin;

    RECT rc = { 0 };

    double percentage = (double)m_nValue / (double)m_nMax;
    double percentageA1 = (percentage - 0) / 0.125;
    double percentageB = (percentage - 0.125) / 0.25;
    double percentageC = (percentage - 0.375) / 0.25;
    double percentageD = (percentage - 0.625) / 0.25;
    double percentageA2 = (percentage - 0.875) / 0.125;
   
    if (percentageA1 > 0) {
      percentageA1 = percentageA1 >= 1.0 ? 1.0 : percentageA1;
      rc.left = ((double)( m_rcItem.right - m_rcItem.left) / 2)  * (1- percentageA1);
      rc.top = m_rcItem.bottom - m_rcItem.top - m_width;
      rc.right = (double)(m_rcItem.right - m_rcItem.left) / 2;
      rc.bottom = m_rcItem.bottom - m_rcItem.top;

      m_diFore.rcDestOffset = rc;
      DrawImage(hDC, m_diFore);
    }

    if (percentageB > 0) {
      percentageB = percentageB >= 1.0 ? 1.0 : percentageB;
      rc.left = 0;
      rc.top = (double)(m_rcItem.bottom - m_rcItem.top) * (1 - percentageB);
      rc.right = m_width;
      rc.bottom = m_rcItem.bottom - m_rcItem.top;

      m_diFore.rcDestOffset = rc;
      DrawImage(hDC, m_diFore);
    }

    if (percentageC > 0) {
      percentageC = percentageC >= 1.0 ? 1.0 : percentageC ;
      rc.left = 0;
      rc.top = 0;
      rc.right = (double)(m_rcItem.right - m_rcItem.left) * percentageC;
      rc.bottom = m_width;

      m_diFore.rcDestOffset = rc;
      DrawImage(hDC, m_diFore);
    }

    if (percentageD > 0) {
      percentageD = percentageD >= 1.0 ? 1.0 : percentageD;
      rc.left = m_rcItem.right - m_rcItem.left -m_width;
      rc.top = 0;
      rc.right = m_rcItem.right - m_rcItem.left;
      rc.bottom = (double)(m_rcItem.bottom - m_rcItem.top) * percentageD;

      m_diFore.rcDestOffset = rc;
      DrawImage(hDC, m_diFore);
    }

    if (percentageA2 > 0) {
      percentageA2 = percentageA2 >= 1.0 ? 1.0 : percentageA2;
      rc.left = ((double)(m_rcItem.right - m_rcItem.left) / 2) * (2 - percentageA2);
      rc.top = m_rcItem.bottom - m_rcItem.top - m_width;
      rc.right = m_rcItem.right - m_rcItem.left;
      rc.bottom = m_rcItem.bottom - m_rcItem.top;

      m_diFore.rcDestOffset = rc;
      DrawImage(hDC, m_diFore);
    }

    return;
  }

}
