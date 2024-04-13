#include "app.h"

#include "common/exception.h"
#include "spdlog/spdlog.h"
namespace prj_exec1 {
App::App() {}

App::~App() {}

void App::Run() {
  spdlog::info("hello world");
  THROW_EXCEPTION("Hello exception", "basic");
}

} // namespace prj_exec1