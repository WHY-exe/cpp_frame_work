#include <cstdint>
#include <filesystem>

#include "catch2/catch_test_macros.hpp"
#include "common/config_file.h"
#include "spdlog/spdlog.h"

uint32_t factorial(uint32_t number) {
  return number <= 1 ? number : factorial(number - 1) * number;
}

TEST_CASE("Factorials are computed", "[factorial]") {
  REQUIRE(factorial(1) == 1);
  REQUIRE(factorial(2) == 2);
  REQUIRE(factorial(3) == 6);
  REQUIRE(factorial(10) == 3'628'800);
}

TEST_CASE("Config file is read", "config_file") {
  util::Config test_cfg("./test_cfg.ini");
  test_cfg["test_section1"]["test_key1"] = "test_value1";
  REQUIRE(test_cfg["test_section1"]["test_key1"] == "test_value1");
  test_cfg.WriteFile();
  test_cfg.ReadConfig();
  REQUIRE(test_cfg["test_section1"]["test_key1"] == "test_value1");
}