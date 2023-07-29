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
#define  WK_LOG_DLLEXPORT
#include <windows.h>
#include "WK_LogCore_Interface.h"
#include "CWKLogCore.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
      break;
    }
    case DLL_THREAD_ATTACH:
    {
      break;
    }
    case DLL_THREAD_DETACH: 
    {
      CWKLogCore::GetInstance()->OnRelease();
      break;
    }
    case DLL_PROCESS_DETACH:
    {
      CWKLogCore::GetInstance()->OnRelease();
      break;
    }
    default: break;
    }
    return TRUE;
}

WK_LogCore_API void WK_LogCore_WritingLog(const char* log_type, const char* str_log) {
  CWKLogCore::GetInstance()->WritingLog(log_type, str_log);
}

