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

#include "WK_String.h"

// WK_String Base
#if(WK_STRING_BUILD_CONFIG_BASE)

WK_String::WK_String()
{
  m_str = NULL;
  m_len = 0;
}

WK_String::WK_String(const WK_String& strin)
{
  m_str = NULL;
  m_len = strin.m_len;
  if (m_len)
  {
    m_str = new char[m_len + 1];
    memcpy(m_str, strin.m_str, sizeof(char) * (m_len + 1));
  }
}

WK_String::WK_String(const char* strin)
{
  m_str = NULL;
  m_len = 0;
  m_len = (int)strlen(strin);
  if (m_len)
  {
    m_str = new char[m_len + 1];
    memcpy(m_str, strin, sizeof(char) * (m_len + 1));
  }
}

WK_String::~WK_String()
{
  delete[] m_str;
}

void WK_String::operator= (const char* strin)
{
  CleanStr();
  if (strin)
  {
    m_len = (int)strlen(strin);
    m_str = new char[m_len + 1];
    memcpy(m_str, strin, sizeof(char) * (m_len + 1));
  }
}

void WK_String::operator= (const WK_String& strin)
{
  CleanStr();
  m_len = strin.m_len;
  if (m_len)
  {
    m_str = new char[m_len + 1];
    memcpy(m_str, strin.m_str, sizeof(char) * (m_len + 1));
  }
}

void 	WK_String::CleanStr()
{
  delete[] m_str;
  m_str = NULL;
  m_len = 0;
}

const char* WK_String::ToChar() const
{
  return m_str;
}

BOOL WK_String::SetSize(unsigned int size)
{
  if (size < 1) {
    return WK_False;
  }
  CleanStr();
  m_len = size;
  m_str = new char[m_len + 1];
  memset(m_str, 0, sizeof(char) * (m_len + 1));
  return WK_True;
}

BOOL WK_CopyStr(WK_String& str, const char* strin)
{
  str.CleanStr();
  if (!strin) {
    return WK_True;
  }

  str.m_len = (int)strlen(strin);
  if (str.m_len)
  {
    str.m_str = new char[str.m_len + 1];
    memcpy(str.m_str, strin, sizeof(char) * (str.m_len + 1));
  }
  return WK_True;
}

BOOL WK_CopyStr(char* strout, const WK_String& str)
{
  if (!str.m_len || !str.m_str) {
    strout[0] = 0; return WK_True;
  }
  memcpy(strout, str.m_str, sizeof(char) * (str.m_len + 1));
  return WK_True;
}

WK_Bool WK_String::FindStr(const char* str, int& firstlocal, int& cnt, unsigned int cmd) const
{
  firstlocal = -1;
  cnt = 0;

  if (m_str == NULL || m_len == 0)
    return WK_False;

  if (str == NULL)
    return WK_False;

  WK_Bool hit_mark = WK_False;

  for (int local_str = 0; m_str[local_str] != 0; local_str++) {

    int strin = 0;
    for (; m_str[local_str + strin] != 0 && str[strin] != 0; strin++) {

      hit_mark = WK_False;

      if (m_str[local_str + strin] == str[strin])
        hit_mark = WK_True;

      if (!hit_mark && cmd & WK_STRING_IGNORE_CAPTION) {
        char c = 0;
        if (str[strin] >= 'a' && str[strin] <= 'z')
          c = str[strin] - 32;

        if (str[strin] >= 'A' && str[strin] <= 'Z')
          c = str[strin] + 32;

        if (m_str[local_str + strin] == c)
          hit_mark = WK_True;
      }

      if (!hit_mark)
        break;

      if (str[strin + 1] == 0) {
        cnt++;

        if (firstlocal == -1)
          firstlocal = local_str;

        if (!(cmd & WK_STRING_REPEAT_FIND))
          return WK_True;
      }

    }
  }

  return cnt != 0;
}

#endif

//Debug Founctions
#if(WK_STRING_BUILD_CONFIG_DEBUG)
BOOL WK_String::WK_Debug()
{
  using namespace std;
  BOOL DebugMark = WK_True;
  cout << "----------------WK_String Debug Procedure Start Now---------------" << endl;
#if(WK_STRING_BUILD_CONFIG_STRING)
  DebugMark = DebugMark && CString_Debug();
#endif
  cout << "----------------WK_String Debug Procedure End---------------------" << endl;
  if (DebugMark)
  {
    cout << "Result: All Check." << endl << endl << endl;
  }
  else
  {
    cout << "Result: ERROR!" << endl << endl << endl;
  }
  return DebugMark;
}
#endif

// String Support
#if(WK_STRING_BUILD_CONFIG_STRING)

WK_String::WK_String(const std::string& strin)
{
  m_str = NULL;
  m_len = 0;
  m_len = (int)strin.length();
  if (m_len)
  {
    m_str = new char[m_len + 1];
    memcpy(m_str, strin.c_str(), sizeof(char) * (m_len + 1));
  }
}

std::string WK_String::ToString() const
{
  if (!m_len || !m_str) {
    return std::string("");
  }
  return std::string(m_str);
}

void  WK_String::operator= (const std::string& strin)
{
  CleanStr();
  m_len = (int)strin.length();
  m_str = new char[m_len + 1];
  memcpy(m_str, strin.c_str(), sizeof(char) * (m_len + 1));
}

BOOL WK_CopyStr(WK_String& str, const std::string strin)
{
  str.CleanStr();
  str.m_len = (int)strin.length();
  if (!str.m_len) {
    return WK_True;
  }
  str.m_str = new char[str.m_len + 1];
  memcpy(str.m_str, strin.c_str(), sizeof(char) * (str.m_len + 1));
  return WK_True;
}

BOOL WK_CopyStr(std::string& strout, const WK_String& str)
{
  if (!str.m_len || !str.m_str) {
    strout = ""; return WK_True;
  }
  strout = str.m_str;
  return WK_True;
}

#endif

// CString Support
#if(WK_STRING_BUILD_CONFIG_CSTRING)

#ifdef _UNICODE
WK_String::WK_String(const CString& strin)
{
  m_str = NULL;
  m_len = 0;
  m_len = WideCharToMultiByte(CP_ACP, 0, strin, strin.GetLength(), NULL, 0, NULL, NULL);
  if (m_len)
  {
    m_str = new char[m_len + 1];
    WideCharToMultiByte(CP_ACP, 0, strin, strin.GetLength(), m_str, m_len, NULL, NULL);
    m_str[m_len] = 0;
  }
}

void WK_String::operator= (const CString& strin)
{
  CleanStr();
  m_len = WideCharToMultiByte(CP_ACP, 0, strin, strin.GetLength(), NULL, 0, NULL, NULL);
  m_str = new char[m_len + 1];
  WideCharToMultiByte(CP_ACP, 0, strin, strin.GetLength(), m_str, m_len, NULL, NULL);
  m_str[m_len] = 0;
}

BOOL WK_CopyStr(WK_String& str, const CString strin)
{
  str.CleanStr();
  str.m_len = WideCharToMultiByte(CP_ACP, 0, strin, strin.GetLength(), NULL, 0, NULL, NULL);
  if (!str.m_len) {
    return WK_True;
  }
  str.m_str = new char[str.m_len + 1];
  WideCharToMultiByte(CP_ACP, 0, strin, strin.GetLength(), str.m_str, str.m_len, NULL, NULL);
  str.m_str[str.m_len] = 0;
  return WK_True;
}
#else
WK_String::WK_String(const CString& strin)
{
  m_str = NULL;
  m_len = 0;
  m_len = strin.GetLength();
  if (m_len)
  {
    m_str = new char[m_len + 1];
    strcpy_s(m_str, m_len + 1, strin);
    m_str[m_len] = 0;
  }
}

void WK_String::operator= (const CString& strin)
{
  CleanStr();
  m_len = strin.GetLength();
  if (m_len)
  {
    m_str = new char[m_len + 1];
    strcpy_s(m_str, m_len + 1, strin);
    m_str[m_len] = 0;
  }
}

BOOL WK_CopyStr(WK_String& str, const CString strin)
{
  str.CleanStr();
  str.m_len = strin.GetLength();
  if (!str.m_len) {
    return WK_True;
  }
  str.m_str = new char[str.m_len + 1];
  strcpy_s(str.m_str, str.m_len + 1, strin);
  str.m_str[str.m_len] = 0;
  return WK_True;
}

#endif // _UNICODE

CString WK_String::ToCString() const
{
  return CString(m_str);
}

BOOL WK_CopyStr(CString& strout, const WK_String& str)
{
  if (!str.m_len || !str.m_str) {
    strout = "";
  }
  strout = str.m_str;
  return WK_True;
}

#if(WK_STRING_BUILD_CONFIG_DEBUG)
BOOL WK_String::CString_Debug()
{
  using namespace std;
  BOOL DebugMark = WK_True;
  cout << "CString Deubg:" << endl;

  {
    CString debug_blank;
    CString debug_wk = "Wave Knight Industry";

    WK_String debug_str(debug_wk);
    if (20 == debug_str.m_len && 0 == strcmp(debug_str.m_str, "Wave Knight Industry")) {
      cout << "PASS----";
    }
    else {
      cout << "ERROR----"; DebugMark = WK_False;
    }
    cout << "CString Constructor" << endl;

    WK_String debug_str_blank(debug_blank);
    if (0 == debug_str_blank.m_len && NULL == debug_str_blank.m_str) {
      cout << "PASS----";
    }
    else {
      cout << "ERROR----"; DebugMark = WK_False;
    }
    cout << "CString Blank Constructor" << endl;
  }

  {
    CString debug_blank;
    CString debug_wk = "Wave Knight Industry";

    WK_String debug_str = debug_wk;
    if (20 == debug_str.m_len && 0 == strcmp(debug_str.m_str, "Wave Knight Industry")) {
      cout << "PASS----";
    }
    else {
      cout << "ERROR----"; DebugMark = WK_False;
    }
    cout << "CString Operator=" << endl;

    WK_String debug_str_blank = debug_blank;
    if (0 == debug_str_blank.m_len && NULL == debug_str_blank.m_str) {
      cout << "PASS----";
    }
    else {
      cout << "ERROR----"; DebugMark = WK_False;
    }
    cout << "CString Blank Operator=" << endl;
  }

  {
    CString debug_blank;
    CString debug_wk = "Wave Knight Industry";

    WK_String debug_str;
    WK_CopyStr(debug_str, debug_wk);
    if (20 == debug_str.m_len && 0 == strcmp(debug_str.m_str, "Wave Knight Industry")) {
      cout << "PASS----";
    }
    else {
      cout << "ERROR----"; DebugMark = WK_False;
    }
    cout << "CString WK_CopyStr" << endl;

    WK_String debug_str_blank;
    WK_CopyStr(debug_str_blank, debug_blank);
    if (0 == debug_str_blank.m_len && NULL == debug_str_blank.m_str) {
      cout << "PASS----";
    }
    else {
      cout << "ERROR----"; DebugMark = WK_False;
    }
    cout << "CString Blank WK_CopyStr" << endl;
  }

  {
    WK_String debug_blank;
    WK_String debug_wk = "Wave Knight Industry";

    CString debug_str;
    WK_CopyStr(debug_str, debug_wk);
    if ("Wave Knight Industry" == debug_str) {
      cout << "PASS----";
    }
    else {
      cout << "ERROR----"; DebugMark = WK_False;
    }
    cout << "CString WK_CopyStr Set" << endl;

    CString debug_str_blank;
    WK_CopyStr(debug_str_blank, debug_blank);
    if ("" == debug_str_blank) {
      cout << "PASS----";
    }
    else {
      cout << "ERROR----"; DebugMark = WK_False;
    }
    cout << "CString Blank WK_CopyStr Set" << endl;
  }

  {
    WK_String debug_blank;
    WK_String debug_wk = "Wave Knight Industry";

    CString debug_str = debug_wk.ToCString();
    if ("Wave Knight Industry" == debug_str) {
      cout << "PASS----";
    }
    else {
      cout << "ERROR----"; DebugMark = WK_False;
    }
    cout << "CString ToCString Set" << endl;

    CString debug_str_blank = debug_blank.ToCString();;
    if ("" == debug_str_blank) {
      cout << "PASS----";
    }
    else {
      cout << "ERROR----"; DebugMark = WK_False;
    }
    cout << "CString Blank ToCString Set" << endl;
  }

  return DebugMark;
}
#endif //WK_STRING_BUILD_CONFIG_DEBUG

#endif

// IO Stearm Support
#if(WK_STRING_BUILD_CONFIG_IO)

BOOL WK_String::IO_Print()
{
  if (!m_len || !m_str) {
    std::cout << "";
    return WK_True;
  }
  std::cout << m_str;
  return WK_True;
}

std::ostream& operator<< (std::ostream& cout, const WK_String& str)
{
  cout << str.m_str;
  return cout;
}

void operator >> (std::istream& cin, WK_String& str)
{
  std::string mid;
  cin >> mid;
  str.m_len = (int)mid.length();
  str.CleanStr();
  str.m_str = new char[str.m_len + 1];
  memcpy(str.m_str, mid.c_str(), sizeof(char) * (str.m_len + 1));
}

#endif