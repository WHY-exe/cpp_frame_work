#include "app.h"

#include "common/exception.h"
#include "spdlog/spdlog.h"
#include <vector>
namespace prj_exec1 {
App::App() {}

App::~App() noexcept {}

void App::Run() {
  std::vector<int> v;
  // spdlog::info("{}", v.at(5));
  SPDLOG_INFO("hello world");
  THROW_EXCEPTION("Hello exception", "basic");
}

} // namespace prj_exec1