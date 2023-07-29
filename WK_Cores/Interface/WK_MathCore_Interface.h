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
#ifdef  WK_MATH_DLLEXPORT
#define WK_MathCore_API extern "C" _declspec(dllexport)
#else
#define WK_MathCore_API extern "C" _declspec(dllimport)
#endif //  WK_MATHCORE_DLLEXPORT

class WK_MathClass{
public:
  virtual int ShowMathCore() = 0;
};

WK_MathCore_API WK_MathClass* WK_MathCore_GetMathClass();