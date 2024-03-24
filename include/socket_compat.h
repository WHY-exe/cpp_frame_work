#pragma once
#include <optional>

#include "common/exception.h"
#include "posix_compat.h"
#ifdef LINUX
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#define closesocket close
#define SOCKET int  // Linux下套接字类型为int
constexpr auto SOCKET_ERROR = -1;
constexpr auto INVALID_SOCKET = -1;

#define THROW_SOCKET_ERROR THROW_SYSTEM_ERROR
#endif

#ifdef WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define THROW_SOCKET_ERROR                                           \
  throw std::system_error(WSAGetLastError(), std::system_category(), \
                          GET_BASIC_INFO("scoket error"))

inline WSADATA Win32InitSocket() {
  WSADATA socket_impl_data{0};
  int ret = WSAStartup(MAKEWORD(2, 2), &socket_impl_data);
  if (ret != 0) {
    std::stringstream ss;
    ss << "wsa init failed: " << ret;
    THROW_BASIC_EXCEPTION(ss.str());
  }
  return socket_impl_data;
}

#endif  // WINDOWS
