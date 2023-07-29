#ifndef __UIRECTPROGRESS_H__
#define __UIRECTPROGRESS_H__

#pragma once

namespace DuiLib
{
  class DUILIB_API CUIRectProgress :public CProgressUI
  {
  public:
    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    void PaintStatusImage(HDC hDC);

  protected:
    int m_width = 10;
  };

}

#endif // __UIRECTPROGRESS_H__