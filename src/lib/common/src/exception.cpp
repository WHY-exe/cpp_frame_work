#include "exception.h"

#include <filesystem>
#include <sstream>

#include "spdlog/fmt/fmt.h"
#ifdef WINDOWS
#include <Windows.h>
#endif  // WINDOWS

namespace util {
namespace exception {
BasicInfo::BasicInfo(uint32_t line, std::string&& file, std::string&& func,
                     std::string&& msg, std::string&& type)
    : _line(line),
      _file(std::filesystem::path(file).filename().string()),
      _func(std::move(func)),
      _msg(std::move(msg)),
      _type(std::move(type)) {}

std::string BasicInfo::GenBasicInfo() const noexcept {
  return fmt::format("[{}][{}:{}][{}]{}", _type, _file, _line, _func, _msg);
}

Basic::Basic(uint32_t line, std::string&& file, std::string&& func,
             std::string&& msg, std::string&& type)
    : m_info(line, std::move(file), std::move(func), std::move(msg),
             std::move(type)) {}

const char* Basic::what() const noexcept {
  m_error_str = m_info.GenBasicInfo();
  return m_error_str.c_str();
}

#ifdef WINDOWS
std::string util::exception::FormatWin32Error(uint32_t error_num) noexcept {
  std::string err("");
  if (error_num == 0) error_num = GetLastError();
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
#endif  // WINDOWS

}  // namespace exception

}  // namespace util
