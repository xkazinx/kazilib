#include <pch.h>
#include "log.h"
#include "rang.hpp"

namespace sdlpp
{
template<LogType T>
ConsoleOut<T>::~ConsoleOut()
{
	// #todo #log #thread detect if program has threads or set a macro

#ifndef CLIENT
	//unique_mutex lock(_mutex);
#endif

	switch(T)
	{
		using enum rang::fg;
		using enum LogType;

	case Out:
		cout << green;
		break;
	case Error:
		cout << red;
		break;
	case Debug:
		cout << cyan;
		break;

	default: ;
	}

	// #todo #log into print data into files, #optional with flag to enable or disable it

	cout << (const char*)(_ptr ? _ptr : _str.c_str()) << "\n";
}

template ConsoleOut<LogType::Out>::~ConsoleOut<LogType::Out>();
template ConsoleOut<LogType::Error>::~ConsoleOut<LogType::Error>();
template ConsoleOut<LogType::Debug>::~ConsoleOut<LogType::Debug>();
}
