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

#include "../WK_Common_Define.h"

//get max number from two int
int WK_GetMax(int a, int b) {
  return a > b ? a : b;
}

//get the factorial value
int WK_GetFactorial(int n) {
  return n <= 1 ? 1 : n * WK_GetFactorial(n - 1);
}