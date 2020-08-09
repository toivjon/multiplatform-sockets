#ifndef MPS_WSA_H
#define MPS_WSA_H

#include "mps/defines.h"

#if MPS_SOCKET_API == MPS_SOCKET_API_WSA
namespace mps {
  class WSA final {
  public:
    WSA();
    WSA(const WSA& other) = delete;
    WSA(WSA&& other) = delete;

    WSA& operator=(const WSA& other) = delete;
    WSA& operator=(WSA&& other) = delete;

    ~WSA();
  };
}
#endif

#endif
