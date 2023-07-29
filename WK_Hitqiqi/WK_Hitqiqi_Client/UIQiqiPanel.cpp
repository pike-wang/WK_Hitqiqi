#include "stdafx.h"
#include "UIQiqiPanel.h"

namespace DuiLib 
{

  LPCTSTR CQiqiPanelUI::GetClass() const {
    return DUI_CTR_QIQIPANEL;
  }

  LPVOID CQiqiPanelUI::GetInterface(LPCTSTR pstrName) {
    if (_tcscmp(pstrName, DUI_CTR_QIQIPANEL) == 0) return static_cast<CQiqiPanelUI*>(this);
    return CControlUI::GetInterface(pstrName);
  }

  void CQiqiPanelUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) {
    CControlUI::SetAttribute(pstrName, pstrValue);
  }

  void CQiqiPanelUI::DoEvent(TEventUI& event) {

    switch (event.Type)
    {
    //case(UIEVENT_MOUSEMOVE): OnMouseMove(event); return;
    //case(UIEVENT_MOUSEENTER): OnMouseEnter(event); return;
    //case(UIEVENT_MOUSELEAVE): OnMouseLeave(); return;
    case(UIEVENT_BUTTONUP):
      if (!CControlUI::Activate()) return;
      if (m_pManager != NULL) m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
      return;
    case(UIEVENT_MOUSEENTER):
    case(UIEVENT_MOUSEMOVE):
    case(UIEVENT_SETCURSOR):
      break;
    default: break;
    }

    CLabelUI::DoEvent(event);
  }

  bool CQiqiPanelUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
  {
    CControlUI::DoPaint(hDC, rcPaint, pStopControl);

    m_PanelRect = this->GetPos();
    DoPaintHpSheet(hDC, rcPaint, pStopControl);
    DoPaintEnergySheet(hDC, rcPaint, pStopControl);
    DoPaintQiqiBody(hDC, rcPaint, pStopControl);
    DoPaintHitText(hDC, rcPaint, pStopControl);

    return true;
  }

  void CQiqiPanelUI::DoPaintHpSheet(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl) {

    Graphics graphics(hDC);
    Image hp_bk(L"image/hpline.png", false);

    UINT width = hp_bk.GetWidth();
    UINT height = hp_bk.GetHeight();

    graphics.DrawImage(&hp_bk, RectF(m_PanelRect.left+10, m_PanelRect.top+10, 60, 28));

    double em_rate = (double)m_hp_value / 1000.0;

    BYTE color_red = 0xFF * (1.0-em_rate);
    BYTE color_green = 0xFF * em_rate;
    INT length = 110 * em_rate;

    SolidBrush hpbrush(Color(255, color_red, color_green, 0x30));

    Rect rects;
    rects = Rect(m_PanelRect.left+20 , m_PanelRect.top+19, length, 9);
    graphics.FillRectangles(&hpbrush, &rects, 1);

    INT mp_length = 90.0 * ((double)m_mp_value / 100.0);

    LinearGradientBrush linGrBrush(
      Point(0, 0),
      Point(110, 0),
      Color(255, 0xee, 0x09, 0x79),
      Color(255, 0xff, 0x6a, 0x00));
    rects = Rect(m_PanelRect.left + 20, m_PanelRect.top + 28, mp_length, 5);
    graphics.FillRectangles(&linGrBrush, &rects, 1);
 }

  void CQiqiPanelUI::DoPaintEnergySheet(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl) {
    Graphics graphics(hDC);

    int hit_cnt = (int)m_energy / 100;
    char buff[10] = { 0 };
    sprintf(buff, "%d", hit_cnt);

    int index = 0;

    CDuiString file_path = L"image/num_";
    CDuiString num;
    UINT width_offset = 0;

    while (buff[index] != 0)
    {
      num = buff[index];
      num = file_path + num + L".png";

      Image num_p(num, false);

      graphics.DrawImage(&num_p, RectF(m_PanelRect.left + 10 + width_offset, m_PanelRect.top + 180, 10, 15));
      width_offset += 10;
      index++;
    }

    double process = (m_energy - ((int)m_energy / 100) * 100) / 100.0 ;
    LinearGradientBrush linGrBrushEnergy(
      Point(0, 0),
      Point(0, 100),
      Color(255, 0xfb, 0xc7, 0xd4),
      Color(255, 0x97, 0x96, 0xf0));

    Rect rects;
    rects = Rect(m_PanelRect.left , m_PanelRect.top + 101+ (100 * (1.0-process)), 5, 100*process);
    graphics.FillRectangles(&linGrBrushEnergy, &rects, 1);

    LinearGradientBrush linGrBrushExp(
      Point(0, 0),
      Point(0, 100),
      Color(255, 0x72, 0xC6, 0xEF),
      Color(255, 0x00, 0x4E, 0x8F));

    rects = Rect(m_PanelRect.left +5, m_PanelRect.top + 101 + (100 * (1.0 - m_exp)), 4, 100 * m_exp);
    graphics.FillRectangles(&linGrBrushExp, &rects, 1);
  }

  void CQiqiPanelUI::DoPaintHitText(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl) {

    hit_text_lock.lock();

    if (m_vhit.empty())
    {
      hit_text_lock.unlock();
      return;
    }

    for (std::vector<Hit_Text>::iterator iter = m_vhit.begin(); iter != m_vhit.end(); )
    {
      if (iter->frame >=200) {
        iter = m_vhit.erase(iter);
      }
      else {
        iter->frame++;
        DoPaintSingleHitText(*iter, hDC, rcPaint, pStopControl);
        iter++;
      }
    }

    hit_text_lock.unlock();
  }

  void CQiqiPanelUI::DoPaintSingleHitText(Hit_Text &hit_text, HDC hDC, const RECT& rcPaint, CControlUI* pStopControl) {

    Graphics graphics(hDC);

    SolidBrush brush(Color(255, 0xcc, 0, 0));
    SolidBrush brushG(Color(255, 0, 0xcc, 0));

    FontFamily fontfamily(L"Î¢ÈíÑÅºÚ");
    Font font(&fontfamily, 40*(200-2*hit_text.frame)/200.0, FontStyleBold, UnitPixel);

    PointF  pointf(hit_text.ptMouse.x, hit_text.ptMouse.y - 2*hit_text.frame);

    if (hit_text.is_warlock_hit)
      graphics.DrawString(hit_text.hit_text, -1, &font, pointf, &brushG);
    else
      graphics.DrawString(hit_text.hit_text, -1, &font, pointf, &brush);
  }

  void CQiqiPanelUI::DoPaintQiqiBody(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl) {

    Graphics graphics(hDC);

    CDuiString file_path = L"image/";

    switch (m_status)
    {
    case QIQI_STATUS_NORMAL:
      file_path = file_path + QIQI_PIC_NORMAL;
      break;
    case QIQI_STATUS_TEA:
      file_path = file_path + QIQI_PIC_TEA;
      break;
    case QIQI_STATUS_RUN:
      file_path = file_path + QIQI_PIC_RUN;
      break;
    case QIQI_STATUS_FITNESS:
      file_path = file_path + QIQI_PIC_FITNESS;
      break;
    case QIQI_STATUS_SHIELD:
      file_path = file_path + QIQI_PIC_SHIELD;
      break;
    case QIQI_STATUS_SHADOW:
      file_path = file_path + QIQI_PIC_SHADOW;
      break;
    case QIQI_STATUS_TATTOO:
      file_path = file_path + QIQI_PIC_TATTOO;
      break;
    case QIQI_STATUS_FISH:
      file_path = file_path + QIQI_PIC_FISH;
      break;
    case QIQI_STATUS_BLADEAMMOR:
      file_path = file_path + QIQI_PIC_BLADEAMMOR;
      break;
    case QIQI_STATUS_SMOKE:
      file_path = file_path + QIQI_PIC_SMOKE;
      break;
    case QIQI_STATUS_DEAD:
      file_path = file_path + QIQI_PIC_DEAD;
      break;
    case QIQI_STATUS_SUCCESS:
      file_path = file_path + QIQI_PIC_SUCCESS;
      break;
    default:
      file_path = file_path + QIQI_PIC_NORMAL;
      break;
    }

    Image qiqi_body(file_path, false);

    graphics.DrawImage(&qiqi_body, RectF(m_PanelRect.left + 25, m_PanelRect.top + 50, 100, 150));
  }

  void CQiqiPanelUI::SetHp(int hp)
  {
    m_hp_value = hp;
    if (hp < 0) m_hp_value = 0;
    if (hp > 1000) m_hp_value = 1000;
  };

  void CQiqiPanelUI::SetMp(int mp)
  {
    m_mp_value = mp;
    if (mp < 0) m_mp_value = 0;
    if (mp > 100) m_mp_value = 100;
  };

  void CQiqiPanelUI::SetUserEnergy(double energy) {
    m_energy = energy;
    if (energy < 0) m_energy = 0;
}
  void CQiqiPanelUI::SetUserExp(double exp) {
    m_exp = exp;
    if (m_exp < 0) m_exp = 0;
  }

  void CQiqiPanelUI::AddHitText(CDuiString hit_text, POINT ptMouse , WK_Bool is_warlock_hit) {
    Hit_Text hit;
    hit.hit_text = hit_text;
    hit.ptMouse = ptMouse;
    hit.is_warlock_hit = is_warlock_hit;
    hit.frame = 0;

    hit_text_lock.lock();
    m_vhit.push_back(hit);
    hit_text_lock.unlock();
  }

  void CQiqiPanelUI::SetQiqiStatus(QIQI_STATUS status) {
    m_status = status;
  }

}

