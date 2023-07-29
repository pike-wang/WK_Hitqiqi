#pragma once
#include <gdiplus.h>
#pragma  comment(lib, "gdiplus.lib")

using namespace Gdiplus;

class DUILIB_API Gdiplus::RectF;
struct DUILIB_API Gdiplus::GdiplusStartupInput;


namespace DuiLib {

  class CSudokuPanel : public CLabelUI
  {

    enum SUDOKU_PANEL_MODE {
      MODE_MAIN = 0,
      MODE_SELECT = 1,
    };

    enum SUDOKU_PANEL_NUMBER_STATUS {
      NUMBER_UNSET = 0,
      NUMBER_SURE = 1,
      NUMBER_GUESS = 2,
      NUMBER_DISABLE = 3,
    };

    struct SudokuPanel_Data
    {
      int num;
      SUDOKU_PANEL_NUMBER_STATUS stauts;
      SudokuPanel_Data() {
        num = -1;
        stauts = NUMBER_UNSET;
      }
    };

  public:
    virtual LPCTSTR GetClass() const;
    virtual LPVOID GetInterface(LPCTSTR pstrName);
    virtual void DoEvent(TEventUI& event);
    virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
    virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    virtual void SetPossable(bool* possable);
    virtual void SetSelectedNumber(int  num);
    virtual  SudokuPanel_Data* GetData() { return &m_Data[0][0]; }
    virtual  void SetData(const void* date);
    virtual  Point GetSelect() { return m_Select; }
    virtual void SetTestData();
    ~CSudokuPanel() { CleanBrush(); }

  private:
   virtual bool DoPaintBorder(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
   virtual bool DoPaintNotice(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
   virtual bool DoPaintNumber(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

   void OnMouseMove(TEventUI& event);
   void OnMouseEnter(TEventUI& event);
   void OnMouseLeave();
   bool GetMouseInPoint(TEventUI& event);
   void CleanBrush();
   void OnButtonDown();

  private:
    SUDOKU_PANEL_MODE m_Mode = MODE_MAIN;
    BOOL m_MouseIn = FALSE;
    Point m_Notice = { -1, -1 };
    Point m_Select = { -1,-1 };
    RECT m_PanelRect = { 0, 0 };
    SudokuPanel_Data m_Data[9][9];

    Pen* m_BorderPen = nullptr;
    Pen* m_DottedPen = nullptr;
    SolidBrush* m_NoticeBrush = nullptr;
    SolidBrush* m_NumSureBrush = nullptr;
    SolidBrush* m_NumGuessBrush = nullptr;
    SolidBrush* m_NumDisableBrush = nullptr;
  };

}

