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
#define WK_ENCRYPT_DLLEXPORT
#include "WK_EncryptCore_Interface.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

WK_EncryptCore_API bool WK_EncryptCore_Encrypt(byte* data, unsigned int len) {
  if (len == 0)
    return true;

  for (int index = 0; index < len; index++) {
    int buff = *data;
    buff -= (2*len + 3*index);
    *data = ~buff;
    data++;
  }

  return true;
}

WK_EncryptCore_API bool WK_EncryptCore_Decrypt(byte* data, unsigned int len) {
  if (len == 0)
    return true;

  for (int index = 0; index < len; index++) {
    int buff = *data;
    buff = ~buff;
    buff += (2*len + 3*index);
    *data = buff;
    data++;
  }

  return true;
}
