#include "signal_handle.h"

#include <csignal>

#include "spdlog/spdlog.h"
#ifdef LINUX
#include <sys/prctl.h>
#include <sys/resource.h>
#endif // LINUX
namespace prj_exec1 {
void HandleQuitSignal(int signal) noexcept {
  SignalHandler sh{};
  sh.Prepare();
  switch (signal) {
  case SIGSEGV:
    spdlog::error("segment fault, there might be problems in code");
    break;
  case SIGABRT:
    spdlog::error("signal abort, there might be problems in code");
    break;
  case SIGFPE:
    spdlog::error("floating point exception");
    break;
#ifdef LINUX
  case SIGBUS:
    spdlog::error("illegal memory access, may be memory alignment error");
    break;
#endif
  case SIGILL:
    spdlog::error("illegal instruction, there might be problems in code");
    break;
  case SIGINT:
    spdlog::warn("signal keyboard interrupt, quiting...");
    break;
  case SIGTERM:
    spdlog::warn("signal terminate, quiting...");
    break;
  default:
    spdlog::error("recieving signal %d, stopping the instance", signal);
    break;
  }
  sh.GenCoreDump();
  exit(1);
}

void HandleSignal() noexcept {
  // 段错�?信号，可能由对内存的非法访问引起
  signal(SIGSEGV, HandleQuitSignal);
  // abort信号，可以由assert或abort函数引起
  signal(SIGABRT, HandleQuitSignal);
  // 除零信号，由对浮点数的非法操作引�?
  // 比�?�某一�?数除�?0
  signal(SIGFPE, HandleQuitSignal);
  // 用户�?�?�?信号，一�?来�?�是Ctrl+C
  signal(SIGINT, HandleQuitSignal);
  // 由kill(1)命令发送的终�??信号，�?�有全局变量
  // �?以在收到这个信号时释放资�?
  signal(SIGTERM, HandleQuitSignal);
#ifdef LINUX
  signal(SIGPIPE, SIG_IGN);
  // 非法的内存�?�取，包�?读取到了不�?�齐的内�?
  signal(SIGBUS, HandleQuitSignal);
#endif
  // 进程执�?�了非法�?件指�?
  signal(SIGILL, HandleQuitSignal);
}

SignalHandler::SignalHandler() noexcept : m_core_size(0) {}

void SignalHandler::Prepare() noexcept {
#ifdef LINUX
  rlimit rlim{0};
  if (prctl(PR_SET_DUMPABLE, 1) < 0) {
    LOG_SYSTEM_ERROR_COUT;
    return;
  }
  if (m_core_size == 0) {
    m_core_size = RLIM_INFINITY;
  }
  rlim.rlim_cur = m_core_size;
  rlim.rlim_max = m_core_size;
  setrlimit(RLIMIT_CORE, &rlim);
#endif
}

void SignalHandler::GenCoreDump() noexcept {
#ifdef LINUX
  system(fmt::format("gcore {}", getpid(), getpid()).c_str());
#endif
}

} // namespace prj_exec1
