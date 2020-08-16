#include "error.h"

#if defined(_WIN32)
#include <windows.h>
#endif

std::string mps::GetErrorMessage() {
  #if defined(_WIN32)
  LPTSTR errorText = nullptr;
  FormatMessage(
    FORMAT_MESSAGE_FROM_SYSTEM
    | FORMAT_MESSAGE_ALLOCATE_BUFFER
    | FORMAT_MESSAGE_IGNORE_INSERTS,
    nullptr,
    WSAGetLastError(),
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPSTR)&errorText,
    0,
    nullptr);
  if (errorText != nullptr) {
    auto result = std::string(errorText);
    LocalFree(errorText);
    return result;
  }
  #endif
  return "Unknown error";
}
