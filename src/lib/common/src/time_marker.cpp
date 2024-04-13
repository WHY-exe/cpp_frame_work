#include "time_marker.h"
using namespace std::chrono;
namespace util {
TimeMarker::TimeMarker() : m_recorded_time(steady_clock::now()) {}

float TimeMarker::Mark() {
  const auto last_recorded_time = m_recorded_time;
  m_recorded_time = steady_clock::now();
  const duration<float> passing_time = m_recorded_time - last_recorded_time;
  return passing_time.count();
}

float TimeMarker::Peek() const {
  return duration<float>(steady_clock::now() - m_recorded_time).count();
}

} // namespace util
