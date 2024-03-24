#pragma once
#include <cstdint>
namespace prj_exec1
{
class SignalHandler
{
private:
	uint64_t m_core_size;
public:
	SignalHandler() noexcept;
	~SignalHandler() noexcept = default;
	void Prepare() noexcept;
};


void HandleQuitSignal(int signal) noexcept;
void HandleSignal() noexcept;   
} // namespace prj_exec1
