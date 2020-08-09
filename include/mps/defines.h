#ifndef MPS_DEFINES_H
#define MPS_DEFINES_H

// the set of supported socket API types
#define MPS_SOCKET_API_WSA  1 // windows & xbox one
#define MPS_SOCKET_API_UNIX 2 // unix based systems

#if defined(_WIN32)
#define MPS_SOCKET_API MPS_SOCKET_API_WSA
#include <winsock2.h>
#define MPS_SOCKET_HANDLE  SOCKET
#define MPS_INVALID_HANDLE INVALID_SOCKET
#else
#define MPS_SOCKET_API MPS_SOCKET_API_UNIX
#define MPS_SOCKET_HANDLE  int
#define MPS_INVALID_HANDLE -1
#endif

#endif
