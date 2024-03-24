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
  // std::stringstream ss;
  // ss << "[" << _type << "] " << _file << "(" << _func << ":" << _line << ") "
  // << _msg; return ss.str();
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
  if (0 ==
      FormatMessage(
          FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
              FORMAT_MESSAGE_IGNORE_INSERTS,  // ��־λ���������˵��lpSource������dwFlags�ĵ�λָ����δ������й����������������Ҳ���������ȵĸ�ʽ�������,��ѡ������
          NULL,       // ����dwFlags��־������
          error_num,  // �������Ϣ�ı�ʶ������dwFlags��־ΪFORMAT_MESSAGE_FROM_STRINGʱ�ᱻ���ԡ�
          MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  // �������Ϣ�����Ա�ʶ����
          (LPTSTR)&lpBuffer,  // ���մ�����Ϣ�����Ļ�����ָ�롣
          0,  // ���FORMAT_MESSAGE_ALLOCATE_BUFFER��־û�б�ָ���������������ָ��Ϊ����������Ĵ�С�����ָ��ֵΪ0���������ָ��Ϊ������������������С����
          NULL  // �����ʽ����Ϣ�еĲ���ֵ��һ�����顣
          )) {  // ʧ��
    char tmp[100] = {0};
    sprintf_s(tmp, "undefine error description(%d)", error_num);
    err = tmp;
  } else  // �ɹ�
  {
    err = lpBuffer;
  }
  return err;
}
#endif  // WINDOWS

}  // namespace exception

}  // namespace util
