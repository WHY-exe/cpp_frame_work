#include "dump_helper.h"
#include "spdlog/spdlog.h"
#include "util.h"
#include <cstdint>
#include <cstdio>

#ifdef WIN32
#include <DbgHelp.h>
#include <corecrt_io.h>
#include <ctime>
#include <direct.h>

#pragma comment(lib, "Dbghelp.lib")
#endif

namespace util {
namespace dmp_helper {
#ifdef WIN32
bool GenFilePath(char *buf, uint32_t path_len) noexcept {
  time_t t;
  struct tm local;
  t = time(NULL);
  localtime_s(&local, &t);

  if (_access(".\\dmp", 00) == -1) {
    _mkdir(".\\dmp");
  }

  // generate file name string
  sprintf_s(buf, path_len, ".\\dmp\\%d%02d%02d%02d%02d%02d.dmp",
            1900 + local.tm_year, local.tm_mon + 1, local.tm_mday,
            local.tm_hour, local.tm_min, local.tm_sec);
  if (strlen(buf) > path_len) {
    SPDLOG_ERROR("File path was too long! {}", buf);
    return false;
  }
  return true;
}

int GenDump(LPEXCEPTION_POINTERS excep_ptr, const char *file_path) noexcept {
  HANDLE hFile = ::CreateFileA(file_path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                               FILE_ATTRIBUTE_NORMAL, NULL);
  if (INVALID_HANDLE_VALUE != hFile) {
    MINIDUMP_EXCEPTION_INFORMATION minidumpExceptionInformation;
    minidumpExceptionInformation.ThreadId = GetCurrentThreadId();
    minidumpExceptionInformation.ExceptionPointers = excep_ptr;
    minidumpExceptionInformation.ClientPointers = TRUE;
    bool isMiniDumpGenerated =
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile,
                          MINIDUMP_TYPE::MiniDumpNormal,
                          &minidumpExceptionInformation, nullptr, nullptr);
    CloseHandle(hFile);
    if (!isMiniDumpGenerated) {
      SPDLOG_ERROR("MiniDumpWriteDump failed");
    }
  } else {
    SPDLOG_ERROR("Failed to create dump file");
  }
  return EXCEPTION_EXECUTE_HANDLER;
}

void SnapshotMem() noexcept {
  char file_path[MAX_PATH_LEN];
  if (!GenFilePath(file_path, MAX_PATH_LEN)) {
    return;
  };
  __try {
    // 通过触发异常获取堆栈
    RaiseException(0xE0000001, 0, 0, 0);
  } __except (GenDump(GetExceptionInformation(), file_path)) {
  }
}

LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo) noexcept {
  char file_path[MAX_PATH_LEN];
  if (!GenFilePath(file_path, MAX_PATH_LEN)) {
    return EXCEPTION_EXECUTE_HANDLER;
  };
  // generate dump file
  return GenDump(lpExceptionInfo, file_path);
}
#endif

#ifdef linux
void PrepareCore() noexcept {
  rlimit rlim{0};
  if (prctl(PR_SET_DUMPABLE, 1) < 0) {
    LOG_ERROR_COUT("{}", strerror(errno));
    return;
  }
  rlim.rlim_cur = RLIM_INFINITY;
  rlim.rlim_max = RLIM_INFINITY;
  setrlimit(RLIMIT_CORE, &rlim);
}
#endif

} // namespace dmp_helper
} // namespace util
