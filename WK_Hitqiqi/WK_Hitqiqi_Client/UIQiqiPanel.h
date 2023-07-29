#pragma once
#include <gdiplus.h>
#include <mutex>
#include <vector>
#include "WK_Hitqiqi_def.h"
#pragma  comment(lib, "gdiplus.lib")

using namespace Gdiplus;

class DUILIB_API Gdiplus::RectF;
struct DUILIB_API Gdiplus::GdiplusStartupInput;

namespace DuiLib
{
  struct Hit_Text
  {
    CDuiString hit_text;
    POINT ptMouse;
    WK_Bool is_warlock_hit;
    int frame;
  };

  class CQiqiPanelUI : public CLabelUI
  {
  public:
    virtual LPCTSTR GetClass() const;
    virtual LPVOID GetInterface(LPCTSTR pstrName);
    virtual void DoEvent(TEventUI& event);
    virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

    virtual void SetHp(int hp);
    virtual void SetMp(int mp);
    virtual void SetUserEnergy(double energy);
    virtual void SetUserExp(double exp);
    virtual void AddHitText(CDuiString hit_text, POINT ptMouse, WK_Bool is_warlock_hit);
    virtual void SetQiqiStatus(QIQI_STATUS status);

  private:
    void DoPaintHpSheet(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
    void DoPaintEnergySheet(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
    void DoPaintHitText(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
    void DoPaintSingleHitText(Hit_Text& hit_text, HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
    void DoPaintQiqiBody(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

    RECT m_PanelRect = { 0, 0 };
    int m_hp_value = -1;
    int m_mp_value = -1;
    double m_energy = -1;
    double m_exp = -1;
    QIQI_STATUS m_status = QIQI_STATUS_ERROR;

    std::vector<Hit_Text> m_vhit;
    std::mutex hit_text_lock;
  };
};
