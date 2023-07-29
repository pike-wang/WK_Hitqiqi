#include "stdafx.h"
#include "CSudokuPanel.h"
#include <stack>
#include <atlconv.h>

namespace DuiLib {

  void CSudokuPanel::SetPossable(bool* possable) {

    if (m_Mode != MODE_SELECT)
      return;

    for (int j = 0; j < 3; j++)
    {
      for (int i = 0; i < 3; i++)
      {
        m_Data[i][j].stauts = *possable ? NUMBER_SURE : NUMBER_DISABLE;
        possable++;
      }
    }
  }

  LPCTSTR CSudokuPanel::GetClass() const {
    return DUI_SUDOKUPANEL_CLASS;
  }

  LPVOID CSudokuPanel::GetInterface(LPCTSTR pstrName) {
    if (_tcscmp(pstrName, DUI_SUDOKUPANEL_CLASS) == 0) return static_cast<CSudokuPanel*>(this);
    return CControlUI::GetInterface(pstrName);
  }

  bool CSudokuPanel::GetMouseInPoint(TEventUI& event) {

    POINT p = event.ptMouse;
    int x = (int)(p.x - m_PanelRect.left) / 50;
    int y = (int)(p.y - m_PanelRect.top) / 50;

    Point newp(x, y);
    if (m_Notice.X != x || m_Notice.Y != y) {
      m_Notice = newp;
      return true;
    }
    return false;
  }

  void CSudokuPanel::OnMouseMove(TEventUI& event) {
    if (GetMouseInPoint(event))
      Invalidate();
  }
  void CSudokuPanel::OnMouseEnter(TEventUI& event) {
    m_MouseIn = true;
    if (GetMouseInPoint(event))
      Invalidate();
  }
  void CSudokuPanel::OnMouseLeave() {
    m_MouseIn = false;
    m_Notice = { -1,-1 };
    Invalidate();
  }

  void CSudokuPanel::DoEvent(TEventUI& event) {

    switch (event.Type)
    {
    case(UIEVENT_MOUSEMOVE): OnMouseMove(event); return;
    case(UIEVENT_MOUSEENTER): OnMouseEnter(event); return;
    case(UIEVENT_MOUSELEAVE): OnMouseLeave(); return;
    case(UIEVENT_BUTTONUP): OnButtonDown(); return;
    default: break;
    }

    CLabelUI::DoEvent(event);
  }

  void CSudokuPanel::OnButtonDown() {

    m_Select = m_Notice;

    if (m_Mode==MODE_SELECT) {
      if (m_Data[m_Select.X][m_Select.Y].stauts != NUMBER_SURE)
        return;

      if (m_pManager != NULL) m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK, m_Select.X +m_Select.Y *3 +1);
      return;
    }
    if (m_pManager != NULL) m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
  }

  bool CSudokuPanel::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
  {
    CControlUI::DoPaint(hDC, rcPaint, pStopControl);

    m_PanelRect = this->GetPos();
    DoPaintBorder(hDC, rcPaint, pStopControl);
    DoPaintNumber(hDC, rcPaint, pStopControl);
    DoPaintNotice(hDC, rcPaint, pStopControl);

    return true;
  }

  bool CSudokuPanel::DoPaintNumber(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl) {

    Graphics graphics(hDC);
    FontFamily fontfamily(L"Î¢ÈíÑÅºÚ");
    Font font(&fontfamily, 30, FontStyleBold, UnitPixel);

    if (!m_NumSureBrush) {
      m_NumSureBrush = new SolidBrush(Color(255, 0x03, 0x10, 0x25));
    }

    if (!m_NumGuessBrush) {
      m_NumGuessBrush = new SolidBrush(Color(255, 0xce, 0x3b, 0x3b));
    }

    if (!m_NumDisableBrush) {
      m_NumDisableBrush = new SolidBrush(Color(255, 0x86, 0x8e, 0x96));
    }

    PointF  pointf;
    CDuiString strNum;

    int MaxLine = m_Mode == MODE_MAIN ? 9 : 3;

    for (int i =0; i<MaxLine; i++)
    {
      for (int j =0; j<MaxLine; j++)
      {
        switch (m_Data[i][j].stauts)
        {
        case(NUMBER_UNSET): break;
        case(NUMBER_SURE): 
          pointf = PointF((REAL)m_PanelRect.left + i *50 +11, (REAL)m_PanelRect.top + j * 50 + 5);
          strNum.Format(_T("%d"), m_Data[i][j].num);
          graphics.DrawString(strNum, 1, &font, pointf, m_NumSureBrush);
          break;
        case(NUMBER_GUESS):
          pointf = PointF((REAL)m_PanelRect.left + i * 50 + 11, (REAL)m_PanelRect.top + j * 50 + 5);
          strNum.Format(_T("%d"), m_Data[i][j].num);
          graphics.DrawString(strNum, 1, &font, pointf, m_NumGuessBrush);
          break;
        case(NUMBER_DISABLE):
          pointf = PointF((REAL)m_PanelRect.left + i * 50 + 11, (REAL)m_PanelRect.top + j * 50 + 5);
          strNum.Format(_T("%d"), m_Data[i][j].num);
          graphics.DrawString(strNum, 1, &font, pointf, m_NumDisableBrush);
          break;
        default: break;
        }
      }
    }

    return false;
  }

  bool CSudokuPanel::DoPaintNotice(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl) {

    if (!m_MouseIn)
      return true;

    Graphics graphics(hDC);

    if (!m_NoticeBrush) {
      m_NoticeBrush = new SolidBrush(Gdiplus::Color(0x60, 0x8a, 0xce, 0x57));
    }

    Rect rects;

    if (m_Mode == MODE_SELECT) {
      rects = Rect(m_PanelRect.left + (m_Notice.X  % 3 )* 50, m_PanelRect.top + m_Notice.Y  * 50, 50, 50);
      graphics.FillRectangles(m_NoticeBrush, &rects, 1);
      return true;
    }

    int zone = (m_Notice.Y / 3) + (m_Notice.Y / 3) * 2 + m_Notice.X / 3;

    rects = Rect(m_PanelRect.left + (zone % 3 * 150),m_PanelRect.top + (zone / 3 * 150 ), 150, 150);
    graphics.FillRectangles(m_NoticeBrush,&rects,1);

    if (zone%3 != 0) {
      rects = Rect(m_PanelRect.left, m_PanelRect.top +m_Notice.Y * 50, (zone % 3)* 150, 50);
      graphics.FillRectangles(m_NoticeBrush, &rects, 1);
    }

    if (zone > 2) {
      rects = Rect(m_PanelRect.left + m_Notice.X * 50, m_PanelRect.top, 50, (zone / 3) * 150);
      graphics.FillRectangles(m_NoticeBrush, &rects, 1);
     }

    if (zone % 3 < 2) {
      rects = Rect(m_PanelRect.left + (zone % 3 * 150 + 150), m_PanelRect.top + m_Notice.Y * 50, (2- zone % 3) * 150, 50);
      graphics.FillRectangles(m_NoticeBrush, &rects, 1);
    }

    if (zone % 3 < 6) {
      rects = Rect(m_PanelRect.left + m_Notice.X * 50, m_PanelRect.top + (zone / 3 * 150 + 150), 50, (2 - zone / 3) * 150);
      graphics.FillRectangles(m_NoticeBrush, &rects, 1);
    }

    return true;
  }

  bool CSudokuPanel::DoPaintBorder(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl) {

    Graphics graphics(hDC);

    GraphicsPath gBorderPath;
    PointF pOutBorder[] = {
                PointF((REAL)m_PanelRect.left-1, (REAL)m_PanelRect.top-1),
                PointF((REAL)m_PanelRect.left-1, (REAL)m_PanelRect.bottom-1),
                PointF((REAL)m_PanelRect.right-1, (REAL)m_PanelRect.bottom-1),
                PointF((REAL)m_PanelRect.right-1, (REAL)m_PanelRect.top-1),
                PointF((REAL)m_PanelRect.left-1, (REAL)m_PanelRect.top-1),
    };
    gBorderPath.AddLines(pOutBorder,5);

    if (!m_BorderPen)
      m_BorderPen = new Pen(Color(0x29, 0x2c, 0x2e),  2);

    graphics.DrawPath(m_BorderPen, &gBorderPath);

    if (m_Mode == MODE_MAIN) {
      graphics.DrawLine(m_BorderPen, PointF((REAL)m_PanelRect.left + 150, (REAL)m_PanelRect.top), PointF((REAL)m_PanelRect.left + 150, (REAL)m_PanelRect.bottom));
      graphics.DrawLine(m_BorderPen, PointF((REAL)m_PanelRect.left + 300, (REAL)m_PanelRect.top), PointF((REAL)m_PanelRect.left + 300, (REAL)m_PanelRect.bottom));
      graphics.DrawLine(m_BorderPen, PointF((REAL)m_PanelRect.left, (REAL)m_PanelRect.top + 150), PointF((REAL)m_PanelRect.right, (REAL)m_PanelRect.top + 150));
      graphics.DrawLine(m_BorderPen, PointF((REAL)m_PanelRect.left, (REAL)m_PanelRect.top + 300), PointF((REAL)m_PanelRect.right, (REAL)m_PanelRect.top + 300));
    };

    if (!m_DottedPen) {
      m_DottedPen = new Pen(Color(0x29, 0x2c, 0x2e), 1);
      m_DottedPen->SetDashStyle(DashStyleDashDot);
    }
    int DrawTimes = m_Mode == MODE_MAIN ? 6 : 2;
    REAL offsite = -50;

    for (int index = 0; index<DrawTimes; index++)
    {
      offsite += (index % 2 == 0) ? 100 : 50;
      graphics.DrawLine(m_DottedPen, PointF((REAL)m_PanelRect.left + offsite, (REAL)m_PanelRect.top), PointF((REAL)m_PanelRect.left + offsite, (REAL)m_PanelRect.bottom));
      graphics.DrawLine(m_DottedPen, PointF((REAL)m_PanelRect.left , (REAL)m_PanelRect.top + offsite), PointF((REAL)m_PanelRect.right, (REAL)m_PanelRect.top + offsite));
    }

    return true;
  }

  void CSudokuPanel::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) {

    LPTSTR pstr = NULL;
    if (_tcscmp(pstrName, _T("mode")) == 0) {
      if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
      LPTSTR pstr = NULL;
      DWORD mode = _tcstoul(pstrValue, &pstr, 16);
      m_Mode = (SUDOKU_PANEL_MODE)mode;
      if (m_Mode == MODE_SELECT) {
        int index = 1;
        for (int j = 0; j < 3; j++)
        {
          for (int i = 0; i < 3; i++)
          {
            m_Data[i][j].num = index;
            index++;
          }
        }
      }
    }
    else
      CControlUI::SetAttribute(pstrName, pstrValue);
  }

  void CSudokuPanel::CleanBrush() {

    delete m_BorderPen;
    delete m_DottedPen;
    delete m_NoticeBrush;
    delete m_NumSureBrush;
    delete m_NumGuessBrush;
    delete m_NumDisableBrush;

  }

  void CSudokuPanel::SetSelectedNumber(int  num) {
    if (m_Data[m_Select.X][m_Select.Y].num != num) {
      m_Data[m_Select.X][m_Select.Y].num = num;
      m_Data[m_Select.X][m_Select.Y].stauts = NUMBER_SURE;
      Invalidate();
    }
  }

  void CSudokuPanel::SetData(const void* data) {
   if (!data) {
     memset(m_Data, 0, sizeof(SudokuPanel_Data) * 81);
   }
   else {
     memcpy(m_Data, data, sizeof(SudokuPanel_Data) * 81);
   }
   Invalidate();
  }

  void CSudokuPanel::SetTestData() {
    struct Testpoint
    {
      Point pt;
      int num;
      Testpoint(int x, int y, int numin) {
        pt.X = x;
        pt.Y = y;
        num = numin;
      };
    };
    stack<Testpoint> testpoint;
    testpoint.push(Testpoint(0, 0, 8));
    testpoint.push(Testpoint(1, 2, 7));
    testpoint.push(Testpoint(1, 3, 5));
    testpoint.push(Testpoint(1, 8, 9));
    testpoint.push(Testpoint(2, 1, 3));
    testpoint.push(Testpoint(2, 6, 1));
    testpoint.push(Testpoint(2, 7, 8));

    testpoint.push(Testpoint(3, 1, 6));
    testpoint.push(Testpoint(3, 5, 1));
    testpoint.push(Testpoint(3, 7, 5));
    testpoint.push(Testpoint(4, 2, 9));
    testpoint.push(Testpoint(4, 4, 4));
    testpoint.push(Testpoint(5, 3, 7));
    testpoint.push(Testpoint(5, 4, 5));

    testpoint.push(Testpoint(6, 2, 2));
    testpoint.push(Testpoint(6, 4, 7));
    testpoint.push(Testpoint(6, 8, 4));
    testpoint.push(Testpoint(7, 5, 3));
    testpoint.push(Testpoint(7, 6, 6));
    testpoint.push(Testpoint(7, 7, 1));
    testpoint.push(Testpoint(8, 6, 8));


    while (!testpoint.empty())
    {
      Testpoint p = testpoint.top();
      m_Data[p.pt.X][p.pt.Y].num = p.num;
      m_Data[p.pt.X][p.pt.Y].stauts = NUMBER_SURE;
      testpoint.pop();
    }

    return;
  }

};
