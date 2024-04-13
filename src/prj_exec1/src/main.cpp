#include "app.h"
#include "common/exception.h"
#include "signal_handle.h"
#include "spdlog/spdlog.h"

int main() {
  prj_exec1::HandleSignal();
  try {
    prj_exec1::App{}.Run();
  } catch (const util::exception::Basic &e) {
    spdlog::error("{}", e.what());
  } catch (const std::system_error &e) {
    spdlog::error("{} Code: {}", e.what(), e.code().value());
  } catch (const std::logic_error &e) {
    spdlog::error("{}", e.what());
  } catch (const std::exception &e) {
    spdlog::error("{}", e.what());
  } catch (...) {
    spdlog::error("An unknown error occoured while app is running ...");
  }
  return 0;
}
