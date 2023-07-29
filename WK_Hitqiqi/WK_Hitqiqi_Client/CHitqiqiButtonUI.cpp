#include "CHitqiqiButtonUI.h"

void CHitqiqiButtonUI::DoEvent(TEventUI& event) {
  switch (event.Type)
  {
  case(UIEVENT_MOUSEENTER): 
    SetBorderSize(2); 
    SetBorderColor(0xFF7d7d00);
    Invalidate(); 
    return;
  case(UIEVENT_MOUSELEAVE): 
    SetBorderSize(0); 
    Invalidate(); 
    return;
  case(UIEVENT_BUTTONUP): 
    if (!CControlUI::Activate()) return;
    if (m_pManager != NULL) m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK, m_choose_value);
    return;
  default: break;
  }

  CButtonUI::DoEvent(event);
}