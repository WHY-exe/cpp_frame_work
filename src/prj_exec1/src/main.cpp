#include "app.h"
#include "common/exception.h"
#include "common/log_formatter.h"
#include <csignal>
#include <memory>
#include <stdlib.h>

#include "common/dump_helper.h"
#include "spdlog/spdlog.h"

#ifdef WIN32
void HandleQuitSignal(int signal) noexcept {
  switch (signal) {
  case SIGSEGV:
    SPDLOG_ERROR("segment fault, there might be problems in code");
    break;
  case SIGABRT:
    SPDLOG_ERROR("signal abort, there might be problems in code");
    break;
  case SIGFPE:
    SPDLOG_ERROR("floating point exception");
    break;
  case SIGILL:
    SPDLOG_WARN("illegal instruction, there might be problems in code");
    break;
  case SIGINT:
    SPDLOG_WARN("signal keyboard interrupt, quiting...");
    break;
  case SIGTERM:
    SPDLOG_WARN("signal terminate, quiting...");
    break;
  default:
    SPDLOG_WARN("recieving signal %d, stopping the instance", signal);
    break;
  }
  util::dmp_helper::SnapshotMem();
  exit(1);
}
#endif

int main() {
#ifdef WIN32
  SetUnhandledExceptionFilter(util::dmp_helper::ExceptionFilter);
#endif
#ifdef linux
  // for linux generating might still need to do
  // bash -c "echo core > /proc/sys/kernel/core_pattern"
  util::dmp_helper::PrepareCore();
#endif
#ifdef unix
#define HandleQuitSignal SIG_DFL
  signal(SIGPIPE, SIG_IGN);
  signal(SIGBUS, HandleQuitSignal);
#endif
  signal(SIGSEGV, HandleQuitSignal);
  signal(SIGABRT, HandleQuitSignal);
  signal(SIGFPE, HandleQuitSignal);
  signal(SIGINT, HandleQuitSignal);
  signal(SIGTERM, HandleQuitSignal);
  signal(SIGILL, HandleQuitSignal);
  // spdlog formatter setup: modual, fuction, and line
  auto formatter = std::make_unique<spdlog::pattern_formatter>();
  formatter->add_flag<util::log::ModuleFlagFormatter>('.').set_pattern(
      "[%Y-%m-%d %H:%M:%S.%e][%.][%!:%#][tid %t][%l] %v");
  spdlog::set_formatter(std::move(formatter));
  try {
    prj_exec1::App{}.Run();
  } catch (const util::Exception &e) {
    SPDLOG_ERROR("custom exception: {}", e.what());
  } catch (const std::system_error &e) {
    SPDLOG_ERROR("system error: {} Code: {}", e.what(), e.code().value());
  } catch (const std::logic_error &e) {
    SPDLOG_ERROR("logic error: {}", e.what());
  } catch (const std::exception &e) {
    SPDLOG_ERROR("common exception: {}", e.what());
  } catch (...) {
    SPDLOG_ERROR("An unknown error occoured while app is running ...");
  }
  return 0;
}
