#pragma once
#include <chrono>
namespace util {
class TimeMarker {
public:
  TimeMarker();
  ~TimeMarker() = default;
  float Mark();
  float Peek() const;

private:
  std::chrono::steady_clock::time_point m_recorded_time;
};

} // namespace util
