#pragma once
#include <UIlib.h>
using namespace DuiLib;

  class  CHitqiqiButtonUI : public CButtonUI
  {
  public:
    virtual void DoEvent(TEventUI& event);
    virtual void SetValue(int value) { m_choose_value = value; }

  private:
    int m_choose_value = -1;
  };


