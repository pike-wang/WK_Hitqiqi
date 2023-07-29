#include "CServerNet.h"
#include "CServerCore.h"

using namespace std;

void Thread_Recv_ClientMsg(CServerNet* net, SOCKET* s_accept)
{
  net->RecvClientMsg(s_accept);
}

WK_Bool CServerNet::Init(){

  //socket_pool_lock.unlock();
  m_socketpool.clear();
  m_command_socketpool.clear();

  WORD w_req = MAKEWORD(2, 2);//版本号
  WSADATA wsadata;
  int err;
  err = WSAStartup(w_req, &wsadata);

  if (err != 0) {
    cout << "初始化套接字库失败！" << endl;
    return WK_False;
  }
  else {
    cout << "初始化套接字库成功！" << endl;
  }
  //检测版本号
  if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
    cout << "套接字库版本号不符！" << endl;
    WSACleanup();
    return WK_False;
  }
  else {
    cout << "套接字库版本正确！" << endl;
  }

  main_listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  SOCKADDR_IN addrSrv;
  addrSrv.sin_family = AF_INET;
  //addrSrv.sin_port = htons(HITQIQI_SERVER_PORT_TEST);
  addrSrv.sin_port = htons(HITQIQI_SERVER_PORT);
  addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

  int result = bind(main_listen_socket, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR_IN));


  return WK_True;
}

WK_Bool CServerNet::BeginTCPListen() {

  if (listen(main_listen_socket, SOMAXCONN) < 0)
  {
    cout << "设置监听状态失败！" << endl;
    WSACleanup();
  }
  else
  {
    cout << "设置监听状态成功！" << endl;
  }
  cout << "服务端正在监听连接，请稍候...." << endl;

  while (WK_True)
  {
    SOCKADDR_IN accept_addr;
    int len = sizeof(SOCKADDR_IN);
    SOCKET * s_accept = new SOCKET;
    *s_accept = accept(main_listen_socket, (SOCKADDR*)&accept_addr, &len);
    if (*s_accept == SOCKET_ERROR)
      continue;
    std::thread t = std::thread(Thread_Recv_ClientMsg, this, s_accept);
    t.detach();
    cout << "用户连接" << endl;
  }
  return WK_True;
}

WK_Bool CServerNet::RecvClientMsg(SOCKET* s_accept) {

  socket_pool_lock.lock();
  m_socketpool.push_back(s_accept);
  socket_pool_lock.unlock();

  int recv_len = 0;
  char recv_buf[HITQIQ_MAX_BUF] = { 0 };

  while (recv_len = recv(*s_accept, recv_buf, HITQIQIMSG_PACKAGE_LEN, 0) != -1)
  {
    HITQIQIMSG_PACKAGE recv_paceage;
    memcpy(&recv_paceage, recv_buf, HITQIQIMSG_PACKAGE_LEN);
    if (!CServerCore::GetInstance()->HandleClientMsg(&recv_paceage, s_accept))
      break;
    memset(recv_buf, 0, HITQIQ_MAX_BUF);
  }
    cout << "用户断线" << endl;
    closesocket(*s_accept);
    DeleteSocket(s_accept);

    if (IsCommandSocket(s_accept))
      CServerCore::GetInstance()->OnCommandOffline();

    delete s_accept;
    return WK_True;
}

WK_Bool CServerNet::DeleteSocket(SOCKET* s_accept) {

  socket_pool_lock.lock();
  if (m_socketpool.empty())
  {
    socket_pool_lock.unlock();
    return WK_True;
  }

  for (vector<SOCKET*>::const_iterator iter = m_socketpool.cbegin(); iter != m_socketpool.cend(); )
  {
    if (*iter == s_accept) {
      iter = m_socketpool.erase(iter);
    }
    else {
      ++iter;
    }
  }
  socket_pool_lock.unlock();
  return WK_True;
}

WK_Bool CServerNet::SendClientMsg(HITQIQIMSG_PACKAGE* send_package, void* s_accept, WK_Bool exclusion) {

  if (m_socketpool.empty())
    return WK_True;

  char send_buf[HITQIQ_MAX_BUF] = { 0 };
  memcpy(send_buf, send_package, HITQIQIMSG_PACKAGE_LEN);
if (s_accept != nullptr && !exclusion) {
 send(*(SOCKET*)s_accept, send_buf, HITQIQIMSG_PACKAGE_LEN, 0);
}
else {
  socket_pool_lock.lock();
  for (vector<SOCKET*>::const_iterator iter = m_socketpool.cbegin(); iter != m_socketpool.cend(); iter++ )
  {
    if (!exclusion || s_accept != *iter)
      send(**iter, send_buf, HITQIQIMSG_PACKAGE_LEN, 0);
  }
  socket_pool_lock.unlock();
}
  return WK_True;
}

WK_Bool CServerNet::AddCommandSocket(void* socket) {
  command_socket_pool_lock.lock();
  m_command_socketpool.push_back((SOCKET*)socket);
  command_socket_pool_lock.unlock();
  return WK_True;
}

WK_Bool CServerNet::IsCommandSocket(SOCKET* socket) {
  command_socket_pool_lock.lock();
  if (m_command_socketpool.empty()) {
    command_socket_pool_lock.unlock();
    return WK_False;
  }

  for (vector<SOCKET*>::const_iterator iter = m_command_socketpool.cbegin() ; iter!= m_command_socketpool.cend(); iter++)
  {
    if (*iter == socket) {
      m_command_socketpool.erase(iter);
      command_socket_pool_lock.unlock();
      return WK_True;
    }
  }
  command_socket_pool_lock.unlock();
  return WK_False;
}

int CServerNet::GetConnectCnt() {
  socket_pool_lock.lock();
  if (m_socketpool.empty())
  {
    socket_pool_lock.unlock();
    return 0;
  }
  int size = m_socketpool.size();
  socket_pool_lock.unlock();
  return size;
}