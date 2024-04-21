#include "app.h"

#include "common/exception.h"
#include "spdlog/spdlog.h"
#include <vector>
namespace prj_exec1 {
App::App() {}

App::~App() {}

void App::Run() {
  std::vector<int> v;
  spdlog::info("{}", v.at(5));
  spdlog::info("hello world");
  THROW_EXCEPTION("Hello exception", "basic");
}

} // namespace prj_exec1