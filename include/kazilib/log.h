#pragma once

namespace kazin
{
enum class LogType
{
	Debug,
	Error,
	Out,
};

template<LogType T>
class ConsoleOut
{
public:
	using Type = LogType;

	ConsoleOut(const char * str) :
		_ptr(str)
	{
	}

	~ConsoleOut();

#ifndef CLIENT
	//std::mutex _mutex;
#endif
	const char * _ptr;
};

namespace Log
{
	using Type = LogType;

	template<typename ... Args>
	void Debug(const string & str)
	{
		ConsoleOut<Type::Debug>(str.c_str());
	}

	template<typename ... Args>
	void Error(const string & str)
	{
		ConsoleOut<Type::Error>(str.c_str());
	}
}
}

template <typename ...Args>
void Print(std::_Fmt_string<Args...> fmt, Args &&... args)
{
	auto str = std::vformat(fmt._Str, std::make_format_args(args...));
	kazin::ConsoleOut<kazin::LogType::Out>(str.c_str());
}

template <typename ...Args>
void PrintDebug(std::_Fmt_string<Args...> fmt, Args &&... args)
{
	auto str = std::vformat(fmt._Str, std::make_format_args(args...));
	kazin::ConsoleOut<kazin::LogType::Debug>(str.c_str());
}

template <typename ...Args>
void PrintError(std::_Fmt_string<Args...> fmt, Args &&... args)
{
	auto str = std::vformat(fmt._Str, std::make_format_args(args...));
	kazin::ConsoleOut<kazin::LogType::Error>(str.c_str());
}

inline void Print(const char* str)
{
	Print("{}", str);
}

inline void PrintDebug(const char* str)
{
	PrintDebug("{}", str);
}

inline void PrintError(const char* str)
{
	PrintError("{}", str);
}