#pragma once
#include <cerrno>
#include <cstdint>
#include <exception>
#include <string>
#include <system_error>

namespace util {
namespace exception {
struct BasicInfo {
  uint32_t _line;
  std::string _file;
  std::string _func;
  std::string _msg;
  std::string _type;
  BasicInfo(uint32_t line, std::string &&file, std::string &&func,
            std::string &&msg, std::string &&type);
  ~BasicInfo() = default;
  BasicInfo(const BasicInfo &lv_rhs) = default;
  BasicInfo &operator=(const BasicInfo &lv_rhs) = default;
  BasicInfo(BasicInfo &&rv_rhs) noexcept = default;
  BasicInfo &operator=(BasicInfo &&rv_rhs) noexcept = default;
  std::string GenBasicInfo() const noexcept;
};

class Basic : public std::exception {
private:
  BasicInfo m_info;
  mutable std::string m_error_str;

public:
  Basic(uint32_t line, std::string &&file, std::string &&func,
        std::string &&msg, std::string &&type);
  Basic(const Basic &) = default;
  Basic &operator=(const Basic &) = default;
  Basic(Basic &&) = default;
  Basic &operator=(Basic &&) = default;
  ~Basic() override = default;
  const char *what() const noexcept override;
};

} // namespace exception
} // namespace util

#define GET_BASIC_INFO(error_type)                                             \
  util::exception::BasicInfo(__LINE__, __FILE__, __func__, "", error_type)     \
      .GenBasicInfo()

#define THROW_EXCEPTION(error_msg, type)                                       \
  throw util::exception::Basic(__LINE__, __FILE__, __func__, error_msg, type)

// #ifdef LINUX
#define THROW_POSIX_ERROR                                                      \
  throw std::system_error(std::error_code(errno, std::system_category()),      \
                          GET_BASIC_INFO("system error"))
// #endif

#ifdef WIN32
#define THROW_WIN_ERROR                                                        \
  throw std::system_error(                                                     \
      std::error_code(GetLastError(), std::system_category()),                 \
      GET_BASIC_INFO("win32 error"))

#endif // WINDOWS
