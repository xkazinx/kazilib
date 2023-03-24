#pragma once
#include <random>
#include "container.h"

#ifdef KAZIN_SFML
#include <SFML/Graphics.hpp>
#endif

namespace kazin
{
inline thread_local std::mt19937 engine_((unsigned int)::time(nullptr));
//thread_local std::random_device device_;

class Random
{
public:
	template<typename T>
	static T Generate(const T & from, const T & to)
	{
		if constexpr(std::is_floating_point_v<T>)
		{
			return std::uniform_real_distribution<T>(from, to)(engine_);
		}
		else
		{
			return std::uniform_int_distribution<T>(from, to)(engine_);
		}
	}

	// #todo adapt to all containers
	template<typename T> requires container::is_container<T>::value
	static auto & Gen(T & container)
	{
		return container[Generate<szt>(0, container.size() - 1)];
	}

	static bool Probability(const float num);

	// todo adapt library both to sfml and sdl

#ifdef KAZIN_SFML
	
	static sf::Color Color(u8 alpha = 255)
	{
		return {
			(u8)Generate<short>(0, 255),
			(u8)Generate<short>(0, 255),
			(u8)Generate<short>(0, 255),
			alpha
		};
	}
	
#endif
};
}
