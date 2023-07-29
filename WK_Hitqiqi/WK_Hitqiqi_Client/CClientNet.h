#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<winsock.h>
#include <tchar.h>

#include "WK_Common_Define.h"
#include "WK_Hitqiqi_def.h"

#pragma comment(lib,"ws2_32.lib")

class CClientNet
{
public:
  ~CClientNet() { closesocket(m_socket); WSACleanup();}

  int RecvServerMsg(HITQIQIMSG_PACKAGE *recv_package);
  int SendServerMsg(char* send_buf);
/*  WK_Bool IsConnect();*/

  WK_Bool Init();
  WK_Bool ReConnectServer();

private:
/*  WK_Bool TestConnect();*/
  BOOL GetRealIpByDomainName(char* szHost, char szIp[50][100], int* nCount);

  SOCKET m_socket;
};

