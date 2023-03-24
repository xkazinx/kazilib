#pragma once

namespace kazin::time
{
	using Value = u32;

	void Tick();
	Value Now();
	Value Delta();
	float DeltaFloat();

#ifdef KAZIN_WINDOWS
	[[nodiscard]] constexpr auto operator ""_ms(u64 val) noexcept { return val; }
	[[nodiscard]] constexpr auto operator  ""_s(u64 val) noexcept { return val * CLOCKS_PER_SEC; }
	[[nodiscard]] constexpr auto operator  ""_m(u64 val) noexcept { return val * CLOCKS_PER_SEC * 60; }
	[[nodiscard]] constexpr auto operator  ""_h(u64 val) noexcept { return val * CLOCKS_PER_SEC * 60 * 60; }
	[[nodiscard]] constexpr auto operator  ""_d(u64 val) noexcept { return val * CLOCKS_PER_SEC * 60 * 60 * 24; }
#endif
}

