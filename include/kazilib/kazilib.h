#pragma once

#include "types.h"

#include "console/log.h"

#include "arg_parser.h"
#include "container.h"

#include "magic_enum.h"
using namespace magic_enum;

#include "net.h"
#include "os.h"

#pragma comment(lib, "winmm.lib")

#include "random.h"

#ifdef KAZIN_SDL
	#ifdef _DEBUG
		#pragma comment(lib, "SDL2.lib")
		#pragma comment(lib, "SDL2main.lib")
	#else
		#pragma comment(lib, "SDL2.lib")
		#pragma comment(lib, "SDL2main.lib")
	#endif

	//#define SDL_MAIN_HANDLED

	#ifdef KAZIN_SDL_IMGUI
		#include "imgui/imgui.h"
		#include "imgui/imgui_impl_sdl.h"
		#include "imgui/imgui_impl_sdlrenderer.h"
		//#include "imgui/imgui_impl_opengl3.h"
	#endif

	#include <SDL.h>
#endif

#if defined(KAZIN_SFML)
	#pragma comment(lib, "opengl32.lib")

	#include <SFML/Graphics.hpp>
	#include <SFML/OpenGL.hpp>
	
	#ifdef KAZIN_SFML_IMGUI
		#include "imgui/imgui.h"
		#include "imgui/imgui-SFML.h"
	#endif

	#include "scenes.h"
#endif

#if defined(KAZIN_SFML) || defined(KAZIN_SDL)
	#define KAZIN_GRAPHICS

	#if defined(KAZIN_SFML_IMGUI) || defined(KAZIN_SDL_IMGUI)
		#define KAZIN_IMGUI
	#endif
#endif

#if defined(KAZIN_NET)
	#define KAZIN_ENET
	#define _WINSOCKAPI_
	#define NOMINMAX

	#pragma comment(lib, "ws2_32.lib")
	#pragma comment(lib, "enet.lib")
	#include <enet/enet.h>
	
#if defined(CLIENT) || defined(SERVER)
	#include <netops.h> // Must create in project
	#include "packet.h"
	#include "socket.h"
	#include "broadcaster.h"
	#include "auth.h"
#endif
#endif

#ifdef KAZIN_WINDOWS

#include <windows.h>
#include <mmsystem.h>
#endif

#include "ntime.h"
#include "file.h"

#ifdef LIVEPP
	#include <LPP_API.h>
#endif

namespace kazin::global
{
	extern unique_ptr<Random> g_rand;
}

using namespace kazin::global;

namespace kazin
{
namespace app
{
//template<class T> requires is_same_v<T, bool>
class Base
{
public:
#ifdef LIVEPP
	HMODULE _livepp;

	virtual void PreLivePP() = 0;
	virtual void PostLivePP() = 0;

	// #todo
	//LPP_PREPATCH_HOOK(PreLivePP);
	//LPP_POSTPATCH_HOOK(PostLivePP);
#endif

public:
	Base(const std::string & app_name);
	virtual ~Base()
	{
		//global::Deinitialize();

#ifdef LIVEPP
		lpp::lppShutdown(_livePP);
		::FreeLibrary(_livePP);
#endif

#ifdef KAZIN_SFML_IMGUI
		ImGui::SFML::Shutdown();
#endif
	};

	ArgParser _args;
	
public:
#ifdef KAZIN_GRAPHICS
	u16 _res_x = 800;
	u16 _res_y = 600;

#ifdef KAZIN_GRAPHICS
	Base(const std::string& app_name, std::function<void(sf::RenderWindow& wnd)> init_wnd) :
		Base(app_name)
	{
		init_wnd(_window);

#ifdef KAZIN_SFML
		// https://www.sfml-dev.org/tutorials/2.5/window-opengl.php
		_window.setActive(true);
		_window.setVerticalSyncEnabled(true);

		AdjustViewport(_window.getSize().x, _window.getSize().y);
#endif

		// #todo #kazin set window title as app name, and set console app title as app name?
#ifdef KAZIN_SFML_IMGUI
		ImGui::SFML::Init(_window);
#endif
	}
#endif

	virtual int Tick()
	{
		/*sf::Clock clock;

		while(!_exit)
		{
			{
				if(!HandleEvents())
					break;
			}

			{
				_scenes.Tick();
				ImGui::SFML::Update(_window, clock.restart());
			}

			{
				_window.clear(sf::Color::Black);
				{
					sf::RenderStates states;
					_scenes.Render(_window, states);
					ImGui::SFML::Render(_window);
				}
				_window.display();
			}

			this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		*/
		return 0;
	}

	bool _exit = false;

	virtual bool HandleEvents() = 0;

	//Scenes _scenes;

#ifdef KAZIN_GRAPHICS
	void AdjustViewport(const GLsizei& x, const GLsizei& y)
	{
		glViewport(0, 0, x, y);
	}
#endif

#endif

public:
#ifdef KAZIN_SFML
	sf::RenderWindow _window;
#endif
};
}

template<typename T>
class Point
{
public:
	T x = 0;
	T y = 0;

	using Type = T;
	using PointT = Point<T>;

	constexpr Point() = default;
	constexpr Point(T vx, T vy) noexcept : x(vx), y(vy) {}

#ifdef CLIENT
	constexpr Point(const sf::Vector2<T>& v) noexcept : x(v.x), y(v.y) {}
	operator sf::Vector2<T>() const
	{
		return { x, y };
	}
#endif

	constexpr auto& This() noexcept
	{
		return static_cast<PointT&>(*this);
	}
	constexpr auto& This() const noexcept
	{
		return static_cast<const PointT&>(*this);
	}

	template <typename U>
	constexpr auto As() const
	{
		return Point<U>(static_cast<U>(x), static_cast<U>(y));
	}

	template <typename T>
	constexpr bool operator == (const Point<T>& r)
	{
		return x == r.x && y == r.y;
	}

	template <typename T>
	constexpr bool operator != (const Point<T>& r)
	{
		return !(operator==(r));
	}

	constexpr bool operator >= (const PointT& p) const noexcept
	{
		return (x >= p.x && y >= p.y);
	}

	constexpr PointT operator ++ (int) noexcept
	{
		PointT copy = This();
		++x;
		++y;
		return copy;
	}
	constexpr PointT operator -- (int) noexcept
	{
		PointT copy = This();
		--x;
		--y;
		return copy;
	}

	constexpr PointT& operator += (const PointT& p) noexcept
	{
		x += p.x;
		y += p.y;
		return This();
	}
	constexpr PointT& operator -= (const PointT& p) noexcept
	{
		x -= p.x;
		y -= p.y;
		return This();
	}
	constexpr PointT& operator *= (const PointT& p) noexcept
	{
		x *= p.x;
		y *= p.y;
		return This();
	}
	constexpr PointT& operator /= (const PointT& p) noexcept
	{
		x /= p.x;
		y /= p.y;
		return This();
	}

	constexpr PointT& operator += (const T& v) noexcept
	{
		x += v;
		y += v;
		return This();
	}
	constexpr PointT& operator -= (const T& v) noexcept
	{
		x -= v;
		y -= v;
		return This();
	}
	constexpr PointT& operator *= (const T& v) noexcept
	{
		x *= v;
		y *= v;
		return This();
	}
	constexpr PointT& operator /= (const T& v) noexcept
	{
		x /= v;
		y /= v;
		return This();
	}

	constexpr PointT operator + (const T& v) const noexcept
	{
		PointT copy = This();
		return (copy += v);
	}
	constexpr PointT operator - (const T& v) const noexcept
	{
		PointT copy = This();
		return (copy -= v);
	}

	constexpr PointT operator + (const PointT& p) const noexcept
	{
		PointT copy = This();
		return (copy += p);
	}
	constexpr PointT operator - (const PointT& p) const noexcept
	{
		PointT copy = This();
		return (copy -= p);
	}
	constexpr PointT operator / (const PointT& p) const noexcept
	{
		PointT copy = This();
		return (copy /= p);
	}
	constexpr PointT operator * (const PointT& p) const noexcept
	{
		PointT copy = This();
		return (copy *= p);
	}
	constexpr PointT operator / (const T& v) const noexcept
	{
		PointT copy = This();
		return (copy /= v);
	}
	constexpr PointT operator * (const T& v) const noexcept
	{
		PointT copy = This();
		return (copy *= v);
	}

	template <typename U>
	constexpr PointT operator + (const Point<U>& p) const noexcept
	{
		return{ x + p.x, y + p.y };
	}
	template <typename U>
	constexpr PointT operator - (const Point<U>& p) const noexcept
	{
		return{ x - p.x, y - p.y };
	}

	constexpr PointT operator-() const noexcept
	{
		return { -x, -y };
	}

	constexpr auto Area() const noexcept
	{
		return x * y;
	}

	template <typename LenT>
	PointT Project(float angle, LenT len) noexcept
	{
		return{ T(x + cos(angle) * len),
				 T(y + sin(angle) * len) };
	}

	template <typename LenT>
	PointT Project(PointT p, LenT len) noexcept
	{
		// #todo: dot product / magnitude is probably faster
		auto vec = p - This();
		return Project((float)atan2(vec.y, vec.x), len);
	}

	auto Floor() noexcept
	{
		return PointT{ (T)floor(x), (T)floor(y) };
	}
	auto Ceil() noexcept
	{
		return PointT{ (T)ceil(x), (T)ceil(y) };
	}
	auto Round() noexcept
	{
		return PointT{ (T)round(x), (T)round(y) };
	}
	auto Abs() noexcept
	{
		return PointT{ (T)abs(x), (T)abs(y) };
	}
};

namespace settings
{
class Base
{
public:

};
}

inline void ToLower(string& str)
{
	std::locale loc;
	for (auto& c : str)
		c = std::tolower<string::value_type>(c, loc);
}

inline void ToUpper(string& str)
{
	std::locale loc;
	for (auto& c : str)
		c = std::toupper<string::value_type>(c, loc);
}

inline bool Fire(bool & flag)
{
	if(flag)
	{
		flag = false;
		return true;
	}

	return false;
}
}