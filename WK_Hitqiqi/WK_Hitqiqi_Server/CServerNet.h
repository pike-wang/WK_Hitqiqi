#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<winsock.h>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

#include "WK_Hitqiqi_def.h"
#include "WK_Common_Define.h"

#pragma comment(lib,"ws2_32.lib")

class CServerNet
{
public:
  ~CServerNet() { closesocket(main_listen_socket); WSACleanup(); }

  WK_Bool Init();
  WK_Bool BeginTCPListen();
  WK_Bool RecvClientMsg(SOCKET* s_accept);
  WK_Bool SendClientMsg(HITQIQIMSG_PACKAGE *send_package, void *s_accept = nullptr, WK_Bool exclusion = WK_False);
  WK_Bool DeleteSocket(SOCKET* s_accept);
  WK_Bool AddCommandSocket(void* socket);
  int GetConnectCnt();

private:

  WK_Bool IsCommandSocket(SOCKET* socket);

  SOCKET main_listen_socket;

  std::mutex socket_pool_lock;
  std::vector<SOCKET*> m_socketpool;

  std::mutex command_socket_pool_lock;
  std::vector<SOCKET*> m_command_socketpool;
};

