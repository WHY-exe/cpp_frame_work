#pragma once
#include "util.h"
#include <cerrno>
#include <cstdint>
#include <exception>

namespace util {
class Exception : public std::exception {
private:
  char m_what_buffer[MAX_BUFFER_SIZE];

public:
  Exception(uint32_t line, const char *func, const char *msg,
            const char *type) noexcept;
  const char *what() const noexcept override;
};

} // namespace util

#define THROW_EXCEPTION(error_msg, type)                                       \
  throw util::Exception(__LINE__, __FUNCTION__, error_msg, type)

// #ifdef LINUX
#define THROW_POSIX_ERROR                                                      \
  throw std::system_error(std::error_code(errno, std::system_category()))
// #endif

#ifdef WIN32
#define THROW_WIN_ERROR                                                        \
  throw std::system_error(                                                     \
      std::error_code(GetLastError(), std::system_category()))

#endif // WIN32
