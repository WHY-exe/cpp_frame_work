#include "app.h"

#include "common/config_file.h"
#include "common/exception.h"
#include "common/util.h"
#include "spdlog/spdlog.h"
namespace prj_exec1 {
App::App() {}

App::~App() {}

void App::Run() { spdlog::info("hello world"); }

}  // namespace prj_exec1