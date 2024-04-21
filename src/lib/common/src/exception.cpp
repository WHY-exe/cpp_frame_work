#include "exception.h"
#include "spdlog/fmt/bundled/core.h"
#include <filesystem>
namespace util {
namespace exception {
BasicInfo::BasicInfo(uint32_t line, std::string &&file, std::string &&func,
                     std::string &&msg, std::string &&type)
    : _line(line), _file(std::move(file)), _func(std::move(func)),
      _msg(std::move(msg)), _type(std::move(type)) {}

std::string BasicInfo::GenBasicInfo() const noexcept {
  namespace fs = std::filesystem;
  fs::path file(_file);
  const std::string module_name = (--(--(--file.end())))->string();
  return fmt::format("[{}][{}][{}:{}][{}]{}", _type, module_name,
                     file.filename().string(), _line, _func, _msg);
}

Basic::Basic(uint32_t line, std::string &&file, std::string &&func,
             std::string &&msg, std::string &&type)
    : m_info(line, std::move(file), std::move(func), std::move(msg),
             std::move(type)) {}

const char *Basic::what() const noexcept {
  m_error_str = m_info.GenBasicInfo();
  return m_error_str.c_str();
}

} // namespace exception

} // namespace util
