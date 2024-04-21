#pragma once
#include <chrono>
namespace util {
class TimeMarker {
public:
  TimeMarker() noexcept;
  ~TimeMarker() noexcept = default;
  float Mark() noexcept(std::is_arithmetic_v<float>);
  float Peek() const;

private:
  std::chrono::steady_clock::time_point m_recorded_time;
};

} // namespace util
