// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define KAZIN_SFML
#define KAZIN_SFML_IMGUI

#include "framework.h"
#include <types.h>
#include <console/log.h>

#ifdef KAZIN_WINDOWS
#include <windows.h>
#endif

#include "kazilib.h"

#ifdef KAZIN_NET_FROM_SOURCE
	#define KAZIN_ENET
	#define _WINSOCKAPI_
	#define NOMINMAX

	#pragma comment(lib, "ws2_32.lib")
	#pragma comment(lib, "enet.lib")
	#include <enet/enet.h>

	namespace common
	{
		// #net_op Replace by higher number in projects and lib in case of need,
		// it probably has to synchronize in type
		enum NetOp : u8
		{
			
		};
	}

	#include "packet.h"
	#include "socket.h"
	#include "broadcaster.h"
	#include "auth.h"
#endif

#endif //PCH_H
