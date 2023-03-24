#include "pch.h"
#include "kazilib.h"

namespace kazin::global
{
	unique_ptr<Random> g_rand;
}

namespace kazin::app
{
	Base::Base([[maybe_unused]] const std::string& name)
	{
#ifdef LIVEPP
		_live_pp = lpp::lppLoadAndRegister(L"D:\\LIBPATH\\LivePP", name.c_str());
		lpp::lppEnableAllCallingModulesAsync(_live_pp);
#endif

		//global::Initialize();
		g_rand = make_unique<Random>();
	}
}
