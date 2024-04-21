#include "app.h"
#include "common/exception.h"
#include "common/log.h"
#include <csignal>
#include <stdlib.h>

#include "common/dump_helper.h"

#ifdef WIN32
void HandleQuitSignal(int signal) noexcept {
  switch (signal) {
  case SIGSEGV:
    LOG_ERROR_COUT("segment fault, there might be problems in code");
    break;
  case SIGABRT:
    LOG_ERROR_COUT("signal abort, there might be problems in code");
    break;
  case SIGFPE:
    LOG_ERROR_COUT("floating point exception");
    break;
  case SIGILL:
    LOG_ERROR_COUT("illegal instruction, there might be problems in code");
    break;
  case SIGINT:
    LOG_WARN_COUT("signal keyboard interrupt, quiting...");
    break;
  case SIGTERM:
    LOG_WARN_COUT("signal terminate, quiting...");
    break;
  default:
    LOG_ERROR_COUT("recieving signal %d, stopping the instance", signal);
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
  try {
    prj_exec1::App{}.Run();
  } catch (const util::exception::Basic &e) {
    LOG_ERROR_COUT("custom exception: {}", e.what());
    abort();
  } catch (const std::system_error &e) {
    LOG_ERROR_COUT("system error: {} Code: {}", e.what(), e.code().value());
    abort();
  } catch (const std::logic_error &e) {
    LOG_ERROR_COUT("logic error: {}", e.what());
    abort();
  } catch (const std::exception &e) {
    LOG_ERROR_COUT("common exception: {}", e.what());
    abort();
  } catch (...) {
    LOG_ERROR_COUT("An unknown error occoured while app is running ...");
    abort();
  }
  return 0;
}
