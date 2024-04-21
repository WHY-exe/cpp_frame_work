#include "log.h"

#include <filesystem>
#ifdef WIN32
#include <Windows.h>
#endif
namespace util {
namespace log {
std::string GetModuleName(std::string_view file_path) {
  std::filesystem::path path(file_path);
  return (--(--(--path.end())))->string();
}

std::string GetFileName(std::string_view file_path) {
  return std::filesystem::path(file_path).filename().string();
}

#ifdef WIN32
std::string FormatWin32Error(uint32_t error_num) noexcept {
  std::string err("");
  if (error_num == 0)
    error_num = GetLastError();
  LPTSTR lpBuffer = NULL;
  if (0 == FormatMessage(
               FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                   FORMAT_MESSAGE_IGNORE_INSERTS,
               NULL, error_num, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
               (LPTSTR)&lpBuffer, 0, NULL)) {
    char tmp[100] = {0};
    sprintf_s(tmp, "undefine error description(%d)", error_num);
    err = tmp;
  } else {
    err = lpBuffer;
  }
  return err;
}
#endif // WIN32
} // namespace log
} // namespace util
