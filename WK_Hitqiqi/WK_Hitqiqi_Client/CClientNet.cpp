#include "CClientNet.h"

WK_Bool CClientNet::Init() {

  WSADATA ws;
  WSAStartup(MAKEWORD(2, 2), &ws);

  m_socket = socket(AF_INET, SOCK_STREAM, 0);

  return WK_True;
}

WK_Bool CClientNet::ReConnectServer() {

  int nIpCount = 0;
  char szIpList[50][100];
  char szDomain[256] = { 0 };
  char szIp[1024] = { 0 };
  strcpy_s(szDomain, HITQIQI_SERVER_DOMAIN);
  GetRealIpByDomainName(szDomain, szIpList, &nIpCount);

  for (int i = 0; i < nIpCount; i++) {
    strcat_s(szIp, szIpList[i]);
  }

  if (szIp[0] == 0)
    return WK_False;

  SOCKADDR_IN server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.S_un.S_addr = inet_addr(szIp);
  server_addr.sin_port = htons(HITQIQI_SERVER_PORT);
  //server_addr.sin_port = htons(HITQIQI_SERVER_PORT_TEST);

  if (SOCKET_ERROR != connect(m_socket, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)))
    return WK_True;

  return WK_False;
}

int CClientNet::RecvServerMsg(HITQIQIMSG_PACKAGE *recv_package) {
  int recv_len = 0;
  int ReConnectCnt = HITQIQ_MAX_RECONNECT;
  char recv_buf[HITQIQ_MAX_BUF] = { 0 };
  WK_Bool IsConnected = WK_True;
  while (ReConnectCnt>0)
  {
    if (IsConnected)
    {
      memset(recv_buf, 0, HITQIQ_MAX_BUF);
      recv_len = recv(m_socket, recv_buf, HITQIQIMSG_PACKAGE_LEN, 0);
      if (recv_len > 0) {
        ReConnectCnt = HITQIQ_MAX_RECONNECT;
        memcpy(recv_package, recv_buf, HITQIQIMSG_PACKAGE_LEN);
        return recv_len;
      }
    }

    IsConnected = WK_False;

    Sleep(10);
    closesocket(m_socket);
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (!ReConnectServer()) {
      ReConnectCnt--;
      continue;;
    }
    IsConnected = WK_True;
  }
  return recv_len;
}

int CClientNet::SendServerMsg(char* send_buf){

  int send_len = 0;
  int ReConnectCnt = HITQIQ_MAX_RECONNECT;
  while (ReConnectCnt > 0)
  {
    send_len = send(m_socket, send_buf, HITQIQIMSG_PACKAGE_LEN, 0);
    if (send_len > 0) {
      ReConnectCnt = HITQIQ_MAX_RECONNECT;
      return send_len;
    }

    Sleep(500);
    closesocket(m_socket);
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    ReConnectServer();
    ReConnectCnt--;
  }
  return send_len;
}

// WK_Bool CClientNet::IsConnect() {
//   int ReConnectCnt = HITQIQ_MAX_RECONNECT;
//   while (ReConnectCnt > 0)
//   {
//     if (TestConnect())
//       return WK_True;
// 
//     closesocket(m_socket);
//     m_socket = socket(AF_INET, SOCK_STREAM, 0);
//     ReConnectServer();
// 
//     ReConnectCnt--;
// 
//     Sleep(500);
//   }
//   return WK_False;
// }
// 
// WK_Bool CClientNet::TestConnect() {
//   char buffer[1];
//   recv(m_socket, buffer, 1, MSG_PEEK);
//   return (WSAECONNRESET != WSAGetLastError());
// }

BOOL  CClientNet::GetRealIpByDomainName(char* szHost, char szIp[50][100], int* nCount)
{
  HOSTENT* pHostEnt;
  int nAdapter = 0;
  struct sockaddr_in   sAddr;

  pHostEnt = gethostbyname(szHost);
  if (pHostEnt)
  {
    while (pHostEnt->h_addr_list[nAdapter])
    {
      memcpy(&sAddr.sin_addr.s_addr, pHostEnt->h_addr_list[nAdapter], pHostEnt->h_length);

      sprintf_s(szIp[nAdapter], "%s", inet_ntoa(sAddr.sin_addr));

      nAdapter++;
    }

    *nCount = nAdapter;
  }
  else
  {
    DWORD  dwError = GetLastError();
    *nCount = 0;
  }

  return TRUE;
}
