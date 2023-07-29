#include <windows.h>
#include "CServerCore.h"

int main()
{
  CServerCore::GetInstance()->Init();
  CServerCore::GetInstance()->StratServer();
  while (true) {
    Sleep(1000);
  }
}


