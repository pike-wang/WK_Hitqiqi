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

#include <cstring>
#include <string>
#include <iostream>
#include <atlstr.h>

#include "Wk_Common_Define.h"

#ifndef  _WK_STRING_BUILD_CONFIG_
#define  _WK_STRING_BUILD_CONFIG_

/* -----------------------------------------------------------------------------------------------------------------
Main Control
----------------------------------------------------------------------------------------------------------------- */
#define WK_STRING_BUILD_CONFIG_ENABLE_ALL  1      //Enable All Founctions
#define WK_STRING_BUILD_CONFIG_ENABLE_SUP  0     //Enable Support Founctions
/* -----------------------------------------------------------------------------------------------------------------
Basic Settings
----------------------------------------------------------------------------------------------------------------- */
// WK_String Base
#define WK_STRING_BUILD_CONFIGS_BASE   1

//Debug Founctions
#define WK_STRING_BUILD_CONFIGS_DEBUG   1

// String Support
#define WK_STRING_BUILD_CONFIGS_STRING  1

// CString Support
#define WK_STRING_BUILD_CONFIGS_CSTRING  1

// IO Stearm Support
#define WK_STRING_BUILD_CONFIGS_IO   1
#if(WK_STRING_BUILD_CONFIGS_IO)
// Cin Reset 
#define WK_STRING_BUILD_CONFIGS_IO_CINRESET    1
#endif

/* -----------------------------------------------------------------------------------------------------------------
Control Unit.
----------------------------------------------------------------------------------------------------------------- */
#define MC_A  || WK_STRING_BUILD_CONFIG_ENABLE_ALL
#define MC_S  || WK_STRING_BUILD_CONFIG_ENABLE_SUP

#define WK_STRING_BUILD_CONFIG_DEBUG WK_STRING_BUILD_CONFIGS_DEBUG MC_A
#define WK_STRING_BUILD_CONFIG_BASE WK_STRING_BUILD_CONFIGS_BASE MC_A
#define WK_STRING_BUILD_CONFIG_STRING WK_STRING_BUILD_CONFIGS_STRING MC_A MC_S
#define WK_STRING_BUILD_CONFIG_CSTRING WK_STRING_BUILD_CONFIGS_CSTRING MC_A MC_S

#define WK_STRING_BUILD_CONFIG_IO WK_STRING_BUILD_CONFIGS_IO MC_A MC_S
#if(WK_STRING_BUILD_CONFIG_IO)
#if(WK_STRING_BUILD_CONFIGS_IO)
#define WK_STRING_BUILD_CONFIG_IO_CINRESET WK_STRING_BUILD_CONFIGS_IO_CINRESET MC_A
#else
#define WK_STRING_BUILD_CONFIG_IO_CINRESET 0 MC_A
#endif
#endif

#endif // ! _WK_STRING_BUILD_CONFIG_

class WK_String
{
	// WK_String Base
#if(WK_STRING_BUILD_CONFIG_BASE)
public:
	 WK_String();
	 WK_String(const WK_String& strin);
	 WK_String(const char *strin);

	~WK_String();

	void operator= (const char *strin);
	void operator= (const WK_String& strin);

  const char* const operator* (){ return m_str; };

	int GetLength() const { return m_len; };
	const char* ToChar() const;
	BOOL SetSize(unsigned int size);

  enum {
    WK_STRING_IGNORE_CAPTION = 0x00000001,
    WK_STRING_REPEAT_FIND    = 0x00000002,
  };

  WK_Bool FindStr(const char* srt , int& firstlocal, int& cnt, unsigned int cmd = 0) const;

private:
	char *m_str;
	int m_len;

	void CleanStr();

//friend:
	friend BOOL WK_CopyStr(WK_String& str ,const char* strin);
	friend BOOL WK_CopyStr(char* strout , const WK_String& str);

#endif

	//Debug Founctions
#if(WK_STRING_BUILD_CONFIG_DEBUG)
public:
	static BOOL WK_Debug();
#endif

	// String Support
#if(WK_STRING_BUILD_CONFIG_STRING)
public:
	WK_String(const std::string& strin);
	std::string ToString() const;
	void operator= (const std::string& strin);
private:

//friend:
friend BOOL WK_CopyStr(WK_String& str, const std::string strin);
friend BOOL WK_CopyStr(std::string& strout, const WK_String& str);
#endif

	// CString Support
#if(WK_STRING_BUILD_CONFIG_CSTRING)
public:
	WK_String(const CString& strin);
	CString ToCString() const;
	void operator= (const CString& strin);
private:

//friend:
friend BOOL WK_CopyStr(WK_String& str, const CString strin);
friend BOOL WK_CopyStr(CString& strout, const WK_String& str);

#if(WK_STRING_BUILD_CONFIG_DEBUG)
private:
	static BOOL CString_Debug();
#endif //WK_STRING_BUILD_CONFIG_DEBUG
#endif
	
	// IO Stearm Support
#if(WK_STRING_BUILD_CONFIG_IO)
public:
	BOOL IO_Print();
private:

//friend:
	friend std::ostream& operator<< (std::ostream& cout , const WK_String& str);
	friend void operator>> (std::istream& cin, WK_String& str);

#endif

};
